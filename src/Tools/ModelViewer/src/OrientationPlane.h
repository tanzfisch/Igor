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

#ifndef __ORIENTATIONPLANE__
#define __ORIENTATIONPLANE__

#include <iMeshBuffers.h>
#include <iMaterial.h>
#include <iNode.h>
using namespace Igor;

namespace Igor
{
    class iScene;
    class iMesh;
    class iTargetMaterial;
}

/*! display a grid plane for better orientation within the scene
*/
class OrientationPlane
{

public:

    /*! initialisation
    */
    OrientationPlane(iScene* scene);

    /*! cleanup 
    */
    ~OrientationPlane();

    /*! sets grid's visibility

    \param visible the visibility
    */
    void setVisible(bool visible);

    /*! \returns true if grid is visible
    */
    bool isVisible() const;

private:

    /*! scene for displaying the grid
    */
    iScene* _scene = nullptr;

    /*! grid material
    */
    uint64 _material = iMaterial::INVALID_MATERIAL_ID;

    /*! grid target material
    */
    iTargetMaterial* _targetMaterial = nullptr;

    /*! grid's transform node
    */
    uint64 _transformNodeID = iNode::INVALID_NODE_ID;

    /*! visibility flag
    */
    bool _visible = true;

    /*! crates a grid mesh
    */
    shared_ptr<iMesh> createGridMesh();

    /*! initialisation
    */
    void init();

    /*! deinitialisation
    */
    void deinit();

};


#endif



