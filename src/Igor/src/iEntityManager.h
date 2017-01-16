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

#ifndef __iENTITYFACTORY__
#define __iENTITYFACTORY__

#include <iDefines.h>
#include <iSphere.h>

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <map>
#include <mutex>
using namespace std;

namespace Igor
{

    class iEntity;
    class iOctree;
    class iScene;

    __IGOR_FUNCTION_POINTER__(iCreateEntityInstance, __IGOR_DEFAULTCALL__, iEntity*, ());

    /*! entity factory
    */
    class Igor_API iEntityManager : public iaSingleton<iEntityManager>
    {

        friend class iaSingleton<iEntityManager>;

    public:

        /*! creates and returns an entity of defined type

        \param entityType entity type
        \returns new entity
        */
        iEntity* createEntity(const iaString& entityType);

        /*! \returns entity by ID. nullptr if entity does not exist

        \param id the entity id
        */
        iEntity* getEntity(uint64 id);

        /*! destroys entity by id

        \param id the entity id to destroy
        */
        void destroyEntity(uint64 id);

        /*! registers an entity type

        \param entityType entity type identifier
        \param functionPointer function pointer to create an instance of that entity type
        */
        void registerEntityType(const iaString& entityType, iCreateEntityInstance functionPointer);

        /*! unregisters enetity type

        \param entityType entity type identifier
        */
        void unregisterEntityType(const iaString& entityType);

        /*! returns entitties that are located in specified volume

        \param sphere the defined volume
        \param[out] result the resulting list of entity IDs
        */
        void getEntities(const iSphered& sphere, vector<uint64>& result);

        /*! iteration handle
        */
        void handle();

    private:

        /*! mutex to protect entity list
        */
        mutex _mutexEntities;

        /*! mutex to protect the octree
        */
        mutex _mutexOctree;

        /*! octree for positioned entities
        */
        iOctree* _octree = nullptr;

        /*! list of entity type creators
        */
        map<uint64, iCreateEntityInstance> _types;

        /*! list of entities
        */
        map<uint64, iEntity*> _entities;

        /*! calculates a hash from entity type string. this is an ugly workaround.

        \todo fix later
        */
        uint64 calcHashValue(const iaString& text);
        
        /*! initializes members
        */
        iEntityManager();

        /*! cleanup
        */
        ~iEntityManager();

    };

}

#endif