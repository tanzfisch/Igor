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

#ifndef IGOR_ENTITY_SYSTEM_MODULE_H
#define IGOR_ENTITY_SYSTEM_MODULE_H

#include <igor/resources/module/iModule.h>

#include <igor/entities/iEntityScene.h>

#include <unordered_map>

namespace igor
{

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
        \param addIgorSystems if true adds all igor systems to it. if false there will be no systems registered with this scene
        \returns new created scene
        */
        iEntityScenePtr createScene(const iaString &name = "", bool addIgorSystems = true);

        /*! \returns scene for given scene id

        \param sceneID the given id
        */
        iEntityScenePtr getScene(const iEntitySceneID &sceneID);

        /*! destroys scene with given id

        \param sceneID the given scene id
        */
        void destroyScene(const iEntitySceneID &sceneID);

        /*! updates all scenes and cleans up scene lists
         */
        void onUpdate();

        /*! renders all scenes
         */
        void onRender(float32 clientWidth, float32 clientHeight);

        /*! set's the simulation rate in Hz

        default is 60Hz

        \param simulationRate simulation rate in Hz
        */
        void setSimulationRate(float64 simulationRate);

        /*! \returns simulation rate
        */
        float64 getSimulationRate();        

    private:
        /*! entity scenes
         */
        std::unordered_map<iEntitySceneID, iEntityScenePtr> _scenes;

        /*! simulation rate in Hz
        */
        float64 _simulationRate = 60.0;        

        /*! simulation frame time
        */
        iaTime _simulationFrameTime = iaTime::getNow();

    };

} // namespace igor

#endif // __IGOR_ENTITY_SYSTEM_MODULE__