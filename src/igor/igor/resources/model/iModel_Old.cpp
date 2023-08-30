// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/iModel_Old.h>
#include <igor/scene/nodes/iNodeManager.h>

#include <iaux/data/iaString.h>
#include <iaux/system/iaConsole.h>
using namespace iaux;

#include <fstream>

namespace igor
{

    iModel_Old::iModel_Old(const iaString &name, iResourceCacheMode cacheMode, iModelDataInputParameterPtr parameter)
        : _name(name), _parameter(parameter), _cacheMode(cacheMode)
    {
    }

    iModel_Old::~iModel_Old()
    {
        if (nullptr != _node)
        {
            iNodeManager::getInstance().destroyNodeAsync(_node);
        }
    }

    iResourceCacheMode iModel_Old::getCacheMode() const
    {
        return _cacheMode;
    }

    iModelDataInputParameterPtr iModel_Old::getParameters() const
    {
        return _parameter;
    }

    const iaString &iModel_Old::getName() const
    {
        return _name;
    }

    iModelState iModel_Old::getState()
    {
        return _state;
    }

    void iModel_Old::setState(iModelState state)
    {
        _state = state;
    }

    void iModel_Old::setNode(iNodePtr node)
    {
        con_assert(node != nullptr, "zero pointer");
        con_assert(_node == nullptr, "can't override data");

        if (node != nullptr &&
            _node == nullptr)
        {
            _node = node;
        }
    }

    iNodePtr iModel_Old::getNodeCopy()
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

    iNodePtr iModel_Old::getNode()
    {
        return _node;
    }

} // namespace igor
