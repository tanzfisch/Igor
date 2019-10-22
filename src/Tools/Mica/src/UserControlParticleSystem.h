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
#include <iaGradient.h>
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
    typedef iDialogGraph* iDialogGraphPtr;
    class iDialog;
    typedef iDialog* iDialogPtr;
    class iWidgetGraph;
    class iNodeParticleSystem;
}

/*! user control to manipulate a particle system
*/
class UserControlParticleSystem : public iUserControl
{
public:

	/*! init gui
	*/
	UserControlParticleSystem();

	/*! deinit gui
	*/
	~UserControlParticleSystem();

    /*! sets the current scene node to work with

    \param id id of node to work with
    */
    void setNode(uint64 id);

    /*! \returns the current node
    */
    uint64 getNode();

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

    /*! number chooser for vorticity confinement value
    */
    iWidgetNumberChooser* _vorticityConfinementChooser = nullptr;

    /*! number chooser for vortex to particle rate
    */
    iWidgetNumberChooser* _vortexToParticleRateChooser = nullptr;

    /*! vortex minimum torque number chooser
    */
    iWidgetNumberChooser* _vortexTorqueMinChooser = nullptr;

    /*! vortex maximum torque number chooser
    */
    iWidgetNumberChooser* _vortexTorqueMaxChooser = nullptr;

    /*! vortex minimum range number chooser
    */
    iWidgetNumberChooser* _vortexRangeMinChooser = nullptr;

    /*! vortex maximum range number chooser
    */
    iWidgetNumberChooser* _vortexRangeMaxChooser = nullptr;

    /*! number chooser for vertical tiling of primary texture
    */
    iWidgetNumberChooser* _tilingVerticalChooser = nullptr;

    /*! number chooser for horizontal tiling of primary texture
    */
    iWidgetNumberChooser* _tilingHorizontalChooser = nullptr;

    /*! number chooser for vortex check range
    */
    iWidgetNumberChooser* _vortexCheckRange = nullptr;

    /*! number chooser for max particle count
    */
    iWidgetNumberChooser* _maxParticleCount = nullptr;

    /*! color gradient display
    */
    iWidgetColorGradient* _colorGradient = nullptr;

    /*! color gradient dialog
    */
    iDialogColorGradient* _colorGradientDialog = nullptr;

    /*! graph dialog. Reused for all the gradients we want to edit
    */
    iDialogGraphPtr _dialogGraph = nullptr;

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

    /*! particle emission graph
    */
    iWidgetGraph* _emissionGraph = nullptr;

    /*! text field to show the current particle count
    */
    iWidgetTextEdit* _textParticleCount = nullptr;

    /*! text field to show the current particle simulation frequency
    */
    iWidgetTextEdit* _textFrequency = nullptr;

    /*! list with all available emitters in all scenes
    */
    std::vector<uint64> _emitters;

    /*! contains user data that has to be deleted after use in the widgets
    */
    std::vector<uint32*> _userDataMaterialID;

    /*! current node
    */
    uint64 _nodeId = iNode::INVALID_NODE_ID;

    /*! prevents updating the node while we are updating the gui
    */
    bool _ignoreNodeUpdate = false;

    /*! triggered from various sources to update the node
    
    \param source the source widget of this event
    */
    void onDoUpdateNode(const iWidgetPtr source);

    /*! triggered when the color gradient editor is to open

    \param source the source widget
    */
    void onOpenColorGradientEditor(const iWidgetPtr source);

    /*! triggered every frame
    */
    void onCyclickUpdate();

    /*! triggered when color gradient editor was closed

    \param dialog the source of the event
    */
    void onCloseColorGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on start size gradient graph

    \param source the source widget
    */
    void onOpenStartSizeGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the start size gradient graph editor

    \param dialog the source of the event
    */
    void onCloseStartSizeGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on size scale gradient graph

    \param source the source widget
    */
    void onOpenScaleSizeGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the size scale gradient graph editor

    \param dialog the source of the event
    */
    void onCloseScaleSizeGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on visibility gradient graph

    \param source the source widget
    */
    void onOpenVisibilityGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the visibility gradient graph editor

    \param dialog the source of the event
    */
    void onCloseVisibilityGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on start orientation gradient graph

    \param source the source widget
    */
    void onOpenStartOrientationGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the start orientation gradient graph editor

    \param dialog the source of the event
    */
    void onCloseStartOrientationGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on orientation rate gradient graph

    \param source the source widget
    */
    void onOpenStartOrientationRateGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the orientation rate gradient graph editor

    \param dialog the source of the event
    */
    void onCloseStartOrientationRateGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on start velocity gradient graph

    \param source the source widget
    */
    void onOpenStartVelocityGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the start velocity gradient graph editor

    \param dialog the source of the event
    */
    void onCloseStartVelocityGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on start lift/weight gradient graph

    \param source the source widget
    */
    void onOpenStartLiftGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the start lift/weight gradient graph editor

    \param dialog the source of the event
    */
    void onCloseStartLiftGradientEditor(iDialogPtr dialog);

    /*! triggered when clicked on particle emission gradient graph

    \param source the source widget
    */
    void onOpenEmissionGradientEditor(const iWidgetPtr source);

    /*! triggered when closed the emission gradient graph editor

    \param dialog the source of the event
    */
    void onCloseEmissionGradientEditor(iDialogPtr dialog);
    
    /*! converts particle system gradients to GUI graphs

    \param node the particle system node
    */
    void convertGradientsToUI(iNodeParticleSystem* node);

    /*! triggered when start button was clicked

    \param source the source widget
    */
    void onStart(const iWidgetPtr source);

    /*! triggered when stop button was clicked

    \param source the source widget
    */
    void onStop(const iWidgetPtr source);
    
    /*! triggered when reset button was clicked

    \param source the source widget
    */
    void onReset(const iWidgetPtr source);

    /*! updates the gui from node data
    */
    void updateGUI();

    /*! updates the node from gui data
    */
    void updateNode();

    /*! initialize gui
    */
    void initGUI();

};

#endif