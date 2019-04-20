// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include "Widget3D.h"

Widget3D::Widget3D(iWindow* window, iView* view, iScene* scene, uint64 nodeID)
{
	_scene = scene;
	_view = view;
	_window = window;
	_nodeID = nodeID;
}


