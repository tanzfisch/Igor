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

#include <iEntity.h>

#include <vector>
#include <map>
using namespace std;

namespace Igor
{

    class iSystem;
    class iComponent;

    class Igor_API iEntityManager
    {

    public:

        uint64 createEntity();

        /*! destroyes and entity by id

        \param entityID the entity's ID
        */
        void destroyEntity(uint64 entityID);

        void registerSystem(iSystem* system);

        void addComponent(uint64 entityID, uint64 componentTypeID);
        void removeComponent(uint64 entityID, uint64 componentTypeID);

        /*! registers a component type
        
        \param componentTypeID unique component type id
        \param componentSize component size in bytes
        \param componentCreator the component instance creating funtion
        */
        void registerComponent(uint64 componentTypeID, uint32 componentSize);

        void handle();

        iEntityManager() = default;
        ~iEntityManager();

    private:

        map<uint64, iEntity*> _entities;

        map<uint64, pair<uint32, void*>> _components;

        vector<iSystem*> _systems;

        void* createComponentBuffer(uint32 componentSize);
        void* getComponentBuffer(uint64 componentTypeID, uint32 componentSize);

    };

}

#endif