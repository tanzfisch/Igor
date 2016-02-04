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

#ifndef __iENTITYFACTORY__
#define __iENTITYFACTORY__

#include <iDefines.h>

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class iEntity;

    __IGOR_FUNCTION_POINTER__(CreateEntity, __IGOR_DEFAULTCALL__, iEntity*, ());

    /*! entity factory
    */
    class Igor_API iEntityFactory : public iaSingleton<iEntityFactory>
    {

        friend class iaSingleton<iEntityFactory>;

    public:

        /*! creates an entity

        \returns pointer to new entity
        */
        iEntity* createEntity(const iaString& identifier);

        /*! destroys entity by pointer

        \param entity pointer to entity
        */
        void destroyEntity(iEntity* entity);

        /*! destroys entity by id

        \param entityID the id of the entity to destroy
        */
        void destroyEntity(uint32 entityID);

        /*! \returns shared pointer to entity by id

        \param id the id of the entity to return
        */
        iEntity* getEntity(uint32 id);

        /*! \returns list of all entities
        */
        map<uint32, iEntity*>& getEntities();

        /*! registers an entity creator

        \param identifier name of the creator
        \param functionPointer function pointer to instance creation
        */
        void registerEntityCreator(const iaString& identifier, CreateEntity functionPointer);

        /*! unregisters an entity creator

        \param identifier name of the creator
        */
        void unregisterEntityCreator(const iaString& identifier);

    private:

        /*! next entity id
        */
        static uint32 _nextID;

        /*! map of ids to shared pointer of entities
        */
        map<uint32, iEntity*> _entities;

        map<int64, CreateEntity> _entityCreators;

        int64 calcHashValue(const iaString& text);

        /*! starts entity control
        */
        iEntityFactory();

        /*! checks if all entity where released
        */
        ~iEntityFactory();

    };

}

#endif