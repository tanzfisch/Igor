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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_LAYERPROFILER__
#define __IGOR_LAYERPROFILER__

#include <igor/layers/iLayer.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/renderer/iView.h>
#include <igor/events/iEventWindow.h>
#include <igor/events/iEventKeyboard.h>
#include <igor/resources/profiler/iProfilerVisualizer.h>
#include <igor/resources/texture/iTextureFont.h>

namespace igor
{
    /*! layer that shows profiler and debug information
     */
    class IGOR_API iLayerProfiler : public iLayer
    {

    public:
        /*! ctor initializes layer

        !!! set high zIndex so this layer can exclude it self from profiling !!!

        \param window the given window
        \param name the name of this layer
        \param zIndex the z index
        \param verbosity the profiler verbosity
        */
        iLayerProfiler(iWindowPtr window, const iaString &name = "Profiler", int32 zIndex = 1000, iProfilerVerbosity verbosity = iProfilerVerbosity::FPSOnly);

        /*! does nothing
         */
        ~iLayerProfiler() = default;

        /*! sets profiler verbosity

        \param verbosity the verbosity of the profiler visualization
        */
        void setVerbosity(iProfilerVerbosity verbosity);

        /*! cycles verbosity
        */
        void cycleVerbosity();

        /*! \returns current profiler verbosity
        */
        iProfilerVerbosity getVerbosity();

    private:
        /*! the viewport
         */
        iView _view;

        /*! displays profiler
         */
        iProfilerVisualizer _profilerVisualizer;

        /*! texture font we use to display the profiler
         */
        iTextureFontPtr _font;

        /*! initialize example
         */
        void onInit() override;

        /*! deinitialize example
         */
        void onDeinit() override;

        /*! called on any other event

        \param event the event to handle
        */
        void onEvent(iEvent &event) override;

        /*! called when key was released

        \param event the event to handle
        */
        bool onKeyUp(iEventKeyUp &event);

        /*! called by orthogonal view
         */
        void onRender();

        /*! handle window resize event

        \param event the window resize event
        \returns true if consumed
        */
        bool onWindowResize(iEventWindowResize &event);
    };

    /*! layer profiler pointer definition
     */
    typedef iLayerProfiler *iLayerProfilerPtr;

} // namespace igor

#endif // __IGOR_LAYERPROFILER__
