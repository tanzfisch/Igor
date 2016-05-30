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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __USERCONTROLMATERIAL__
#define __USERCONTROLMATERIAL__

#include <iNodeMesh.h>
#include <iaColor4.h>
using namespace IgorAux;

#include <iWidgetUserControl.h>
#include <iUserControlColorChooser.h>
#include <iFileDialog.h>
using namespace Igor;

namespace Igor
{
	class iWidgetGrid;
	class iWidgetTextEdit;
	class iWidgetLabel;
	class iWidgetButton;
	class iUserControlColorChooser;
	class iWidgetSlider;
	class iWidgetNumberChooser;
	class iWidgetCheckBox;
	class iWidgetSelectBox;
}

iaEVENT(MaterialNameChangedEvent, MaterialNameChangedDelegate, void, (), ());

class UserControlMaterial : public iWidgetUserControl
{
public:

	UserControlMaterial();
	~UserControlMaterial();

	iWidget* getWidget();

	void setMaterial(uint32 id);
	uint32 getMaterial();

	void registerNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);
	void unregisterNameChangeDelegate(MaterialNameChangedDelegate nameChangedDelegate);

private:

	MaterialNameChangedEvent _materialNameChangedEvent;

	iWidgetGrid* _grid = nullptr;
	iWidgetTextEdit* _textName = nullptr;
	iWidgetNumberChooser* _renderingOrder = nullptr;

	iWidgetCheckBox* _checkBoxCullFace = nullptr;
	iWidgetCheckBox* _checkBoxDepthTest = nullptr;
	iWidgetCheckBox* _checkBoxDepthMask = nullptr;
	iWidgetCheckBox* _checkBoxBlend = nullptr;
	iWidgetCheckBox* _checkBoxTexture2D0 = nullptr;
	iWidgetCheckBox* _checkBoxTexture2D1 = nullptr;
	iWidgetCheckBox* _checkBoxTexture2D2 = nullptr;
	iWidgetCheckBox* _checkBoxTexture2D3 = nullptr;
	iWidgetCheckBox* _checkBoxWireframe = nullptr;
	iWidgetCheckBox* _checkBoxInstanced = nullptr;
	iWidgetSelectBox* _selectBoxDepthFunc = nullptr;
	iWidgetSelectBox* _selectBoxCullFaceFunc = nullptr;
	iWidgetSelectBox* _selectBoxBlendFuncSource = nullptr;
	iWidgetSelectBox* _selectBoxBlendFuncDestination = nullptr;
	iWidgetSelectBox* _selectBoxInstancedFunc = nullptr;

    iWidgetTextEdit* _textShader0 = nullptr;
    iWidgetTextEdit* _textShader1 = nullptr;
    iWidgetTextEdit* _textShader2 = nullptr;

    iWidgetButton* _shader0Button = nullptr;
    iWidgetButton* _shader1Button = nullptr;
    iWidgetButton* _shader2Button = nullptr;

    iWidgetButton* _shaderReload = nullptr;

	vector<iWidget*> _allWidgets;

	uint32 _materialID = 0;

    uint32 _loadShaderNumber = 0;

    iFileDialog* _fileDialog = nullptr;

	bool _ignoreMaterialUpdate = false;

    void onShader0Button(iWidget* source);
    void onShader1Button(iWidget* source);
    void onShader2Button(iWidget* source);
    void onReloadShader(iWidget* source);

	void onTextChangedName(iWidget* source);
	void onDoUpdateMaterial(iWidget* source);

    void onFileLoadDialogClosed(iFileDialogReturnValue fileDialogReturnValue);

    void reloadShader(iMaterial* material);

	void updateGUI();
	void updateMaterial();

	void initGUI();
	void deinitGUI();

};

#endif