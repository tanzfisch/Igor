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
using namespace Igor;

#include <memory>
using namespace std;

namespace Igor
{
    class iTargetMaterial;
    class iNodeTransform;
    class iNodeSwitch;
    class iNodeCamera;
}

enum class ManipulatorMode
{
    Locator,
    Translate,
    Scale,
    Rotate
};

/*! ui element to modify position, orientation and scale of objects in the scene
*/
class Manipulator
{

public:

    Manipulator(iWindow* window);

    void setNodeID(uint64 nodeID);
    uint64 getNodeID() const;

    void setVisible(bool visible);
    bool isVisible() const;

    void setCamCOI(const iaMatrixd& matrix);
    void setCamHeading(const iaMatrixd& matrix);
    void setCamPitch(const iaMatrixd& matrix);
    void setCamTranslate(const iaMatrixd& matrix);

    void update();

    void init();

    void deinit();

    bool isSelected() const;

    void setManipulatorMode(ManipulatorMode ManipulatorMode);
    ManipulatorMode getManipulatorMode() const;

    void onMouseMoved(const iaVector2i& from, const iaVector2i& to, iWindow* window);
    void onMouseWheel(int32 d);
    void onMouseKeyDown(iKeyCode key);
    void onMouseKeyUp(iKeyCode key);

private:

    iNodeTransform* _cameraCOIUI = nullptr;
    iNodeTransform* _cameraHeadingUI = nullptr;
    iNodeTransform* _cameraPitchUI = nullptr;
    iNodeTransform* _cameraTranslationUI = nullptr;
    iNodeTransform* _transformModelUI = nullptr;
    iNodeCamera* _cameraUI = nullptr;

    uint64 _materialCelShading;

    iWindow* _window = nullptr;
    iView _view;
    iScene* _scene = nullptr;

    uint64 _selectedLocatorNodeID = iNode::INVALID_NODE_ID;
    uint64 _selectedNodeID = iNode::INVALID_NODE_ID;
    iNode* _parent = nullptr;
    bool _visible = false;

    vector<uint64> _locatorIDs;
    vector<uint64> _translateIDs;
    vector<uint64> _scaleIDs;
    vector<uint64> _rotateIDs;

    iNodeTransform* _rootTransform = nullptr;

    iNodeSwitch* _switchNode = nullptr;

    iNode* _locatorModifier = nullptr;
    iNode* _translateModifier = nullptr;
    iNode* _scaleModifier = nullptr;
    iNode* _roateModifier = nullptr;
    iNodeTransform* _rotateBillboardTransform = nullptr;

    ManipulatorMode _manipulatorMode = ManipulatorMode::Locator;

    iTargetMaterial* _red = nullptr;
    iTargetMaterial* _green = nullptr;
    iTargetMaterial* _blue = nullptr;
    iTargetMaterial*_cyan = nullptr;

    uint64 _material = iMaterial::INVALID_MATERIAL_ID;

    shared_ptr<iMesh> createTranslateMesh();
    shared_ptr<iMesh> createLocatorMesh();
    shared_ptr<iMesh> createScaleMesh();
    shared_ptr<iMesh> createRingMesh();
    shared_ptr<iMesh> create2DRingMesh();

    void translate(const iaVector3d& vec, iaMatrixd& matrix);
    void scale(const iaVector3d& vec, iaMatrixd& matrix);
    void rotate(int32 x1, int32 y1, int32 x2, int32 y2, iaMatrixd& matrix);

    void render();

    void highlightSelected();

    void createLocatorModifier(shared_ptr<iMesh> &locatorMesh);
    void createTranslateModifier(shared_ptr<iMesh> &translateMesh);
    void createScaleModifier(shared_ptr<iMesh> &scaleMesh);
    void createRotateModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &ringMesh, shared_ptr<iMesh> &ringMesh2D);

};


#endif



