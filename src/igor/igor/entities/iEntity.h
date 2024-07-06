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
// (c) Copyright 2012-2024 by Martin Loga
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

    /*! entity component mask definition
    */
    typedef std::bitset<IGOR_MAX_ENTITY_COMPONENT_TYPES> iEntityComponentMask;

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
        friend class iEntitySystem;
        friend class iEntityView;

    public:
        /*! \returns entity id
         */
        const iEntityID &getID() const;

        /*! \returns name of entity
         */
        const iaString &getName() const;

        /*! sets parent of this entity by id

        \param parentID the given parent id
        */
        void setParent(const iEntityID &parentID);

        /*! sets parent of this entity

        \param parent the given parent 
        */
        void setParent(iEntityPtr parent);

        /*! removes parent connection
        */
        void removeParent();

        /*! \returns parent id
        */
        iEntityPtr getParent() const;

        /*! \returns true if entity has parent
        */
        bool hasParent() const;

        /*! \returns active children
        */
        const std::vector<iEntityPtr>& getChildren() const;

        /*! \returns inactive children
        */
        const std::vector<iEntityPtr>& getInactiveChildren() const;

        /*! \returns true if entity has children
        */
        bool hasChildren() const;

        /*! sets wether and entity is active or not

        \param active if true entity will be active
        */
        void setActive(bool active);

        /*! \returns true if entity is active
        */
        bool isActive() const;

        /*! \returns scene this entity belongs to
        */
        iEntityScenePtr getScene() const;

        /*! calculate type hash from component types

        \param types the types to use for calculation
        */
        static iEntityComponentMask calcComponentMask(const std::vector<std::type_index> &types);        

        /*! \returns type hash of this entity
        */
        iEntityComponentMask getComponentMask() const;

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
        \param name the name of the behaviour
        */
        void addBehaviour(const iBehaviourDelegate &behaviour, const std::any &userData = std::any(), const iaString &name = "");

        /*! removes behaviour from entity

        \param behaviour the behaviour to be removed
        */
        void removeBehaviour(const iBehaviourDelegate &behaviour);

        /*! removes all components
        */
        void clearComponents();

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

        /*! parent entity
        */
        iEntityPtr _parent = nullptr;

        /*! child entities
        */
        std::vector<iEntityPtr> _children;

        /*! inactive child entities
        */
        std::vector<iEntityPtr> _inactiveChildren;

        /*! if true entity is active and will be processed
        */
        bool _active = true;

        /*! true in case hierarchy (or transforms) is dirty
        */
        bool _dirtyHierarchy = true;

        /*! map of components
         */
        std::unordered_map<std::type_index, iEntityComponentPtr> _components;

        /*! added components end up in a queue first
        */
        std::vector<std::pair<std::type_index, iEntityComponentPtr>> _addedComponents;

        /*! type hash of entity
        */
        iEntityComponentMask _componentMask = 0;

        /*! ctor with name

        \param name the name of this entity
        */
        iEntity(const iaString &name = "");

        /*! remove components
         */
        ~iEntity();

        /*! calculate type hash from component types

        \param components the components to use for calculation
        */
        iEntityComponentMask calcComponentMask();

        /*! flushes queues
        */
        void processComponents();

        /*! destroy component by type id

        \param typeID the components type id
        */
        void destroyComponent(const std::type_index &typeID);

        /*! notifies scene that components have changed
        */
        void onEntityChanged();

    };

#include <igor/entities/iEntity.inl>

} // namespace igor

#endif // IGOR_ENTITY_H 