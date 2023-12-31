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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __USERCONTROLMATERIAL__
#define __USERCONTROLMATERIAL__

#include "UserControlResource.h"

/*! material name change event
*/
IGOR_EVENT_DEFINITION(MaterialNameChanged, void);

class UserControlMaterial : public UserControlResource
{
public:
    /*! init user control

    \param resourceID the resource id to use
    \param parent the optional parent widget
    */
	UserControlMaterial(iResourceID resourceID, const iWidgetPtr parent = nullptr);

	/*! does nothing
	*/
	~UserControlMaterial() = default;

    /*! init ui
     */
    virtual void init();

    /*! update ui with node data
     */
    virtual void update();

    /*! update node with ui data
     */
    virtual void updateResource();

private:

	/*! number chooser representing render order
	*/
	iWidgetNumberChooser *_renderingOrder;

	/*! checkbox representing cull face
	*/
	iWidgetCheckBox *_checkBoxCullFace;

	/*! checkbox representing depth test
	*/
	iWidgetCheckBox *_checkBoxDepthTest;

	/*! checkbox representing depth mask
	*/
	iWidgetCheckBox *_checkBoxDepthMask;

	/*! checkbox representing blend
	*/
	iWidgetCheckBox *_checkBoxBlend;

	/*! checkbox representing wireframe
	*/
	iWidgetCheckBox *_checkBoxWireframe;

	/*! checkbox representing instancing
	*/
	iWidgetCheckBox *_checkBoxInstanced;

	/*! select box representing depth function
	*/
	iWidgetSelectBox *_selectBoxDepthFunc;

	/*! select box representing cull face function
	*/
	iWidgetSelectBox *_selectBoxCullFaceFunc;

	/*! select box representing instancing function
	*/
	iWidgetSelectBox *_selectBoxInstancedFunc;

	/*! flag to prevent endless update loop
	*/
	bool _ignoreMaterialUpdate = false;

	/*! widget to display the material texture
	*/
	iWidgetPicturePtr _materialPicture;

	/*! triggers material update

	\param source the source widget of this event
	*/
	void onDoUpdateMaterial(const iWidgetPtr source);

	/*! updates material display render

	\param material the material to use
	*/
	void updateMaterialDisplay(iShaderMaterialPtr material);
};

#endif // __USERCONTROLMATERIAL__