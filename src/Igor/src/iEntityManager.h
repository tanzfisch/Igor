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
    class iComponentBase;

    class Igor_API iEntityManager
    {

    public:

        uint64 createEntity();

        /*! destroyes and entity by id

        \param entityID the entity's ID
        */
        void destroyEntity(uint64 entityID);

        void registerSystem(iSystem* system, const vector<iComponentBase*>& dependencies);
        void registerSystem(iSystem* system, const vector<uint64>& dependencies);

        void linkComponent(uint64 entityID, uint64 componentID);
        void unlinkComponent(uint64 entityID, uint64 componentID);
        void linkComponent(uint64 entityID, iComponentBase* component);
        void unlinkComponent(uint64 entityID, iComponentBase* component);

        iComponentBase* getComponent(uint64 componentID);

        /*! registers a component type
        
        \param component the component to register
        */
        void registerComponent(iComponentBase* component);

        void handle();

        iEntityManager() = default;
        ~iEntityManager();

    private:

        map<uint64, iEntity*> _entities;
        map<uint64, vector<uint64>> _entityComponents;

        map<uint64, iComponentBase*> _components;

        map<uint64, iSystem*> _systems;

        void findMatch(uint64 entityID);
        void clearMatch(uint64 entityID);

    };

}

#endif