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

#ifndef __iSYSTEMPOSITION__
#define __iSYSTEMPOSITION__

#include <iSystem.h>
#include <iSphere.h>

#include <iaVector3.h>
using namespace IgorAux;

#include <map>
#include <vector>
using namespace std;

namespace Igor
{

    class iOctree;

    class Igor_API iSystemPosition : public iSystem
    {

    public:

        virtual void registerEntity(uint64 entityID);
        virtual void unregisterEntity(uint64 entityID);
        virtual bool hasEntity(uint64 entityID);

        void setPosition(uint64 entityID, const iaVector3f& position);
        const iaVector3f& getPosition(uint64 entityID);

        void query(const iSphered& sphere, vector<uint64>& data);

        iSystemPosition();
        virtual ~iSystemPosition();

    private:

        /*! map of entity spheres
        */
        map<uint64, iSpheref> _spheres;

        /*! octree
        */
        iOctree* _octree = nullptr;


    };

}

#endif