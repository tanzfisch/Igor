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

#ifndef IGOR_ENTITY_H
#define IGOR_ENTITY_H

#include <igor/entities/components/iComponents.h>

#include <unordered_map>
#include <typeindex>

namespace igor
{

    /*! pointer to entity
     */
    class iEntity;
    typedef iEntity *iEntityPtr;

    /*! pointer to scene
     */
    class iEntityScene;
    typedef iEntityScene *iEntityScenePtr;

    /*! entity
     */
    class IGOR_API iEntity
    {
        friend class iEntityScene;

    public:
        /*! ctor with name

        \param name the name of this entity
        */
        iEntity(const iaString &name = "");

        /*! does nothing
         */
        iEntity() = default;

        /*! does nothing
         */
        ~iEntity() = default;

        /*! \returns entity id
         */
        const iEntityID &getID() const;

        /*! \returns name of entity
         */
        const iaString &getName() const;

        /*! sets parent by id

        \param parentID the given parent id
        */
        void setParent(const iEntityID &parentID);

        /*! \returns parent id
        */
        const iEntityID getParent() const;

        /*! \returns scene this entity belongs to
        */
        iEntityScenePtr getScene() const;

        /*! \returns true if entity is in scene
        */
        bool isInScene() const;

        /*! \returns true if entity has parent
        */
        bool hasParent() const;

        /*! add component (or overrides if already existing)

        entity takes ownership of component

        \param component the component to add
        \returns the new component
        */
        template<typename T>
        T* addComponent(T* component);

        /*! \returns component for given type
        */
        template<typename T>
        T* getComponent() const;

        /*! destroys given component by type
        */
        template<typename T>
        void destroyComponent();

        /*! adds behaviour to entity

        \param behaviour the behaviour to be added
        \param userData user data added to behaviour
        */
        void addBehaviour(const iBehaviourDelegate &behaviour, const std::any &userData = std::any());

        /*! removes behaviour from entity

        \param behaviour the behaviour to be removed
        */
        void removeBehaviour(const iBehaviourDelegate &behaviour);

    private:
        /*! the entities id (unique)
         */
        iEntityID _id;

        /*! the entities name (non unique)
         */
        iaString _name;

        /*! scene this entity belongs to
        */
        iEntityScenePtr _scene = nullptr;

        /*! map of components
         */
        std::unordered_map<std::type_index, iEntityComponentPtr> _components;

        /*! children if any
         */
        std::vector<iEntityPtr> _children;

        /*! parent (optional)
         */
        iEntityPtr _parent = nullptr;
    };

#include <igor/entities/iEntity.inl>

} // namespace igor

#endif // IGOR_ENTITY_H