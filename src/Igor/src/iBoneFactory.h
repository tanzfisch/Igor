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

#ifndef __iBONEFACTORY__
#define __iBONEFACTORY__

#include <iDefines.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class iBone;
    class iJoint;

    class Igor_API iBoneFactory : public iaSingleton<iBoneFactory>
    {

        friend class iaSingleton<iBoneFactory>;

    public:

        /*! creates a bone and returns the bone's id

        \returns the bone id
        */
        uint64 createBone();

        /*! destroyes bone by id

        \param boneID the bones id
        */
        void destroyBone(uint64 boneID);

        /*! \returns bone by id (nullptr if not found)

        \param boneID the bone's id
        */
        iBone* getBone(uint64 boneID);

        /*! creates a a joint and returns the joint's id

        \returns the joint id
        */
        uint64 createJoint();

        /*! destroyes joint by id

        \param jointID the joint's id
        */
        void destroyJoint(uint64 jointID);

        /*! \returns joint by id (nullptr if not found)

        \param jointID the joint's id
        */
        iJoint* getJoint(uint64 jointID);

    private:

        /*! map of all bones
        */
        map<uint64, iBone*> _bones;

        /*! map of all joints
        */
        map<uint64, iJoint*> _joints;

        /*! init id
        */
        iBoneFactory();

        /*! does nothing
        */
        virtual ~iBoneFactory() = default;

    };

}


#endif