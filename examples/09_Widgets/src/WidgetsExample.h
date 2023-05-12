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
//    (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __WIDGETEXAMPLE_H__
#define __WIDGETEXAMPLE_H__

#include <igor/igor.h>
using namespace igor;
using namespace iaux;

#include "Background3D.h"

/*! this example shows how to use Igor Widgets

    we derive from iLayerWidgets so the handling of the widget manager is covered for us
*/
class WidgetsExample : public iLayerWidgets
{

public:
    /*! ctor initializes widgets

    \param window the given window
    */
    WidgetsExample(iWindowPtr window, iLayerProfilerPtr layerProfiler, Background3D *backGround);

    /*! does nothing
     */
    ~WidgetsExample() = default;

private:
    /*! the main dialog
     */
    iDialogPtr _dialog = nullptr;

    /*! layer profiler
     */
    iLayerProfilerPtr _layerProfiler;

    /*! 3d scene in background
     */
    Background3D *_backGround;

    /*! triggered when main dialog got closed

    \param dialog source of the event
    */
    void onCloseDialog(iDialogPtr dialog);

    /*! initializes GUI
     */
    void initGUI();

    /*! \returns menu bar
     */
    iWidgetMenuBarPtr createMenu();

    /*! triggered by exit button. will shut down application

    \param source the source widget of this event
    */
    void onExitClick(const iWidgetPtr source);

    /*! profiler verbosity change event
     */
    void onProfilerVerbosityChange(const iWidgetPtr source);

    /*! on wireframe change event
     */
    void onWireframeChange(const iWidgetPtr source);

    /*! on octree change event
     */
    void onOctreeChange(const iWidgetPtr source);

    /*! on bounds change event
     */
    void onBoundsChange(const iWidgetPtr source);

    /*! initialize example
     */
    void onInit() override;

    /*! deinitialize example
     */
    void onDeinit() override;
};

#endif // __WIDGETEXAMPLE_H__
