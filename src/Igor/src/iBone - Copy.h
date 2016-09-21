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

#ifndef __iJOINT__
#define __iJOINT__

#include <iDefines.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    /*! joining bones togehter
    */
    class Igor_API iJoint
    {

        friend class iBoneFactory;

    public:

        /*! sets base bone of this joint

        \param boneID the bones id
        */
        void setBaseBone(uint64 boneID);

        /*! \returns the base bone's ID
        */
        uint64 getBaseBone() const;

        /*! adds cild bone to joint
        */
        void addChildBone(uint64 boneID);

        /*! removes child bone from joint
        */
        void removeChildBone(uint64 boneID);

        /*! \returns reference of child bones list
        */
        vector<uint64> getChildren() const;

    private:

        /*! id of base bone
        */
        uint64 _baseBoneID;

        /*! ids of bones connected to the base bone in this joint
        */
        vector<uint64> _childBones;

        /*! init id
        */
        iJoint();

        /*! does nothing
        */
        virtual ~iJoint() = default;

    };

}


#endif