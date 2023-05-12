//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __PLANTMESHGENERATOR__
#define __PLANTMESHGENERATOR__

#include <iaux/math/iaVector3.h>
#include <iaux/math/iaRandomNumberGenerator.h>
using namespace iaux;

#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/model/loader/iModelDataIO.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/data/iSkeleton.h>
using namespace igor;

namespace igor
{
    class iLSystem;
    class iMeshBuilder;
    class iTargetMaterial;
}

/*! struct to store generation information
*/
struct PlantInformation
{
    /*! pointer to L-System to use
    */
    iLSystem* _lSystem = nullptr;

    /*! starting sentence for L-System
    */
    wchar_t _axiom[10];

    /*! iteration count
    */
    uint32 _iterations = 0;

    /*! random seed
    */
    uint64 _seed = 0;

    /*! segemnt length
    */
    float64 _segmentLenght = 0;

    /*! segment rotation angle
    */
    float64 _segmentAngle = 0;

    /*! trunk color
    */
    iaColor3f _trunkColor;

    /*! branch color
    */
    iaColor3f _branchColor;

    /*! bud color
    */
    iaColor3f _budColor;

    /*! flower color
    */
    iaColor3f _flowerColor;

    /*! leaf color
    */
    iaColor3f _leafColor;

    /*! material
    */
    iMaterialPtr _material;
};

/*! plant mesh generator
*/
class PlantMeshGenerator : public iModelDataIO
{

    /*! type of bone / section
    */
    enum SectionType
    {
        Trunk,
        Branch,
        Sproud,
        Leaf,
        Bud,
        Flower
    };

public:

    /*! generates terrain tiles 

    !!! ATTENTION consumes and deletes "parameter"

    \param sectionname name of tile section
    \return parameter tile parameters
    */
    iNodePtr importData(const iaString& sectionName, iModelDataInputParameter* parameter);

    /*! initialize members
    */
    PlantMeshGenerator();

    /*! does nothing
    */
    virtual ~PlantMeshGenerator() = default;

    /*! creates an instance of this class

    \returns new instance
    */
    static iModelDataIO* createInstance();

private:

    /*! segment length
    */
    float64 _segmentLength = 0;

    /*! segment angle
    */
    float64 _segmentAngle = 0;

    /*! mesh builder for trunk segments
    */
    iMeshBuilder _meshBuilderTrunk;

    /*! mesh builder for flowes
    */
    iMeshBuilder _meshBuilderFlowers;

    /*! mesh builder for buds
    */
    iMeshBuilder _meshBuilderBuds;

    /*! mesh builder for leafs
    */
    iMeshBuilder _meshBuilderLeaves;

    /*! skeleton
    */
    iSkeleton _skeleton;

    /*! model matrix
    */  
    iaMatrixf _modelMatrix;

    iaRandomNumberGenerator _rand;

    /*! generate mesh

    \param joint current joint to generate the mesh for
    */
    void generateMesh(iJoint* joint);

    /*! generate mesh

    \param sectionType type of current section to generate mesh for
    \param dir direction and length of current section
    */
    void generateMesh(SectionType sectionType, const iaVector3f& dir);

    /*! generates trunk mesh

    \param dir direction and length of current section
    */
    void generateTrunk(const iaVector3f& dir);

    /*! generates flower mesh

    \param dir direction and length of current section
    */
    void generateFlower(const iaVector3f& dir);

    /*! generates bud mesh

    \param dir direction and length of current section
    */
    void generateBud(const iaVector3f& dir);

    /*! generates leaf mesh

    \param dir direction and length of current section
    */
    void generateLeaf(const iaVector3f& dir);

    /*! generate skeleton for specified sentence

    \param sentence the L-System sentence
    */
    void generateSkeleton(const iaString& sentence);

};

#endif
