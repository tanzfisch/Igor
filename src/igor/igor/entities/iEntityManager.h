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
// (c) Copyright 2012-2020 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_ENTITYMANAGER_H__
#define __IGOR_ENTITYMANAGER_H__

#include <igor/data/octree/iOctree.h>
#include <igor/resources/module/iModule.h>

#include <map>

namespace igor
{

    class iEntity_Old;

    /*! entities manager (singleton)

    After experimenting with some ECS (entity component systems) ideas I decided that it is in my case not worth the 
    effort as long as there is not an army of artists and content creators that would work for me. So I stick with the 
    classical approach using inherritance to define entities even if this provokes potencially ugly code within the 
    application layer. I don't care. I want to keep the engine clean. ML

    \deprecated will work out an ECS
    */
    class IGOR_API iEntityManager : public iModule<iEntityManager>
    {

        friend class iModule<iEntityManager>;
        friend class iEntity_Old;
        friend class iEntityLocatable;

    public:
        /*! \returns pointer to entity for given entity ID

        \param entityID the entity id
        */
        iEntity_Old *getEntity(uint64 entityID);

        /*! destroys entity with given entity id

        \param entityID the entity id
        */
        void destroyEntity(uint64 entityID);

        /*! returns list of entities within given sphere

        \param sphere the sphere to filter with
        \param[out] entities the resulting list with entity ids
        */
        void getEntities(const iSphered &sphere, std::vector<uint64> &entities);

        /*! returns all entities

        \param[out] entities list of all the entities
        */
        void getEntities(std::vector<uint64> &entities);

        /*! calls all entities handles and updates positions in spatial db
        */
        void handle();

    private:
        /*! list of all the entities
        */
        std::map<uint64, iEntity_Old *> _entities;

        /*! list of all entitties which are marked for deletion
        */
        std::vector<uint64> _toDelete;

        /*! spatial db of positioned entitties
        */
        iOctree *_octree = nullptr;

        /*! registers an entity

        all entities call this them selves in their constructor

        \param entity entity to register
        */
        void registerEntity(iEntity_Old *entity);

        /*! unregister entity

        all entities call this them selves in their destructor

        \param entity entity to unregister
        */
        void unregisterEntity(iEntity_Old *entity);

        /*! create octree
        */
        iEntityManager();

        /*! cleanup
        */
        ~iEntityManager();
    };

} // namespace igor

#endif // __IGOR_ENTITYMANAGER_H__