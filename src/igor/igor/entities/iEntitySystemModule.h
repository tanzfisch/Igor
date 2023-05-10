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

#ifndef __IGOR_ENTITY_SYSTEM_MODULE_H__
#define __IGOR_ENTITY_SYSTEM_MODULE_H__

#include <igor/resources/module/iModule.h>

#include <igor/entities/iEntityScene.h>

#include <iaux/system/iaMutex.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

#include <vector>

namespace igor
{

    /*! entity system module

    manages and updates all entity systems created
    */
    class IGOR_API iEntitySystemModule : public iModule<iEntitySystemModule>
    {

        friend class iModule<iEntitySystemModule>;

    public:
        /*! creates a scene and returns it

        \returns new created scene
        */
        iEntityScenePtr createScene();

        /*! updates all scenes and cleans up scene lists
         */
        void onUpdate();

        /*! renders all scenes
         */
        void onRender(float32 clientWidth, float32 clientHeight);

        /*! starts/continues the entity systems to run
        */
        void start();

        /*! stops the entity system to run (except rendering)
        */
        void stop();

        /*! set's the simulation rate in Hz

        default is 60Hz

        \param simulationRate simulation rate in Hz
        */
        void setSimulationRate(float64 simulationRate);

        /*! \returns simulation rate
        */
        float64 getSimulationRate();        

    private:
        /*! mutex to safeguard entity scene list
         */
        iaMutex _mutex;

        /*! entity scenes
         */
        std::vector<iEntityScenePtr> _scenes;

        /*! simulation rate in Hz
        */
        float64 _simulationRate = 60.0;        

        /*! if true entity system is running

        render is always running
        */
        bool _running = true;

        /*! last frame time
        */
        iaTime _lastTime;

        /*! does nothing
         */
        iEntitySystemModule() = default;

        /*! does nothing
         */
        ~iEntitySystemModule() = default;
    };

} // namespace igor

#endif // __IGOR_ENTITY_SYSTEM_MODULE_H__