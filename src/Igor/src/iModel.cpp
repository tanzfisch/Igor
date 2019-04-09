// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iModel.h>
#include <iNodeFactory.h>

#include <iaString.h>
#include <iaConsole.h>
using namespace IgorAux;

#include <fstream>
using namespace std;

namespace Igor
{

    iModel::iModel(const iaString& name, iResourceCacheMode cacheMode, iModelDataInputParameter* parameter)
        : _name(name)
        , _parameter(parameter)
        , _cacheMode(cacheMode)
	{
	}

	iModel::~iModel()
	{
        if (nullptr != _node)
        {            
            iNodeFactory::getInstance().destroyNodeAsync(_node);
        }

        if (_parameter != nullptr)
        {
            delete _parameter;
            _parameter = nullptr;
        }
	}

    iResourceCacheMode iModel::getCacheMode() const
    {
        return _cacheMode;
    }

    iModelDataInputParameter* iModel::getParameters()
    {
        return _parameter;
    }

    const iaString& iModel::getName() const
    {
        return _name;
    }

	iModelState iModel::getState()
	{
		return _state;
	}

	void iModel::setState(iModelState state)
	{
		_state = state;
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
            return iNodeFactory::getInstance().createCopy(_node);
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
	

}
