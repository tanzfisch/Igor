// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iTaskLoadModel.h>
#include <iTaskManager.h>
#include <iModelResourceFactory.h>
#include <iNodeMesh.h>
#include <iWindow.h>

namespace Igor
{

    iTaskLoadModel::iTaskLoadModel(iWindow* window, shared_ptr<iModel> model, bool needsRenderContext, uint32 priority)
        : iTask(window, priority, false, needsRenderContext)
    {
        _model = model;
    }

    void iTaskLoadModel::run()
    {
        iNode* node = iModelResourceFactory::getInstance().loadData(_model->getName(), _model->getParameters());
        if (node != nullptr)
        {
            _model->setData(node);
            _model->setState(iModelState::Loaded);
        }
        else
        {
            _model->setState(iModelState::LoadFailed);
        }

        _isRunning = false;
    }

};
