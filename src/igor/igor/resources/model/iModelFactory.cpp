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

    const iaString &iModelFactory::getType() const
    {
        const static iaString typeName(L"model");
        return typeName;
    }

    iResourcePtr iModelFactory::createResource(const iParameters &parameters)
    {
        return iResourcePtr(new iModel(parameters), iModelDeleter());
    }

    bool iModelFactory::loadResource(iResourcePtr resource)
    {
        const iaString filename = iResourceManager::getInstance().getPath(resource->getName());
        iModelPtr model = std::dynamic_pointer_cast<iModel>(resource);

        const auto &parameters = resource->getParameters();

        iaString iotype = parameters.getParameter<iaString>("iotype", "");
        if (iotype == "")
        {
            iaFile file(filename);
            iotype = file.getExtension();
        }

        if (iotype == "")
        {
            con_err("no iotype specified to load \"" << filename << "\"");
            return false;
        }

        auto modelDataIO = getModelDataIO(iotype);
        if (modelDataIO == nullptr)
        {
            con_err("unknown model data io type \"" << iotype << "\" for \"" << filename << "\"");
            return false;
        }

        iNodePtr node = modelDataIO->importData(filename);
        if (node == nullptr)
        {
            con_err("failed to load \"" << filename << "\"");
            return false;
        }
        else
        {
            con_info("loaded " << iotype << " \"" << filename << "\"");
        }

        model->setNode(node);
        return true;
    }

    void iModelFactory::unloadResource(iResourcePtr resource)
    {
        con_info("released model \"" << resource->getName() << "\"");
    }

    iaString iModelFactory::getHashData(const iParameters &parameters) const
    {
        iaString hashData;

        // TODO

        return hashData;
    }

    bool iModelFactory::matchingType(const iParameters &parameters) const
    {
        if (parameters.getParameter<iaString>("type") == getType())
        {
            return true;
        }

        iaFile file(parameters.getParameter<iaString>("name"));
        const iaString &fileExtension = file.getExtension();
        static const std::vector<iaString> supportedExtensions = {L"ompf", L"obj"};

        for (const auto &extension : supportedExtensions)
        {
            if (fileExtension == extension)
            {
                return true;
            }
        }

        return false;
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

}; // namespace igor