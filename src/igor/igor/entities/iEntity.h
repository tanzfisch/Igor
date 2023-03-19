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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_ENTITY__
#define __IGOR_ENTITY__

#include <igor/entities/iEntityScene.h>

#include <iaux/system/iaMutex.h> // deprecated

namespace igor
{
    /*! entity
     */
    class IGOR_API iEntity
    {
    public:
        /*! does nothing
         */
        iEntity() = default;

        /*! copy ctor

        \param other the entity to make a copy from
        */
        iEntity(const iEntity &other) = default;

        /*! \returns true if entity is valid
         */
        bool isValid() const;

        /*! param ctor

        \param entity the entity handle
        \param scene the scene this entity belongs to

        \todo maybe scene could be a module and globaly accessible
        */
        iEntity(iEntityID entity, iEntityScene &scene);

        /*! \returns entity id
         */
        iEntityID getID() const;

        /*! adds component to entity of given type
         */
        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            return _scene->getRegistry().emplace_or_replace<T>(_entity, std::forward<Args>(args)...);
        }

        /*! \returns component of entity of given type
         */
        template <typename T>
        T &getComponent() const
        {
            return _scene->getRegistry().get<T>(_entity);
        }

        /*! \returns component of entity of given type
         */
        template <typename T>
        T *tryGetComponent() const
        {
            return _scene->getRegistry().try_get<T>(_entity);
        }

        /*! \returns true if entity has component of given type
         */
        template <typename T>
        bool hasComponent() const
        {
            return _scene->getRegistry().try_get<T>(_entity) != nullptr;
        }

        /*! removes component of given type
         */
        template <typename T>
        void removeComponent()
        {
            _scene->getRegistry().remove<T>(_entity);
        }

        /*! \returns entity name
         */
        const iaString getName() const;

    private:
        /*! the entity ID
         */
        iEntityID _entity;

        /*! the scene this entity is in
         */
        iEntityScene *_scene;
    };

    /*! pointer to entity
     */
    typedef iEntity *iEntityPtr;

    /*///////////////////7 deprecated*/

    /*! engine internal entity base types
     */
    enum class iEntityType
    {
        Undefined,
        Base,
        Locatable
    };

    /*! entity base class
    \deprecated will work out an ECS
    */
    class IGOR_API iEntity_Old
    {

        /*! so we can call the handle
         */
        friend class iEntityManager;

    public:
        /*! invalid entity id definition
         */
        static const uint64 INVALID_ENTITY_ID;

        /*! \returns entity id
         */
        uint64 getID() const;

        /*! \returns entity type
         */
        iEntityType getType() const;

        /*! init id and register entity
         */
        iEntity_Old();

        /*! unregister
         */
        virtual ~iEntity_Old();

    protected:
        /*! called every simulation frame
         */
        virtual void handle() = 0;

        /*! entity type
         */
        iEntityType _type = iEntityType::Undefined;

    private:
        /*! entity id
         */
        uint64 _id = 0;

        /*! next entity id
         */
        static uint64 _nextID;

        /*! mutex to protec id generation
         */
        static iaMutex _mutexID;
    };

} // namespace igor

#endif // __IGOR_ENTITY__