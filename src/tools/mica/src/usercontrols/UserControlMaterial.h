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
iaEVENT(MaterialNameChangedEvent, MaterialNameChangedDelegate, (), ());

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

	iWidgetTextEdit *_textName = nullptr;
	iWidgetNumberChooser *_renderingOrder = nullptr;

	iWidgetCheckBox *_checkBoxCullFace = nullptr;
	iWidgetCheckBox *_checkBoxDepthTest = nullptr;
	iWidgetCheckBox *_checkBoxDepthMask = nullptr;
	iWidgetCheckBox *_checkBoxBlend = nullptr;
	iWidgetCheckBox *_checkBoxTexture2D0 = nullptr;
	iWidgetCheckBox *_checkBoxTexture2D1 = nullptr;
	iWidgetCheckBox *_checkBoxTexture2D2 = nullptr;
	iWidgetCheckBox *_checkBoxTexture2D3 = nullptr;
	iWidgetCheckBox *_checkBoxWireframe = nullptr;
	iWidgetCheckBox *_checkBoxInstanced = nullptr;
	iWidgetSelectBox *_selectBoxDepthFunc = nullptr;
	iWidgetSelectBox *_selectBoxCullFaceFunc = nullptr;
	iWidgetSelectBox *_selectBoxInstancedFunc = nullptr;

	iWidgetTextEdit *_textShaderGeometry = nullptr;
	iWidgetTextEdit *_textShaderVertex = nullptr;
	iWidgetTextEdit *_textShaderFragment = nullptr;

	iWidgetButton *_shader0Button = nullptr;
	iWidgetButton *_shader1Button = nullptr;
	iWidgetButton *_shader2Button = nullptr;

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

	void updateGUI();
	void updateMaterial();

	void initGUI();
	void deinitGUI();
};

#endif // __USERCONTROLMATERIAL_H__