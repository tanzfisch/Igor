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

#ifndef __USERCONTROLPARTICLESYSTEM__
#define __USERCONTROLPARTICLESYSTEM__

#include <iNodeModel.h>
#include <iUserControl.h>
#include <iGradient.h>
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
    class iWidgetSelectBox;
    class iUserControlFileChooser;
    class iWidgetCheckBox;
    class iWidgetColorGradient;
    class iDialogColorGradient;
    class iDialogGraph;
    class iWidgetGraph;
    class iNodeParticleSystem;
}

class UserControlParticleSystem : public iUserControl
{
public:

    /*! init gui
    */
    UserControlParticleSystem();

    /*! deinit gui
    */ 
    ~UserControlParticleSystem();

    /*! \returns root widget

    \todo remove later. but fix the widget mechanism first
    */
    iWidget* getWidget();

    /*! sets the current scene node to work with

    \param id id of node to work with
    */
    void setNode(uint32 id);

    /*! \returns the current node
    */
    uint32 getNode();

private:

    /*! root grid
    */
    iWidgetGrid* _grid = nullptr;

    /*! button to start or continue the particle system
    */
    iWidgetButton* _buttonStart = nullptr;

    /*! button to stop or pause the particle system
    */
    iWidgetButton* _buttonStop = nullptr;

    /*! button to restart the particle system
    */
    iWidgetButton* _buttonReset = nullptr;

    /*! texture chooser for primary texture
    */
    iUserControlFileChooser* _textureChooser0 = nullptr;

    /*! texture chooser for first noise texture
    */
    iUserControlFileChooser* _textureChooser1 = nullptr;

    /*! texture chooser for second noise texture
    */
    iUserControlFileChooser* _textureChooser2 = nullptr;

    /*! \deprecated currently unused
    */
    iUserControlFileChooser* _textureChooser3 = nullptr;

    /*! select box for selecting the emitter to connect the particle system to
    */
    iWidgetSelectBox* _emitterSelection = nullptr;

    /*! select box for selecting the material to use with the particle system
    */
    iWidgetSelectBox* _materialSelection = nullptr;

    /*! check box to switch on/off the particle system loop
    */
    iWidgetCheckBox* _loopCheckBox = nullptr;

    /*! check box to switch on/off the particle to velocity orientation
    */
    iWidgetCheckBox* _velocityOrientedCheckBox = nullptr;

    /*! number chooser for particle period time
    */
    iWidgetNumberChooser* _periodChooser = nullptr;

    /*! number chooser for air drag effect on particle system
    */
    iWidgetNumberChooser* _airDragChooser = nullptr;

    iWidgetNumberChooser* _vorticityConfinementChooser = nullptr;
    iWidgetNumberChooser* _vorticityAppearanceRateChooser = nullptr;

    /*! number chooser for vertical tiling of primary texture
    */
    iWidgetNumberChooser* _tilingVerticalChooser = nullptr;

    /*! number chooser for horizontal tiling of primary texture
    */
    iWidgetNumberChooser* _tilingHorizontalChooser = nullptr;

    /*! color gradient display
    */
    iWidgetColorGradient* _colorGradient = nullptr;

    /*! color gradient dialog
    */
    iDialogColorGradient* _colorGradientDialog = nullptr;

    /*! graph dialog. Reused for all the gradients we want to edit
    */
    iDialogGraph* _dialogGraph = nullptr;

    /*! start size graph
    */
    iWidgetGraph* _startSizeGraph = nullptr;

    /*! scale over time size scale graph
    */
    iWidgetGraph* _scaleSizeGraph = nullptr;

    /*! visibility graph
    */
    iWidgetGraph* _visibilityGraph = nullptr;

    /*! orientation graph
    */
    iWidgetGraph* _orientationGraph = nullptr;

    /*! orientation rate graph
    */
    iWidgetGraph* _orientationRateGraph = nullptr;

    /*! velocity graph
    */
    iWidgetGraph* _startVelocityGraph = nullptr;

    /*! start lift / weight graph
    */
    iWidgetGraph* _startLiftGraph = nullptr;

    /*! list with all widgets for easy cleanup
    */
    vector<iWidget*> _allWidgets;

    /*! list with all available emitters in all scenes
    */
    vector<iNode*> _emitters;

    /*! contains user data that has to be deleted after use in the widgets
    */
    vector<uint32*> _userDataMaterialID;

    /*! current node
    */
    uint32 _nodeId = iNode::INVALID_NODE_ID;

    /*! prevents updating the node while we are updating the gui
    */
    bool _ignoreNodeUpdate = false;

    /*! triggered from various sources to update the node
    
    \param source the source widget of this event
    */
    void onDoUpdateNode(iWidget* source);

    /*! triggered when the color gradient editor is to open

    \param source the source widget
    */
    void onOpenColorGradientEditor(iWidget* source);

    /*! triggered when color gradient editor was closed

    \param ok if true the ok button was pressed to close
    \param gradient the resulting color gradient
    */
    void onCloseColorGradientEditor(bool ok, const iGradientColor4f& gradient);

    void onOpenStartSizeGradientEditor(iWidget* source);
    void onCloseStartSizeGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenScaleSizeGradientEditor(iWidget* source);
    void onCloseScaleSizeGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenVisibilityGradientEditor(iWidget* source);
    void onCloseVisibilityGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenStartOrientationGradientEditor(iWidget* source);
    void onCloseStartOrientationGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenStartOrientationRateGradientEditor(iWidget* source);
    void onCloseStartOrientationRateGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenStartVelocityGradientEditor(iWidget* source);
    void onCloseStartVelocityGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);

    void onOpenStartLiftGradientEditor(iWidget* source);
    void onCloseStartLiftGradientEditor(bool ok, const vector<vector<iaVector2f>>& graphs);
    
    void convertGradientsToUI(iNodeParticleSystem* node);

    void onStart(iWidget* source);
    void onStop(iWidget* source);
    void onReset(iWidget* source);

    void updateGUI();
    void updateNode();

    void initGUI();
    void deinitGUI();

};

#endif