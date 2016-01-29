// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeCamera.h>

#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	
	iNodeCamera::iNodeCamera()
		: iNode()
	{
		setName(L"iNodeCamera");
		_nodeType = iNodeType::iNodeCamera;
		_nodeKind = iNodeKind::Camera;
	}

    iNodeCamera::iNodeCamera(iNodeCamera* node)
        : iNode()
    {
        con_assert(node != nullptr, "zero pointer");

        _nodeType = node->_nodeType;
        _nodeKind = node->_nodeKind;

        setName(node->getName());
    }

	void iNodeCamera::getWorldMatrix(iaMatrixf& matrix)
	{
		matrix = _worldMatrix;
	}

    void iNodeCamera::onUpdateTransform(iaMatrixf& matrix)
	{
		_worldMatrix = matrix;
	}

	void iNodeCamera::getViewMatrix(iaMatrixf& viewmatrix)
	{
        viewmatrix.lookAt(_worldMatrix._pos, _worldMatrix._pos - _worldMatrix._depth, _worldMatrix._top);
	}

	void iNodeCamera::makeCurrent()
	{
		if (getScene())
		{
			getScene()->setCamera(this);
		}
	}

	void iNodeCamera::onPreSetScene()
	{
		if (getScene())
		{
			getScene()->unregisterCamera(this);
		}
	}

	void iNodeCamera::onPostSetScene()
	{
		if (getScene())
		{
			getScene()->registerCamera(this);
		}
	}

};
