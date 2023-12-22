// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/iModelFactory.h>

#include <igor/resources/model/loader/iModelDataIOOMPF.h>
#include <igor/resources/model/loader/iModelDataIOOBJ.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/system/iaFile.h>
#include <iaux/data/iaConvert.h>
using namespace iaux;

namespace igor
{

    iModelFactory::iModelFactory()
        : iFactory(IGOR_RESOURCE_MODEL, IGOR_SUPPORTED_MODEL_EXTENSIONS)
    {
    }

    class iModelDeleter
    {
    public:
        void operator()(iModel *p) { delete p; }
    };

    /*! map of model data IOs
     */
    static std::map<iaString, iCreateModelDataIOInstance> _modelDataIOCreators =
        {
            {iaString("ompf"), &iModelDataIOOMPF::createInstance},
            {iaString("obj"), &iModelDataIOOBJ::createInstance}};

    /*! mutex to lock model data IO list
     */
    static iaMutex _mutexDataIOs;

    iResourcePtr iModelFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iModel(parameters), iModelDeleter());
    }

    bool iModelFactory::loadResource(iResourcePtr resource)
    {
        // copy parameters to add filename
        auto parameters = resource->getParameters();

        iaString filename = iResourceManager::getInstance().getFilename(resource->getID());
        if (filename.isEmpty())
        {
            filename = resource->getSource();
        }

        filename = iResourceManager::getInstance().resolvePath(filename);
        if (!filename.isEmpty())
        {
            parameters.setParameter(IGOR_RESOURCE_PARAM_SOURCE, filename);
        }

        iaString subType = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SUB_TYPE, "");
        if (subType.isEmpty())
        {
            iaFile file(filename);
            subType = file.getExtension();
        }

        if (subType.isEmpty())
        {
            con_err("no subType specified to load \"" << filename << "\"");
            return false;
        }

        auto modelDataIO = getModelDataIO(subType);
        if (modelDataIO == nullptr)
        {
            con_err("unknown model data io type \"" << subType << "\" for \"" << filename << "\"");
            return false;
        }

        iNodePtr node = modelDataIO->importData(parameters);
        if (node == nullptr)
        {
            con_err("failed to load \"" << filename << "\"");
            return false;
        }

        iModelPtr model = std::dynamic_pointer_cast<iModel>(resource);
        model->setNode(node);
        return true;
    }

    void iModelFactory::unloadResource(iResourcePtr resource)
    {
        // nothing to do
    }

    iaString iModelFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO

        return hashData;
    }

    void iModelFactory::registerModelDataIO(const iaString &identifier, iCreateModelDataIOInstance functionPointer)
    {
        _mutexDataIOs.lock();
        auto generatorIter = _modelDataIOCreators.find(identifier);
        if (generatorIter == _modelDataIOCreators.end())
        {
            _modelDataIOCreators[identifier] = functionPointer;
        }
        else
        {
            con_err("model data io " << identifier << " already registered");
        }
        _mutexDataIOs.unlock();
    }

    void iModelFactory::unregisterModelDataIO(const iaString &identifier)
    {
        _mutexDataIOs.lock();
        auto generatorIter = _modelDataIOCreators.find(identifier);
        if (generatorIter != _modelDataIOCreators.end())
        {
            _modelDataIOCreators.erase(generatorIter);
        }
        else
        {
            con_err("model data io " << identifier << " was not registered");
        }
        _mutexDataIOs.unlock();
    }

    std::unique_ptr<iModelDataIO> iModelFactory::getModelDataIO(const iaString &identifier)
    {
        iModelDataIO *result = nullptr;

        _mutexDataIOs.lock();
        auto iter = _modelDataIOCreators.find(identifier);
        if (iter != _modelDataIOCreators.end())
        {
            result = (*iter).second();
        }
        _mutexDataIOs.unlock();

        return std::unique_ptr<iModelDataIO>(result);
    }

    void iModelFactory::exportToFile(const iaString &filename, iNodePtr node, iSaveMode saveMode, const iaString &formatIdentifier)
    {
        con_assert_sticky(node != nullptr, "zero pointer");

        iaFile file(filename);
        const iaString format = formatIdentifier != "" ? formatIdentifier : file.getExtension();

        auto modelDataIO = getModelDataIO(format);
        if (modelDataIO != nullptr)
        {
            modelDataIO->exportData(filename, node, saveMode);
            con_info("exported " << format << " \"" << filename << "\"");
        }
        else
        {
            con_err("format \"" << format << "\" not supported");
        }
    }

}; // namespace igor