// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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

    iModel::iModel(const iaString& name, iModelDataInputParameter* parameter)
	{
		_name = name;
        _parameter = parameter;
	}

	iModel::~iModel()
	{
        if (nullptr != _data)
        {
            iNodeFactory::getInstance().destroyNode(_data);
        }

        if (_parameter != nullptr)
        {
            delete _parameter;
            _parameter = nullptr;
        }
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

	void iModel::setData(iNode* node)
	{
        con_assert(node != nullptr, "zero pointer");
        con_assert(_data == nullptr, "can't override data");

        if (node != nullptr &&
            _data == nullptr)
        {
            _data = node;
        }
	}

    iNode* iModel::getDataCopy()
    {
        con_assert(_data != nullptr, "zero pointer. data not ready");
         
        return iNodeFactory::getInstance().createCopy(_data);
    }

	iNode* iModel::getData()
	{
		return _data;
	}
	

}
