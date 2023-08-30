// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/iModelResourceFactory.h>

#include <igor/resources/model/iModel_Old.h>
#include <igor/resources/iResourceManager.h>
#include <igor/threading/tasks/iTaskLoadModel.h>
#include <igor/threading/iTaskManager.h>
#include <igor/resources/mesh/iMesh.h>

#include <iaux/system/iaFile.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <fstream>
#include <vector>

namespace igor
{

    iModelResourceFactory::iModelResourceFactory()
    {
        registerModelDataIO("ompf", &iModelDataIOOMPF::createInstance);
        registerModelDataIO("obj", &iModelDataIOOBJ::createInstance);

        iTaskManager::getInstance().registerTaskFinishedDelegate(iTaskFinishedDelegate(this, &iModelResourceFactory::onTaskFinished));
    }

    iModelResourceFactory::~iModelResourceFactory()
    {
        while (flush(nullptr, iResourceCacheMode::Keep))
            ;

        iTaskManager::getInstance().unregisterTaskFinishedDelegate(iTaskFinishedDelegate(this, &iModelResourceFactory::onTaskFinished));

        unregisterModelDataIO("ompf");
        unregisterModelDataIO("obj");

        if (!_models.empty())
        {
            con_warn("possible mem leak! Still " << _models.size() << " model references left");
        }

        if (!_modelDataIOCreators.empty())
        {
            con_warn("possible mem leak! Still " << _modelDataIOCreators.size() << " generators registered");
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

    iModelDataIO *iModelResourceFactory::getModelDataIO(const iaString &identifier)
    {
        iModelDataIO *result = nullptr;
        iaString id = identifier;

        iaFile file(identifier);
        if (file.exist())
        {
            id = file.getExtension();
        }

        id.toLower();
        int64 hashValue = calcModelDataIOHashValue(id);

        _mutexDataIOs.lock();
        auto iter = _modelDataIOCreators.find(hashValue);
        if (iter != _modelDataIOCreators.end())
        {
            result = (*iter).second();
        }
        _mutexDataIOs.unlock();

        return result;
    }

    void iModelResourceFactory::exportModelData(iaString filename, iNodePtr node, const iaString &formatIdentifier, iSaveMode saveMode)
    {
        iaFile file(filename);
        if (iaDirectory::directoryIsAbsolute(file.getPath()))
        {
            con_assert(node != nullptr, "zero pointer");

            if (node != nullptr)
            {
                iModelDataIO *modelDataIO = getModelDataIO(formatIdentifier);
                if (modelDataIO != nullptr)
                {
                    modelDataIO->exportData(filename, node, saveMode);
                    con_info("exported " << formatIdentifier << " \"" << filename << "\"");
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
            con_err("can't export to relative folder " << file.getPath());
        }
    }

    iNodePtr iModelResourceFactory::loadData(iaString filename, iModelDataInputParameterPtr parameter)
    {
        iNodePtr result = nullptr;
        iModelDataIO *modelDataIO = getModelDataIO(filename);

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

            if (result == nullptr)
            {
                con_err("can't load: \"" << filename << "\"");
            }
        }
        else
        {
            if (parameter != nullptr)
            {
                con_err("can't find loader \"" << parameter->_identifier << "\" for \"" << filename << "\"");
            }
            else
            {
                con_err("can't find loader for \"" << filename << "\"");
            }
        }

        return result;
    }

    void iModelResourceFactory::registerModelDataIO(const iaString &identifier, iCreateModelDataIOInstance functionPointer)
    {
        int64 hashValue = calcModelDataIOHashValue(identifier);

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

    void iModelResourceFactory::unregisterModelDataIO(const iaString &identifier)
    {
        int64 hashValue = calcModelDataIOHashValue(identifier);

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

    int64 iModelResourceFactory::calcResourceHashValue(const iaString &text, iResourceCacheMode cacheMode)
    {
        std::hash<std::wstring> hashFunc;
        iaString combined = text;
        switch (cacheMode)
        {
        case iResourceCacheMode::Free:
            combined += "F";
            break;
        case iResourceCacheMode::Cache:
            combined += "C";
            break;
        case iResourceCacheMode::Keep:
            combined += "K";
            break;
        }

        std::wstring keyValue = combined.getData();
        return static_cast<int64>(hashFunc(keyValue));
    }

    int64 iModelResourceFactory::calcModelDataIOHashValue(const iaString &text)
    {
        std::hash<std::wstring> hashFunc;
        std::wstring keyValue = text.getData();
        return static_cast<int64>(hashFunc(keyValue));
    }

    iModel_OldPtr iModelResourceFactory::requestModelData(const iaString &filename, iResourceCacheMode cacheMode, iModelDataInputParameterPtr parameter)
    {
        iModel_OldPtr result;
        iaString hashKey;

        con_assert_sticky(filename != "", "invalid parameter");

        if (parameter != nullptr &&
            parameter->_modelSourceType != iModelSourceType::File)
        {
            hashKey = filename;
        }
        else
        {
            hashKey = iResourceManager::getInstance().getPath(filename);

            if (hashKey == "")
            {
                con_err("file not found " << filename);
                return result;
            }
        }

        int64 hashValue = calcResourceHashValue(hashKey, cacheMode);

        _mutexModels.lock();
        auto modelIter = _models.find(hashValue);
        if (modelIter != _models.end())
        {
            result = (*modelIter).second;
        }
        _mutexModels.unlock();

        if (nullptr == result.get())
        {
            result = iModel_OldPtr(new iModel_Old(hashKey, cacheMode, parameter));
            _mutexModels.lock();
            _models[hashValue] = result;
            _mutexModels.unlock();

            _mutexModelQueue.lock();
            _loadingQueue.push_back(result);
            _mutexModelQueue.unlock();
        }

        return result;
    }

    iModel_OldPtr iModelResourceFactory::loadModelData(const iaString &filename, iResourceCacheMode cacheMode, iModelDataInputParameterPtr parameter)
    {
        iModel_OldPtr result;
        iaString hashKey;

        con_assert_sticky(filename != "", "invalid parameter");

        if (parameter != nullptr &&
            parameter->_modelSourceType != iModelSourceType::File)
        {
            hashKey = filename;
        }
        else
        {
            hashKey = iResourceManager::getInstance().getPath(filename);

            if (hashKey == "")
            {
                con_err("file not found " << filename);
                return result;
            }
        }

        int64 hashValue = calcResourceHashValue(hashKey, cacheMode);

        _mutexModels.lock();
        auto modelIter = _models.find(hashValue);
        if (modelIter != _models.end())
        {
            result = (*modelIter).second;
        }
        _mutexModels.unlock();

        if (nullptr == result.get())
        {
            iNodePtr node = loadData(hashKey, parameter);
            if (node != nullptr)
            {
                result = iModel_OldPtr(new iModel_Old(hashKey, cacheMode, parameter));
                result->setNode(node);
                result->setState(iModelState::Loaded);

                _mutexModels.lock();
                _models[hashValue] = result;
                _mutexModels.unlock();
            }
            else
            {
                result = iModel_OldPtr(new iModel_Old(hashKey, cacheMode, parameter));
                result->setState(iModelState::LoadFailed);

                _mutexModels.lock();
                _models[hashValue] = result;
                _mutexModels.unlock();
            }
        }

        return result;
    }

    bool iModelResourceFactory::flush(iWindowPtr window, iResourceCacheMode cacheModeLevel)
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
                    if ((*modelIter).second->_cacheMode <= cacheModeLevel)
                    {
                        if ((*modelIter).second->getParameters() != nullptr &&
                            (*modelIter).second->getParameters()->_modelSourceType == iModelSourceType::File)
                        {
                            if (!(*modelIter).second->getName().isEmpty())
                            {
                                con_info("released model \"" << (*modelIter).second->getName() << "\"");
                            }
                        }

                        modelIter = _models.erase(modelIter);
                        continue;
                    }
                }
            }
            modelIter++;
        }
        _mutexModels.unlock();

        std::vector<iModel_OldPtr> modelsToProcess;

        _mutexModelQueue.lock();
        modelsToProcess = std::move(_loadingQueue);
        _mutexModelQueue.unlock();

        for (auto model : modelsToProcess)
        {
            iTaskContext taskContext = iTaskContext::RenderContext;
            uint32 priority = iTask::DEFAULT_PRIORITY;
            if (model->getParameters() != nullptr)
            {
                taskContext = model->getParameters()->_needsRenderContext ? iTaskContext::RenderContext : iTaskContext::Default;
                priority = model->getParameters()->_loadPriority;
            }

            iTaskLoadModel *task = new iTaskLoadModel(window, model, taskContext, priority);
            iTaskManager::getInstance().addTask(task);

            _mutexRunningTasks.lock();
            _runningTasks.push_back(task->getID());
            _mutexRunningTasks.unlock();
        }

        _mutexRunningTasks.lock();
        if (_runningTasks.empty())
        {
            result = false;
        }
        _mutexRunningTasks.unlock();

        return result;
    }

} // namespace igor
