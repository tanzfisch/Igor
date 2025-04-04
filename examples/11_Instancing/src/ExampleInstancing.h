//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2012-2025 by Martin Loga
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
// contact: martinloga@gmx.de

#ifndef IGOR_EXAMPLEINSTANCING_H
#define IGOR_EXAMPLEINSTANCING_H

#include <ExampleBase.h>

class ExampleInstancing : public ExampleBase
{

public:
    /*! init members

    \param window the given window
    */
    ExampleInstancing(iWindowPtr window);

    /*! does nothing
     */
    ~ExampleInstancing() = default;

private:
    /*! entity scene
     */
    iEntityScenePtr _entityScene = nullptr;

    /*! id of camera entity
    */
    iEntityID _camera;

    /*! id of camera heading entity
    */
    iEntityID _cameraHeading;

    /*! id of camera pitch entity
    */
    iEntityID _cameraPitch;

    /*! deinit example
     */
    void onDeinit() override;

    /*! init example
     */
    void onInit() override;

    /*! called on any other event

    \param event the event to handle
    */
    void onEvent(iEvent &event) override;

    /*! handles mouse move event

    \param event the mouse move event
    \returns true if consumed
    */
    bool onMouseMoveEvent(iEventMouseMove &event);

    /*! handles mouse wheel event

    \param event the mouse wheel event
    \returns true if consumed
    */
    bool onMouseWheelEvent(iEventMouseWheel &event);
};

#endif // IGOR_EXAMPLEINSTANCING_H