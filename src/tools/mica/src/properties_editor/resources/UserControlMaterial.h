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

	/*! cleanup
	*/
	~UserControlMaterial();

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
	MaterialNameChangedEvent _materialNameChangedEvent;

	iWidgetNumberChooser *_renderingOrder;

	iWidgetCheckBox *_checkBoxCullFace;
	iWidgetCheckBox *_checkBoxDepthTest;
	iWidgetCheckBox *_checkBoxDepthMask;
	iWidgetCheckBox *_checkBoxBlend;
	iWidgetCheckBox *_checkBoxWireframe;
	iWidgetCheckBox *_checkBoxInstanced;
	iWidgetSelectBox *_selectBoxDepthFunc;
	iWidgetSelectBox *_selectBoxCullFaceFunc;
	iWidgetSelectBox *_selectBoxInstancedFunc;

	iWidgetLineTextEdit *_textShaderGeometry;
	iWidgetLineTextEdit *_textShaderVertex;
	iWidgetLineTextEdit *_textShaderFragment;

	iWidgetButton *_shader0Button;
	iWidgetButton *_shader1Button;
	iWidgetButton *_shader2Button;

    iWidgetButton *_shaderReload;
    iWidgetButton *_exportMaterial;

	uint32 _loadShaderNumber = 0;

	iDialogFileSelectPtr _fileDialog = nullptr;

	bool _ignoreMaterialUpdate = false;

	void onShader0Button(const iWidgetPtr source);
	void onShader1Button(const iWidgetPtr source);
	void onShader2Button(const iWidgetPtr source);

	void onTextChangedName(const iWidgetPtr source);
	void onDoUpdateMaterial(const iWidgetPtr source);

	void onFileLoadDialogClosed(iDialogPtr dialog);
    void onExportMaterialDialogClosed(iDialogPtr dialog);

    void onReloadShader(const iWidgetPtr source);
    void onExportMaterial(const iWidgetPtr source);

    void reloadShader(iMaterialPtr material);
};

#endif // __USERCONTROLMATERIAL__