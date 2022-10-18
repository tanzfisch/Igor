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

namespace igor
{
    /*! entt wrapper
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
            return _scene->_registry.emplace_or_replace<T>(_entity, std::forward<Args>(args)...);
        }

        /*! \returns component of entity of given type
        */
        template <typename T>
        T &getComponent() const
        {
            return _scene->_registry.get<T>(_entity);
        }

        /*! \returns component of entity of given type
        */
        template <typename T>
        T* tryGetComponent() const
        {
            return _scene->_registry.try_get<T>(_entity);
        }        

        /*! \returns true if entity has component of given type
        */
        template <typename T>
        bool hasComponent() const
        {
            return _scene->_registry.try_get<T>(_entity) != nullptr;
        }        

        /*! removes component of given type
        */
        template <typename T>
        void removeComponent()
        {
            _scene->_registry.remove<T>(_entity);
        }

        /*! \returns entity name
        */
        const iaString getName() const;

    private:
        iEntityID _entity;
        iEntityScene *_scene;
    };

    /*! pointer to entity
    */
    typedef iEntity* iEntityPtr;

} // namespace igor

#endif // __IGOR_ENTITY__