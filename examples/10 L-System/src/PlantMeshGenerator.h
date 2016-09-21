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
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __PLANTMESHGENERATOR__
#define __PLANTMESHGENERATOR__

#include <iaVector3.h>
using namespace IgorAux;

#include <iModelDataIO.h>
#include <iSkeleton.h>
using namespace Igor;

namespace Igor
{
    class iLSystem;
    class iMeshBuilder;
    class iTargetMaterial;
}

struct PlantInformation
{
    iLSystem* _lSystem = nullptr;
    iaString _axiom;
    uint32 _iterations = 0;
    uint64 _seed = 0;
    float64 _segmentLenght = 0;
    float64 _segmentAngle = 0;
};

class PlantMeshGenerator : public iModelDataIO
{

public:

    /*! generates terrain tiles 

    !!! ATTENTION consumes and deletes "parameter"

    \param sectionname name of tile section
    \return parameter tile parameters
    */
    iNode* importData(const iaString& sectionName, iModelDataInputParameter* parameter);

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

    float64 _segmentLength = 0;
    float64 _segmentAngle = 0;

    iSkeleton _skeleton;

    void generateMesh(iJoint* joint);
    void generateSkeleton(const iaString& sentence);

};

#endif
