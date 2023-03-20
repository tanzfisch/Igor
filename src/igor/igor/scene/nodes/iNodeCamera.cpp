// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeCamera.h>

#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

	iNodeCamera::iNodeCamera()
		: iNode()
	{
		setName(L"iNodeCamera");
		_nodeType = iNodeType::iNodeCamera;
		_nodeKind = iNodeKind::Camera;
	}

	iNodeCamera::iNodeCamera(iNodeCamera *node)
		: iNode()
	{
		con_assert(node != nullptr, "zero pointer");

		_nodeType = node->_nodeType;
		_nodeKind = node->_nodeKind;

		setName(node->getName());
	}

	iNodeCamera::~iNodeCamera()
	{
		setScene(nullptr);
	}

	void iNodeCamera::getWorldMatrix(iaMatrixd &matrix)
	{
		matrix = _worldMatrix;
	}

	void iNodeCamera::onUpdateTransform(iaMatrixd &matrix)
	{
		_worldMatrix = matrix;
	}

	void iNodeCamera::getViewMatrix(iaMatrixd &viewmatrix)
	{
		viewmatrix.lookAt(_worldMatrix._pos, _worldMatrix._pos - _worldMatrix._depth, _worldMatrix._top);
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

}; // namespace igor
