// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/iModel.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <fstream>

namespace igor
{   

    iModel::iModel(const iParameters &parameters)
        : iResource(parameters)
    {
    }

    iModel::~iModel()
    {
        if (nullptr != _node)
        {
            iNodeManager::getInstance().destroyNodeAsync(_node);
        }
    }

    void iModel::setNode(iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");
        con_assert(_node == nullptr, "can't override data");

        if (node != nullptr &&
            _node == nullptr)
        {
            _node = node;
        }
    }

    iNodePtr iModel::getNodeCopy()
    {    
        if (_node != nullptr)
        {
            return iNodeManager::getInstance().createCopy(_node);
        }
        else
        {
            con_err("zero pointer. data not ready");
            return nullptr;
        }
    }

    iNodePtr iModel::getNode()
    {
        return _node;
    }

} // namespace igor
