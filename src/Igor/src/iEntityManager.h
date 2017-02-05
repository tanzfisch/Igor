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

#ifndef __iENTITYMANAGER__
#define __iENTITYMANAGER__

#include <iOctree.h>

#include <iaSingleton.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{
    
    class Igor_API iEntityManager : public iaSingleton<iEntityManager>
    {

        friend class iaSingleton<iEntityManager>;
        friend class iEntity;
        friend class iEntityLocatable;

    public:

        iEntity* getEntity(uint64 entityID);

        void killEntity(uint64 entityID);

        void getEntities(const iSphered& sphere, vector<uint64>& result);
        void getEntities(vector<uint64>& result);

        void handle();

    private:

        map<uint64, iEntity*> _entities;

        vector<uint64> _toDelete;

        iOctree* _octree = nullptr;

        void registerEntity(iEntity* entity);
        void unregisterEntity(iEntity* entity);

        iEntityManager();
        ~iEntityManager();

    };

}

#endif