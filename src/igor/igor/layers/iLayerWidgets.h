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
//    (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_LAYERWIDGETS_H
#define IGOR_LAYERWIDGETS_H

#include <igor/layers/iLayer.h>
#include <igor/ui/theme/iWidgetTheme.h>
#include <igor/renderer/iView.h>
#include <igor/events/iEventWindow.h>

namespace igor
{
    /*! layer for witdget system
    */
    class IGOR_API iLayerWidgets : public iLayer
    {

    public:
        /*! ctor initializes layer

        \param window the given window
        \param theme the widget theme to use
        \param name the name of this layer
        \param zIndex the z index
        */
        iLayerWidgets(iWidgetThemePtr theme, iWindowPtr window, const iaString &name, int32 zIndex);

        /*! does nothing
        */
        virtual ~iLayerWidgets() = default;

    protected:
        /*! initialize example
        */
        virtual void onInit() override;

        /*! deinitialize example
        */
        virtual void onDeinit() override;

        /*! called on application pre draw event
        */
        virtual void onUpdate() override;

        /*! called on any other event

        \param event the event to handle
        */
        virtual void onEvent(iEvent &event) override;

    private:
        /*! the viewport
        */
        iView _view;

        /*! the widget theme in use
        */
        iWidgetThemePtr _theme;

        /*! called by orthogonal view
        */
        void onRender();

        /*! handle window resize event

        \param event the window resize event
        \returns true if consumed
        */
        bool onWindowResize(iEventWindowResize &event);
    };

} // namespace igor
#endif // IGOR_LAYERWIDGETS_H
