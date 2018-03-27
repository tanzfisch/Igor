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

#include <iMeshBuffers.h>
#include <iMaterial.h>
#include <iNode.h>
using namespace Igor;

#include <memory>
using namespace std;

namespace Igor
{
    class iTargetMaterial;
    class iNodeTransform;
    class iNodeSwitch;
}

enum class ModifierMode
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

    Manipulator(iNode* parent);

    void setVisible(bool visible);
    bool isVisible() const;

    void setMatrix(const iaMatrixd& matrix);
    void getMatrix(iaMatrixd& matrix) const;

    void updateCamMatrix(const iaMatrixd& camMatrix);

    void init();
    
    void deinit();
    
    void setModifierMode(ModifierMode modifierMode);
    ModifierMode getModifierMode() const;

    void setSelected(uint64 nodeID);
    uint64 getSelected() const;

    void transform(float64 dx, float64 dy, iaMatrixd& matrix);

private:

    uint64 _selectedNodeID = iNode::INVALID_NODE_ID;
    iNode* _parent = nullptr;
    bool _visible = false;

    iNodeTransform* _rootTransform = nullptr;

    iNodeSwitch* _switchNode = nullptr;

    iNode* _locatorModifier = nullptr;
    iNode* _translateModifier = nullptr;
    iNode* _scaleModifier = nullptr;
    iNode* _roateModifier = nullptr;
    iNodeTransform* _rotateBillboardTransform = nullptr;

    ModifierMode _modifierMode = ModifierMode::Locator;

    uint64 _translateXNodeID = iNode::INVALID_NODE_ID;
    uint64 _translateYNodeID = iNode::INVALID_NODE_ID;
    uint64 _translateZNodeID = iNode::INVALID_NODE_ID;

    uint64 _scaleXNodeID = iNode::INVALID_NODE_ID;
    uint64 _scaleYNodeID = iNode::INVALID_NODE_ID;
    uint64 _scaleZNodeID = iNode::INVALID_NODE_ID;
    uint64 _scaleAllNodeID = iNode::INVALID_NODE_ID;

    iTargetMaterial* _red = nullptr;
    iTargetMaterial* _green = nullptr;
    iTargetMaterial* _blue = nullptr;
    iTargetMaterial*_cyan = nullptr;

    iaMatrixd _modifierMatrix;

    uint64 _material = iMaterial::INVALID_MATERIAL_ID;

    shared_ptr<iMesh> createTranslateMesh();
    shared_ptr<iMesh> createLocatorMesh();
    shared_ptr<iMesh> createScaleMesh();
    shared_ptr<iMesh> createRingMesh();
    shared_ptr<iMesh> create2DRingMesh();

    void createLocatorModifier(shared_ptr<iMesh> &locatorMesh);
    void createTranslateModifier(shared_ptr<iMesh> &translateMesh);
    void createScaleModifier(shared_ptr<iMesh> &scaleMesh);
    void createRotateModifier(shared_ptr<iMesh> &cylinderMesh, shared_ptr<iMesh> &ringMesh, shared_ptr<iMesh> &ringMesh2D);

};


#endif



