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

#ifndef __IGOR_DOCKER__
#define __IGOR_DOCKER__

#include <igor/ui/dialogs/iDialog.h>

#include <iaux/data/iaRectangle.h>
using namespace iaux;

#include <memory>

namespace igor
{

    class iDocker
    {

    public:
        /*! init docker
         */
        iDocker();

        /*! does nothing
         */
        ~iDocker() = default;

        /*! updates position of all dialogs based on desktop size

        \param desktopWidth width of the desktop
        \param desktopHeight height of the desktop
        */
        void update(uint32 desktopWidth, uint32 desktopHeight);

        /*! draw docker overlay
         */
        void draw(uint32 desktopWidth, uint32 desktopHeight);

    private:
        struct iDockArea
        {
            /*! id of dialog in this dock area. An area can be empty
             */
            iWidgetID _dialog = iWidget::INVALID_WIDGET_ID;

            /*! sub area A
             */
            std::unique_ptr<iDockArea> _areaA = nullptr;

            /*! sub area B
             */
            std::unique_ptr<iDockArea> _areaB = nullptr;

            /*! if true the split is vertical. Means areaA is on the left and areaB is on the right. Only applies if there is sub areas
             */
            bool _verticalSplit = true;

            /*! the ratio of the split 0.0-1.0. Only applies if there is sub areas
             */
            float32 _ratio = 0.5f;
        };

        /*! root area
         */
        iDockArea _root;

        /*! updates position of all dialogs based on desktop size

        \param area the current area processed
        \param rect rect of area
        */
        void update(iDockArea *area, const iaRectanglei &rect);

        void draw(iDockArea *area, const iaRectanglei &rect);
    };

}

#endif // __IGOR_DOCKER__