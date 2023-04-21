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

#ifndef __USERCONTROLMATERIAL_H__
#define __USERCONTROLMATERIAL_H__

#include <igor/igor.h>
using namespace iaux;
using namespace igor;

/*! material name change event
*/
IGOR_SIGNAL(MaterialNameChangedEvent, MaterialNameChangedDelegate, (), ());

class UserControlMaterial : public iUserControl
{
public:
	UserControlMaterial();
	~UserControlMaterial();

	void setMaterial(const iMaterialID &materialID);
	const iMaterialID& getMaterialID() const;

	void registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);
	void unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);

private:
	MaterialNameChangedEvent _materialNameChangedEvent;

	iWidgetLineTextEdit *_textName;
    iWidgetLabel *_textID;
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

	iMaterialID _materialID;

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

	void updateGUI();
	void updateMaterial();

	void initGUI();
	void deinitGUI();

    void reloadShader(iMaterialPtr material);
};

#endif // __USERCONTROLMATERIAL_H__