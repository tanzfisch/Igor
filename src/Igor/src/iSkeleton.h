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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iSKELETON__
#define __iSKELETON__

#include <iJoint.h>
#include <iBone.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    /*! skeleton that contains joints and bones
    */
    class Igor_API iSkeleton
    {

    public:

        /*! does nothing
        */
        iSkeleton() = default;

        /*! does nothing
        */
        virtual ~iSkeleton() = default;

        /*! \returns the ID of the root joint
        */
        uint64 getRootJoint() const;

        /*! adds a bone at current position

        \param matrix the orientation and offset of the bone
        \param lenght the lenght of the bone
        */
        void addBone(const iaMatrixf& matrix, float64 lenght);

        /*! pushes current position in skeleton
        */
        void push();

        /*! pops back to last pushed position in skeleton
        */
        void pop();

        /*! clears all data
        */
        void clear();

        /*! \returns last created bone
        */
        uint64 getLastBone() const;

    private:

        /*! internal bone stack. handles push and pop
        */
        vector<uint64> _boneStack;

        /*! list of all bones in use
        */
        vector<uint64> _bones;

        /*! list of all joints in use
        */
        vector<uint64> _joints;

        /*! root joint id
        */
        uint64 _rootJoint = iJoint::INVALID_JOINT_ID;

        /*! current joint
        */
        uint64 _currentJoint = iJoint::INVALID_JOINT_ID;

        /*! last created bone
        */
        uint64 _lastBone = iBone::INVALID_BONE_ID;

    };

}


#endif