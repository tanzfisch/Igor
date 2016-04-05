// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iaFile.h>
#include <iaString.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <iModelResourceFactory.h>
#include <iModel.h>
#include <iResourceManager.h>
#include <iTaskLoadModel.h>
#include <iTaskManager.h>

#include <iMesh.h>
#include <iSphere.h>

#include <fstream>
#include <vector>
using namespace std;

namespace Igor
{

    iModelResourceFactory::iModelResourceFactory()
    {
        registerModelDataIO("ompf", &iModelDataIOOMPF::createInstance);
        registerModelDataIO("obj", &iModelDataIOOBJ::createInstance);

        iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &iModelResourceFactory::onTaskFinished));
    }

    iModelResourceFactory::~iModelResourceFactory()
    {
        while(flush(nullptr));

        iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iModelResourceFactory::onTaskFinished));

        unregisterModelDataIO("ompf");
        unregisterModelDataIO("obj");

        if (!_models.empty())
        {
            con_err("possible mem leak! Still " << _models.size() << " model references left");
        }

        if (!_modelDataIOCreators.empty())
        {
            con_err("possible mem leak! Still " << _modelDataIOCreators.size() << " generators registered");
        }
    }

    void iModelResourceFactory::onTaskFinished(uint64 taskID)
    {
        _mutexRunningTasks.lock();
        auto iter = find(_runningTasks.begin(), _runningTasks.end(), taskID);
        if (iter != _runningTasks.end())
        {
            _runningTasks.erase(iter);
        }
        _mutexRunningTasks.unlock();
    }

    iModelDataIO* iModelResourceFactory::getModelDataIO(const iaString& identifier)
    {
        iModelDataIO* result = nullptr;
        iaString id = identifier;

        iaFile file(identifier);
        if (file.exist())
        {
            id = file.getExtension();
        }

        id.toLower();
        int64 hashValue = calcHashValue(id);

        _mutexDataIOs.lock();
        auto iter = _modelDataIOCreators.find(hashValue);
        if (iter != _modelDataIOCreators.end())
        {
            result = (*iter).second();
        }
        _mutexDataIOs.unlock();

        return result;
    }

    void iModelResourceFactory::exportModelData(iaString filename, iNode* node, const iaString& formatIdentifier, iSaveMode saveMode)
    {
        iaFile file(filename);
        if (iaDirectory::directoryIsAbsolute(file.getPath()))
        {
            if (node != nullptr)
            {
                iModelDataIO* modelDataIO = getModelDataIO(formatIdentifier);
                if (modelDataIO != nullptr)
                {
                    modelDataIO->exportData(filename, node, saveMode);
                    delete modelDataIO;
                }
                else
                {
                    con_err("format \"" << formatIdentifier << "\" not supported");
                }
            }
            else
            {
                con_err("can't export node with zero pointer");
            }
        }
        else
        {
            con_err("can't export to folder " << file.getPath());
        }
    }

    iNode* iModelResourceFactory::loadData(iaString filename, iModelDataInputParameter* parameter)
    {
        iNode* result = nullptr;
        iModelDataIO* modelDataIO = getModelDataIO(filename);

        if (modelDataIO == nullptr)
        {
            if (parameter != nullptr)
            {
                modelDataIO = getModelDataIO(parameter->_identifier);
            }
        }

        if (modelDataIO != nullptr)
        {
            result = modelDataIO->importData(filename, parameter);
            delete modelDataIO;

            if (!result)
            {
                con_err("can't load: \"" << filename << "\"");
            }
        }
        else
        {
            con_err("can't find loader for \"" << filename << "\"");
        }

        return result;
    }

    void iModelResourceFactory::interruptFlush()
    {
        _interrupFlush = true;
    }

    void iModelResourceFactory::registerModelDataIO(const iaString& identifier, CreateModelDataIOInstance functionPointer)
    {
        int64 hashValue = calcHashValue(identifier);

        _mutexDataIOs.lock();
        auto generatorIter = _modelDataIOCreators.find(hashValue);
        if (generatorIter == _modelDataIOCreators.end())
        {
            _modelDataIOCreators[hashValue] = functionPointer;
        }
        else
        {
            con_err("model data io " << identifier << " already registered");
        }
        _mutexDataIOs.unlock();
    }

    void iModelResourceFactory::unregisterModelDataIO(const iaString& identifier)
    {
        int64 hashValue = calcHashValue(identifier);

        _mutexDataIOs.lock();
        auto generatorIter = _modelDataIOCreators.find(hashValue);
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

    int64 iModelResourceFactory::calcHashValue(const iaString& text)
    {
        std::hash<wstring> hashFunc;
        wstring keyValue = text.getData();
        return static_cast<int64>(hashFunc(keyValue));
    }

    shared_ptr<iModel> iModelResourceFactory::requestModelData(const iaString& filename, iModelDataInputParameter* parameter)
    {
        shared_ptr<iModel> result;
        iaString hashKey;

        if (parameter != nullptr &&
            parameter->_modelSourceType == iModelSourceType::Generated)
        {
            hashKey = filename;
        }
        else
        {
            hashKey = iResourceManager::getInstance().getPath(filename);
        }

        int64 hashValue = calcHashValue(hashKey);

        _mutexModels.lock();
        auto modelIter = _models.find(hashValue);
        if (modelIter != _models.end())
        {
            result = (*modelIter).second;
        }
        _mutexModels.unlock();

        if (nullptr == result.get())
        {
            result = shared_ptr<iModel>(new iModel(hashKey, parameter), iModel::private_deleter());
            _mutexModels.lock();
            _models[hashValue] = result;
            _mutexModels.unlock();

            _mutexModelQueue.lock();
            _loadingQueue.push_back(result);
            _mutexModelQueue.unlock();
        }

        return result;
    }

    shared_ptr<iModel> iModelResourceFactory::loadModelData(const iaString& filename, iModelDataInputParameter* parameter)
    {
        shared_ptr<iModel> result;
        iaString hashKey;

        if (parameter != nullptr &&
            parameter->_modelSourceType == iModelSourceType::Generated)
        {
            hashKey = filename;
        }
        else
        {
            hashKey = iResourceManager::getInstance().getPath(filename);
        }

        int64 hashValue = calcHashValue(hashKey);

        _mutexModels.lock();

        auto modelIter = _models.find(hashValue);
        if (modelIter != _models.end())
        {
            result = (*modelIter).second;
        }

        if (nullptr == result.get())
        {
            iNode* node = loadData(hashKey, parameter);
            if (node != nullptr)
            {
                result = shared_ptr<iModel>(new iModel(hashKey, parameter), iModel::private_deleter());
                result->setData(node);
                result->setState(iModelState::Loaded);
                _models[hashValue] = result;
            }
            else
            {
                result = shared_ptr<iModel>(new iModel(hashKey, parameter), iModel::private_deleter());
                result->setState(iModelState::LoadFailed);
                _models[hashValue] = result;
            }
        }

        _mutexModels.unlock();

        return result;
    }

    bool iModelResourceFactory::flush(iWindow* window)
    {        
        bool result = true;

        _mutexModels.lock();
        auto modelIter = _models.begin();
        while (modelIter != _models.end())
        {
            if ((*modelIter).second->getState() != iModelState::NotLoaded)
            {
                if ((*modelIter).second.use_count() == 1)
                {
                    if ((*modelIter).second->getParameters() != nullptr &&
                        (*modelIter).second->getParameters()->_modelSourceType == iModelSourceType::File)
                    {
                        if (!(*modelIter).second->getName().isEmpty())
                        {
                            con_info("released model", "\"" << (*modelIter).second->getName() << "\"");
                        }
                    }

                    modelIter = _models.erase(modelIter);
                    continue;
                }
            }
            modelIter++;
        }
        _mutexModels.unlock();

        if (!_interrupFlush)
        {
            vector<shared_ptr<iModel>> modelsToProcess;

            _mutexModelQueue.lock();
            modelsToProcess = std::move(_loadingQueue);
            _mutexModelQueue.unlock();

            for(auto model :modelsToProcess)
            {
                bool renderContext = true;
                uint32 priority = iTask::DEFAULT_PRIORITY;
                if (model->getParameters() != nullptr)
                {
                    renderContext = model->getParameters()->_needsRenderContext;
                    priority = model->getParameters()->_loadPriority;
                }

                iTaskLoadModel* task = new iTaskLoadModel(window, model, renderContext, priority);
                iTaskManager::getInstance().addTask(task);

                _mutexRunningTasks.lock();
                _runningTasks.push_back(task->getID());
                _mutexRunningTasks.unlock();
            }
        }

        _interrupFlush = false;

        _mutexRunningTasks.lock();
        if (_runningTasks.empty())
        {
            result = false;
        }
        _mutexRunningTasks.unlock();

        return result;
    }

}
