//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_ENTITY_SYSTEM_MODULE_H
#define IGOR_ENTITY_SYSTEM_MODULE_H

#include <igor/resources/module/iModule.h>

#include <igor/entities/iEntityScene.h>
#include <igor/resources/prefab/iPrefab.h>

#include <unordered_map>

namespace igor
{
    /*! event called after creation of new entity
     */
    IGOR_EVENT_DEFINITION(iCreatedEntity, iEntityPtr);

    /*! event called before destruction of given entity
     */
    IGOR_EVENT_DEFINITION(iDestroyEntity, iEntityPtr);

    /*! event called when hierarchy of given scene has changed
     */
    IGOR_EVENT_DEFINITION(iHierarchyChanged, iEntityScenePtr);

    /*! event called when entity name changed
     */
    IGOR_EVENT_DEFINITION(iEntityNameChanged, iEntityPtr);

    /*! entity system module
     */
    class IGOR_API iEntitySystemModule : public iModule<iEntitySystemModule>
    {

        friend class iModule<iEntitySystemModule>;
        friend class iEntity;

    public:
        /*! creates a scene and returns it

        ownership of scenes always stay with module

        \param name the name of the scene
        \param id optional id to override the generated one
        \param addIgorSystems if true adds all igor systems to it. if false there will be no systems registered with this scene
        \returns new created scene
        */
        iEntityScenePtr createScene(const iaString &name = "", const iEntitySceneID &id = iEntitySceneID::getInvalid(), bool addIgorSystems = true);

        /*! \returns scene for given scene id

        \param sceneID the given id
        */
        iEntityScenePtr getScene(const iEntitySceneID &sceneID);

        /*! insert prefab at given entity

        \param prefab the source scene or prefab to insert
        \param entity the entity inside the destination scene
        */
        void insert(iPrefabPtr prefab, iEntityPtr entity);

        /*! destroys scene with given id

        \param sceneID the given scene id
        */
        void destroyScene(const iEntitySceneID &sceneID);

        /*! updates all scenes and cleans up scene lists
         */
        void onUpdate();

        /*! triggers pre render update stage with given scene

        \param scene the given scene to update/render
         */
        void onPreRender(iEntityScenePtr scene);

        /*! triggers render update stage with given scene

        \param scene the given scene to update/render
         */
        void onRender(iEntityScenePtr scene);

        /*! set's the simulation rate in Hz

        default is 60Hz

        \param simulationRate simulation rate in Hz
        */
        void setSimulationRate(float64 simulationRate);

        /*! \returns simulation rate
         */
        float64 getSimulationRate();

        /*! registers a component type

        \param componentTypeName component type name (must be unique)
         */
        template <typename T>
        void registerComponentType(iEntityComponentFactory factoryFunction, const iaString &componentTypeName);

        /*! \returns mask for given component type

        \param typeID the given component type
        */
        iEntityComponentMask getComponentMask(const std::type_index &typeID);

        /*! activates given scene

        \param scene the given scene to activate
        */
        void activateScene(iEntityScenePtr scene);

        /*! deactivates given scene

        \param scene the given scene to deactivate
        */
        void deactivateScene(iEntityScenePtr scene);

        /*! \returns all active scenes
         */
        std::vector<iEntityScenePtr> getActiveScenes();

        /*! \returns all inactive scenes
         */
        std::vector<iEntityScenePtr> getInactiveScenes();

        /*! \returns entity got created event
         */
        iCreatedEntityEvent &getCreatedEntityEvent();

        /*! \returns entity will be destroyed event
         */
        iDestroyEntityEvent &getDestroyEntityEvent();

        /*! \returns hierarchy changed event
         */
        iHierarchyChangedEvent &getHierarchyChangedEvent();

        /*! \returns entity name changed event
         */
        iEntityNameChangedEvent &getEntityNameChangedEvent();

        /*! clear everything
         */
        void clear();

        /*! \returns registered component types
         */
        const std::unordered_map<std::type_index, iEntityComponentTypeInfo> &getRegisteredComponentTypes() const;

    private:
        /*! entity scenes
         */
        std::unordered_map<iEntitySceneID, iEntityScenePtr> _scenes;

        /*! active entity scenes
         */
        std::vector<iEntityScenePtr> _activeScenes;

        /*! inactive entity scenes
         */
        std::vector<iEntityScenePtr> _inactiveScenes;

        /*! mutex protecting all data
         */
        iaMutex _mutex;

        /*! simulation rate in Hz
         */
        float64 _simulationRate = 60.0;

        /*! simulation frame time
         */
        iaTime _simulationFrameTime;

        /*! event triggered when entity got created
         */
        iCreatedEntityEvent _createdEntityEvent;

        /*! event triggered before entity get's destroyed
         */
        iDestroyEntityEvent _destroyEntityEvent;

        /*! the hierarchy changed event
         */
        iHierarchyChangedEvent _hierarchyChangedEvent;

        /*! entity name changed event
         */
        iEntityNameChangedEvent _entityNameChangedEvent;

        /*! the registered component types

        IGOR_MAX_ENTITY_COMPONENT_TYPES is the maximum that can be registered
        */
        std::unordered_map<std::type_index, iEntityComponentTypeInfo> _registeredComponentTypes;

        /*! register known types
         */
        iEntitySystemModule();
    };

#include <igor/entities/iEntitySystemModule.inl>

} // namespace igor

#endif // __IGOR_ENTITY_SYSTEM_MODULE__