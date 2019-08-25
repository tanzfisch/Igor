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

#ifndef __MODIFIER__
#define __MODIFIER__

#include <iMouse.h>
#include <iMeshBuffers.h>
#include <iMaterial.h>
#include <iWindow.h>
#include <iNode.h>
#include <iView.h>
#include <iScene.h>
#include <iMesh.h>
using namespace Igor;

#include <memory>


namespace Igor
{
    class iTargetMaterial;
    class iNodeTransform;
    class iNodeSwitch;
    class iNodeCamera;
}

/*! manipulator modes
*/
enum class ManipulatorMode
{
    None,
    Translate,
    Scale,
    Rotate
};

/*! 3d ui element to modify position, orientation and scale of objects in the scene
*/
class Manipulator
{

public:

    /*! initialize manipulator

    \param window the window this manipulator is displayed with
    */
    Manipulator(iWindow* window, iView* view, iScene* scene);

    /*! cleanup
    */
    ~Manipulator();

    /*! sets the node to control by ID

    \param nodeID id of node to control
    */
    void setNodeID(uint64 nodeID);

    /*! \returns id of controled node
    */
    uint64 getNodeID() const;

    /*! sets manipulator visible
    */
    void setVisible(bool visible);

    /*! \returns true if manipulator is visible
    */
    bool isVisible() const;

    /*! sets camera center of interest

    \param matrix center of interest transform in world space
    */
    void setCamCOI(const iaMatrixd& matrix);

    /*! sets camera heading matrix

    \param matrix should contain only heading of camera
    */
    void setCamHeading(const iaMatrixd& matrix);

    /*! sets camera pitch matrix

    \param matrix should contain only pitch of camera
    */
    void setCamPitch(const iaMatrixd& matrix);

    /*! sets camera translation matrix

    \param matrix contains distance to cio in Z axis
    */
    void setCamTranslate(const iaMatrixd& matrix);

    /*! \returns true if manipulator is selected
    */
    bool isSelected() const;

    /*! sets the mode of the manipulator

    \param manipulatorMode the manipulator mode
    */
    void setManipulatorMode(ManipulatorMode manipulatorMode);

    /*! \returns the current manipulator mode
    */
    ManipulatorMode getManipulatorMode() const;

    // ugly interfaces
    void onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window);
    void onMouseWheel(int32 d);
    void onMouseKeyDown(iKeyCode key);
    void onMouseKeyUp(iKeyCode key);

private:

    iNodeTransform* _cameraCOIUI = nullptr;
    iNodeTransform* _cameraHeadingUI = nullptr;
    iNodeTransform* _cameraPitchUI = nullptr;
    iNodeTransform* _cameraTranslationUI = nullptr;
    iNodeCamera* _cameraUI = nullptr;

    uint64 _materialCelShading;

    iWindow* _window = nullptr;
    iView* _view = nullptr;
    iScene* _scene = nullptr;

    uint64 _selectedManipulatorNodeID = iNode::INVALID_NODE_ID;
    uint64 _selectedNodeID = iNode::INVALID_NODE_ID;
    iNodePtr _parent = nullptr;
    bool _visible = false;

    std::vector<uint64> _locatorIDs;
    std::vector<uint64> _translateIDs;
    std::vector<uint64> _scaleIDs;
    std::vector<uint64> _rotateIDs;

    iNodeTransform* _rootTransform = nullptr;

    iNodeSwitch* _switchNode = nullptr;

    iNodePtr _transformRepresentation = nullptr;
    iNodePtr _translateModifier = nullptr;
    iNodePtr _scaleModifier = nullptr;
    iNodePtr _roateModifier = nullptr;
    iNodeTransform* _rotateBillboardTransform = nullptr;

    ManipulatorMode _manipulatorMode = ManipulatorMode::None;

    iTargetMaterial* _red = nullptr;
    iTargetMaterial* _green = nullptr;
    iTargetMaterial* _blue = nullptr;
    iTargetMaterial*_cyan = nullptr;

    uint64 _material = iMaterial::INVALID_MATERIAL_ID;

    iMeshPtr createTranslateMesh();
    iMeshPtr createScaleMesh();
	iMeshPtr createCube();
    iMeshPtr createRingMesh();
    iMeshPtr create2DRingMesh();
	iMeshPtr createCylinder();

    /*! update internal structure
    */
    void update();

    /*! initialisation
    */
    void init();

    /*! clean up
    */
    void deinit();

    

    void translate(const iaVector3d& vec, iaMatrixd& matrix);
    void scale(const iaVector3d& vec, iaMatrixd& matrix);
    void rotate(const iaVector2d& from, const iaVector2d& to, iaMatrixd& matrix);

    void render();

    void highlightSelected();

    void createTranslateModifier(iMeshPtr &translateMesh);
    void createScaleModifier(iMeshPtr &scaleMesh);
    void createRotateModifier(iMeshPtr &ringMesh, iMeshPtr &ringMesh2D, iMeshPtr& cylinder);
	void createTransformRepresentation(iMeshPtr& cylinder);

};


#endif



