// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
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
        : iResource("model", parameters)
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
        con_assert(_node != nullptr, "zero pointer. data not ready");

        if (_node != nullptr)
        {
            return iNodeManager::getInstance().createCopy(_node);
        }
        else
        {
            return nullptr;
        }
    }

    iNodePtr iModel::getNode()
    {
        return _node;
    }

} // namespace igor
