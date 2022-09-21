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

#ifndef __IGOR_ENTITY__
#define __IGOR_ENTITY__

#include <igor/entities/iEntityScene.h>

#include <iaux/system/iaMutex.h> // deprecated

namespace igor
{
    /*! entt wrapper
    */
    class IGOR_API iEntity
    {
    public:
        iEntity() = default;
        iEntity(const iEntity &other) = default;
        iEntity(entt::entity entity, iEntityScene *scene);

        operator entt::entity() const
        {
            return _entity;
        }

        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            return _scene->_registry.emplace_or_replace<T>(_entity, std::forward<Args>(args)...);
        }

        template <typename T>
        T &getComponent() const
        {
            return _scene->_registry.get<T>(_entity);
        }

        template <typename T>
        void removeComponent()
        {
            _scene->_registry.remove<T>(_entity);
        }

        const iaString &getName() const;

    private:
        entt::entity _entity;
        iEntityScene *_scene;
    };

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