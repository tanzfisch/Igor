// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Widget3D.h"

Widget3D::Widget3D(iWindow* window, iView* view, iScene* scene)
{
	_scene = scene;
	_view = view;
	_window = window;
}

void Widget3D::setNodeID(uint64 nodeID)
{
	if (_nodeID == nodeID)
	{
		return;
	}

	_nodeID = nodeID;
	update();
}

uint64 Widget3D::getNodeID() const
{
	return _nodeID;
}


