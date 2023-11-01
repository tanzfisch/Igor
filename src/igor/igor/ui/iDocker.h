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
#include <igor/resources/texture/iTexture.h>

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

        \param desktopRect rect of the desktop
        \param mousePos the mouse position
        */
        void update(const iaRectanglei &desktopRect, const iaVector2i &mousePos);

        /*! draw docker overlay
         */
        void draw();

        void drawDebug(); // TODO remove later

        /*! dock a dialog

        \param dialogID the dialog ID to dock
        */
        void dock(iWidgetID dialogID);

        /*! undock given dialog

        \param dialogID the given dialog id
        */
        void undock(iWidgetID dialogID);

    private:
        struct iDockArea
        {
            /*! id of dialog in this dock area. An area can be empty
             */
            iWidgetID _dialog = iWidget::INVALID_WIDGET_ID;

            /*! sub area A
             */
            std::shared_ptr<iDockArea> _areaA;

            /*! sub area B
             */
            std::shared_ptr<iDockArea> _areaB;

            /*! parent area

            nullptr in case this is the root area
            */
            std::weak_ptr<iDockArea> _parent;

            /*! if true the split is vertical. Means areaA is on the left and areaB is on the right. Only applies if there is sub areas
             */
            bool _verticalSplit = true;

            /*! the ratio of the split 0.0-1.0. Only applies if there is sub areas
             */
            float32 _ratio = 0.5f;

            iDockArea(std::shared_ptr<iDockArea> parent)
                : _parent(parent)
            {
            }
        };

        /*! root area
         */
        std::shared_ptr<iDockArea> _root;

        /*! marks the target area
         */
        std::shared_ptr<iDockArea> _targetArea;

        iaRectanglei _selectorCenter;
        iaRectanglei _selectorRight;
        iaRectanglei _selectorLeft;
        iaRectanglei _selectorBottom;
        iaRectanglei _selectorTop;

        iaRectanglei _selectorRightEdge;
        iaRectanglei _selectorLeftEdge;
        iaRectanglei _selectorBottomEdge;
        iaRectanglei _selectorTopEdge;

        iTexturePtr _selectorCenterTexture;
        iTexturePtr _selectorRightTexture;
        iTexturePtr _selectorLeftTexture;
        iTexturePtr _selectorBottomTexture;
        iTexturePtr _selectorTopTexture;
        iTexturePtr _selectorQuarterRightTexture;
        iTexturePtr _selectorQuarterLeftTexture;
        iTexturePtr _selectorQuarterBottomTexture;
        iTexturePtr _selectorQuarterTopTexture;

        bool _subdivideCenter;
        bool _subdivideLeftHalf;
        bool _subdivideRightHalf;
        bool _subdivideTopHalf;
        bool _subdivideBottomHalf;

        bool _subdivideLeftEdge;
        bool _subdivideRightEdge;
        bool _subdivideTopEdge;
        bool _subdivideBottomEdge;

        /*! the target rectangle for the moving dialog to dock
         */
        iaRectanglei _targetRect;

        /*! if true area can be or will be subdivided
         */
        bool _subdivide = false;

        /*! store last desktop rect
         */
        iaRectanglei _desktopRect;

        /*! updates position of all dialogs based on desktop size

        \param area the current area processed
        \param rect rect of area
        */
        void update(std::shared_ptr<iDockArea> area, const iaRectanglei &rect);

        void updateTargets(std::shared_ptr<iDockArea> area, const iaRectanglei &rect, const iaVector2i &pos);

        void dock(std::shared_ptr<iDockArea> area, iWidgetID dialogID, const iaRectanglei &rect);

        bool undock(std::shared_ptr<iDockArea> area, iWidgetID dialogID);

        bool isEmpty(std::shared_ptr<iDockArea> area);

        void loadResources();

        void update();

        void drawDebug(std::shared_ptr<iDockArea> area, const iaRectanglei &rect, int nesting);
    };

}

#endif // __IGOR_DOCKER__