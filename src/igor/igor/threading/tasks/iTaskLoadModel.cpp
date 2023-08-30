// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/tasks/iTaskLoadModel.h>

#include <igor/threading/iTaskManager.h>
#include <igor/resources/model/iModelResourceFactory.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/system/iWindow.h>

namespace igor
{

    iTaskLoadModel::iTaskLoadModel(iWindowPtr window, iModel_OldPtr model, iTaskContext taskContext, uint32 priority)
        : iTask(window, priority, false, taskContext)
    {
        _model = model;
    }

    void iTaskLoadModel::run()
    {
        iNodePtr node = iModelResourceFactory::getInstance().loadData(_model->getName(), _model->getParameters());
        if (node != nullptr)
        {
            _model->setNode(node);
            _model->setState(iModelState::Loaded);
        }
        else
        {
            _model->setState(iModelState::LoadFailed);
        }
    }

}; // namespace igor
