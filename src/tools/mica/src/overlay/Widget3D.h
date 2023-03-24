//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2023 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __WIDGET3D_H__
#define __WIDGET3D_H__

#include <igor/igor.h>
using namespace igor;

/*! 3d space widget base class for representing nodes
*/
class Widget3D
{

public:
	/*! init 3d widget

	\param window the window this widget is visible at
	\param view the view this widget it visible at
	\param scene the scene to use for the 3d widget
	\param nodeID id of the node to represent
	*/
	Widget3D(iWindowPtr window, iView *view, iScenePtr scene);

	/*! default dtor
	*/
	virtual ~Widget3D() = default;

	/*! specifies which node is to be manipulated or represented by this widget

	\param nodeID the nodes id
	*/
	void setNodeID(iNodeID nodeID);

	/*! \returns the node id
	*/
	iNodeID getNodeID() const;

protected:
	/*! renders the 3d widget
	*/
	virtual void update() = 0;

	/*! the scene to use for the 3d widget
	*/
	iScenePtr _scene = nullptr;

	/*! the window this widget is visible at
	*/
	iWindowPtr _window = nullptr;

	/*! the view this widget it visible at
	*/
	iView *_view = nullptr;

	/*! id of node to represent
	*/
	iNodeID _nodeID = iNode::INVALID_NODE_ID;
};

#endif // __WIDGET3D_H__
