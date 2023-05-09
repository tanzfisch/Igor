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
// (c) Copyright 2012-2023 by Martin Loga
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

        /*! param ctor

        \param entity the entity handle
        \param scene the scene this entity belongs to

        \todo maybe scene could be a module and globally accessible
        */
        iEntity(iEntityID entity, iEntityScenePtr scene);

        /*! \returns entity id
         */
        iEntityID getID() const;

        /*! \returns true if entity is valid
         */
        bool isValid() const;

        /*! \returns entity name
         */
        const iaString getName() const;

        /*! sets name of entity

        \param name the name to set
        */
        void setName(const iaString &name);

        /*! \returns true if entity is active
         */
        bool isActive() const;

        /*! sets entity active

        \param active if true entity is active
        */
        void setActive(bool active);

        /*! adds behaviour to entity

        \param behaviour the behaviour to be added
        \param userData user data added to behaviour
        */
        void addBehaviour(const iBehaviourDelegate &behaviour, const std::any &userData = std::any());

        /*! removes behaviour from entity

        \param behaviour the behaviour to be removed
        */
        void removeBehaviour(const iBehaviourDelegate &behaviour);

        /*! set parent of entity

        \param parent the parent id to use. if parent id invalid the parent relationship is reset
        */
        void setParent(iEntityID parent);

        /*! \returns the parent id or invalid id if there is no parent
         */
        iEntityID getParent() const;

        /*! sets motion interaction type

        \param interactionType the motion interaction type
        */
        void setMotionInteractionType(iMotionInteractionType interactionType);

        /*! \returns motion interaction type
         */
        iMotionInteractionType getMotionInteractionType() const;

        /*! adds component to entity

        \param component the component to add
        */
        template <typename T>
        T &addComponent(const T &component)
        {
            return _scene->addComponent<T>(_entity, component);
        }

        template<typename T>
        T &addCustomComponent(const T &component)        
        {
            return _scene->addCustomComponent<T>(_entity, component);
        }

        /*! \returns component of entity of given type
         */
        template <typename T>
        T &getComponentV2()
        {
            return _scene->getComponent<T>(_entity);
        }

        template<typename T>
        T &getCustomComponent()
        {
            return _scene->getCustomComponent<T>(_entity);
        }        

        /*! \returns component of entity of given type

        returns nullptr in case component does not exist
         */
        template <typename T>
        T *tryGetComponentV2() const
        {
            return _scene->tryGetComponent<T>(_entity);
        }

        template <typename T>
        T *tryGetCustomComponent() const
        {
            return _scene->tryGetCustomComponent<T>(_entity);
        }        

        /*! removes component of given type
         */
        template <typename T>
        void removeComponent()
        {
            _scene->removeComponent<T>(_entity);
        }

        /////////// deprecated
        template <typename T>
        T *tryGetComponent() const
        {
            return _scene->getRegistry().try_get<T>(_entity);
        }

        /////////// deprecated
        template <typename T>
        T &getComponent() const
        {
            return _scene->getRegistry().get<T>(_entity);
        }

        /////////// deprecated
        template <typename T, typename... Args>
        T &addComponent(Args &&...args)
        {
            return _scene->getRegistry().emplace_or_replace<T>(_entity, std::forward<Args>(args)...);
        }

    private:
        /*! the entity ID
         */
        iEntityID _entity;

        /*! the scene this entity is in
         */
        iEntityScenePtr _scene;
    };

    /*! pointer to entity
     */
    typedef iEntity *iEntityPtr;

} // namespace igor

#endif // __IGOR_ENTITY__