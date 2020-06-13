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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: martinloga@gmx.de

#ifndef __WIDGET3DLOCATOR__
#define __WIDGET3DLOCATOR__

#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iMaterial.h>
using namespace igor;

#include "Widget3D.h"

namespace igor
{
	class iTargetMaterial;
	class iNodeTransform;
} // namespace igor

class Widget3DLocator : public Widget3D
{

public:
	/*! init 3d widget for loactors

	\param window the window this widget is visible at
	\param view the view this widget it visible at
	\param scene the scene to use for the 3d widget
	\param nodeID id of the node to represent
	*/
	Widget3DLocator(iWindow *window, iView *view, iScene *scene);

	/*! cleanup
	*/
	virtual ~Widget3DLocator() override;

private:
	/*! target materials
	*/
	iTargetMaterial *_red = nullptr;
	iTargetMaterial *_green = nullptr;
	iTargetMaterial *_blue = nullptr;

	iNodeTransform *_rootTransform = nullptr;

	uint64 _material = iMaterial::INVALID_MATERIAL_ID;

	/*! renders the 3d widget
	*/
	void update() override;

	/*! create a mesh the represents a locator
	*/
	iMeshPtr createLocatorMesh();

	/*! create the locator
	*/
	void createLocator();
};

#endif
