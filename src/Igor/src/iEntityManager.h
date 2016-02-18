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

#ifndef __iENTITYMANAGER__
#define __iENTITYMANAGER__

#include <iDefines.h>

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class iEntityData;
    class iSystem;

    /*! entity factory
    */
    class Igor_API iEntityManager : public iaSingleton<iEntityManager>
    {

        friend class iaSingleton<iEntityManager>;
        friend class iSystem;

    public:

        /*! invallid id definition
        */
        static const uint64 INVALID_ENTITY_ID = 0;

        /*! creates an entity

        \returns entity id
        */
        uint64 createEntity();

        /*! destroys entity by id

        \param entityID the id of the entity to destroy
        */
        void destroyEntity(uint64 entityID);

        /*! checks if given entity id exists

        \param entityID entity id
        \returns true if entity id exists
        */
        bool isEntity(uint64 entityID);

        void setEntityDataMask(uint64 entityID, uint64 dataMask);
        uint64 getEntityDataMask(uint64 entityID);

        // can't be called after system was added
        void registerEntityData(iEntityData* entityData);
        // can't be called after system was added
        void unregisterEntityData(iEntityData* entityData);

        iEntityData* getEntityData(uint64 dataMask);

        void registerSystem(iSystem* system);
        void unregisterSystem(iSystem* system);

    private:

        /*! next entity id
        */
        uint64 _nextID = INVALID_ENTITY_ID + 1;

        /*! map of ids to entities
        */
        map<uint64, uint64> _entitys;

        /*! data related to entity masks and entities
        */
        map<uint64, iEntityData*> _entityData;

        /*! systems to manipulate entities data
        */
        map<uint64, iSystem*> _systems;


        /*! called per frame. calls handles of all systems registered
        */
        void onHandle();

        /*! does nothing
        */
        iEntityManager();

        /*! checks if all entity where released
        */
        ~iEntityManager();

    };

}

#endif