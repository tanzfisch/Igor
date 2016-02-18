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

#ifndef __iENTITYDATAPHYSICS__
#define __iENTITYDATAPHYSICS__

#include <iEntityData.h>

#include <iaVector3.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class Igor_API iEntityDataPhysics : public iEntityData
    {

    public:

        __IGOR_INLINE__ uint64 getDataMask();

        __IGOR_INLINE__ void setVelocity(uint64 entityID, const iaVector3f& velocity);
        __IGOR_INLINE__ const iaVector3f& getVelocity(uint64 entityID);

        __IGOR_INLINE__ void setMass(uint64 entityID, float32 mass);
        __IGOR_INLINE__ float32 getMass(uint64 entityID);

        __IGOR_INLINE__ void setBody(uint64 entityID, uint64 bodyID);
        __IGOR_INLINE__ uint64 getBody(uint64 entityID);

    private:

        /*! map of entity transform nodes
        */
        map<uint64, iaVector3f> _velocities;

        map<uint64, float32> _masses;

        map<uint64, uint64> _bodies;


    };

#include <iEntityDataPhysics.inl>

}

#endif