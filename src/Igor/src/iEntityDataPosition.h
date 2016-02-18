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

#ifndef __iENTITYDATAPOSITION__
#define __iENTITYDATAPOSITION__

#include <iEntityData.h>
#include <iSphere.h>
#include <iOctree.h>

#include <iaVector3.h>
using namespace IgorAux;

#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    class Igor_API iEntityDataPosition : public iEntityData
    {

    public:

        __IGOR_INLINE__ uint64 getDataMask();

        __IGOR_INLINE__ void setPosition(uint64 entityID, const iaVector3f& position);
        __IGOR_INLINE__ const iaVector3f& getPosition(uint64 entityID);

        __IGOR_INLINE__ void query(const iSphered& sphere, vector<uint64>& data);

    private:

        /*! map of entity spheres
        */
        map<uint64, iSpheref> _spheres;

        /*! octree
        */
        iOctree* _octree = nullptr;

    };

#include <iEntityDataPosition.inl>

}

#endif