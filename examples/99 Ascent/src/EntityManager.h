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

#ifndef __ENTITYMANAGER__
#define __ENTITYMANAGER__

#include <iDefines.h>
#include <iSphere.h>
using namespace Igor;

#include <iaSingleton.h>
#include <iaString.h>
#include <iOctree.h>
using namespace IgorAux;

#include <map>
#include <mutex>
using namespace std;

class Entity;

__IGOR_FUNCTION_POINTER__(iCreateEntityInstance, __IGOR_DEFAULTCALL__, Entity*, ());

/*! entity factory
*/
class EntityManager : public iaSingleton<EntityManager>
{

    friend class iaSingleton<EntityManager>;

public:

    /*! creates and returns an entity of defined type

    \param entityType entity type name
    \returns new entity
    */
    Entity* createEntity(const iaString& entityTypeName);

    /*! creates and returns an entity of defined type

    \param entityTypeID entity type id
    \returns new entity
    */
    Entity* createEntity(uint64 entityTypeID);

    /*! \returns entity by ID. nullptr if entity does not exist

    \param id the entity id
    */
    Entity* getEntity(uint64 id);

    /*! destroys entity by id

    \param id the entity id to destroy
    */
    void destroyEntity(uint64 id);

    /*! registers an entity type

    \param entityType entity type identifier
    \param functionPointer function pointer to create an instance of that entity type
    */
    uint64 registerEntityType(const iaString& entityTypeName, iCreateEntityInstance functionPointer);

    /*! unregisters enetity type

    \param entityType entity type name
    */
    void unregisterEntityType(const iaString& entityTypeName);

    /*! unregister entity type by id

    \param entityTypeID entity type id
    */
    void unregisterEntityType(uint64 entityTypeID);

    /*! returns entitties that are located in specified volume

    \param sphere the defined volume
    \param[out] result the resulting list of entity IDs
    */
    void getEntities(const iSphered& sphere, vector<uint64>& result);

    /*! returns all entities

    \param[out] result list of all entities
    */
    void getEntities(vector<uint64>& result);

    /*! starts / continues entity manager to run
    */
    void start();

    /*! stops / pauses entity manager
    */
    void stop();

    /*! \returns true if entity manager is running
    */
    bool isRunning();

private:

    /*! flag if entity manager is running. means that it's handle is registered with the application handle event
    */
    bool _isRunning = false;

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
    map<uint64, Entity*> _entities;

    /*! iteration handle
    */
    void handle();

    /*! calculates a hash from entity type string. this is an ugly workaround.

    \todo fix later
    */
    uint64 calcHashValue(const iaString& text);

    /*! initializes members
    */
    EntityManager();

    /*! cleanup
    */
    ~EntityManager();

};

#endif