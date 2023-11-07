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

#ifndef __IGOR_WIDGET_DOCKING_LAYOUT__
#define __IGOR_WIDGET_DOCKING_LAYOUT__

#include <igor/ui/widgets/iWidget.h>
#include <igor/resources/texture/iTexture.h>

#include <memory>

namespace igor
{

    /*! docking layout widget
     */
    class IGOR_API iWidgetDockingLayout : public iWidget
    {
    public:
        /*! ctor initializes member variables

        \param parent optional parent
        */
        iWidgetDockingLayout(const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        ~iWidgetDockingLayout() = default;

        /*! called once per frame
         */
        void onUpdate() override;

        /*! dock a dialog

        \param dialogID the dialog ID to dock
        \returns true if successfully docked
        */
        bool dock(iWidgetID dialogID);

        /*! undock given dialog

        \param dialogID the given dialog id
        \returns true if successfully un-docked
        */
        bool undock(iWidgetID dialogID);

        /*! draws overlay
         */
        void drawOverlay();

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

        iaRectanglef _selectorCenter;
        iaRectanglef _selectorRight;
        iaRectanglef _selectorLeft;
        iaRectanglef _selectorBottom;
        iaRectanglef _selectorTop;

        iaRectanglef _selectorRightEdge;
        iaRectanglef _selectorLeftEdge;
        iaRectanglef _selectorBottomEdge;
        iaRectanglef _selectorTopEdge;

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
        iaRectanglef _targetRect;

        /*! if true area can be or will be subdivided
         */
        bool _subdivide = false;

        /*! store last desktop rect
         */
        iaRectanglef _parentRect;

        /*! updates mouse click targets

        \param area the current context area
        \param rect the rectangle of the context area (relative to parent widget)
        \param mousePos the mouse pos (relative to parent widget)
        */
        void updateTargets(std::shared_ptr<iDockArea> area, const iaRectanglef &rect, const iaVector2f &mousePos);

        /*! calls recursive functions updateDialogs
        */
        void updateDialogs();

        /*! updates position of all dialogs based on desktop size

        \param area the current area processed
        \param rect rect of area
        */
        void updateDialogs(std::shared_ptr<iDockArea> area, const iaRectanglef &rect);

        void dock(std::shared_ptr<iDockArea> area, iWidgetID dialogID, const iaRectanglef &rect);

        bool undock(std::shared_ptr<iDockArea> area, iWidgetID dialogID);

        bool isEmpty(std::shared_ptr<iDockArea> area);

        void loadResources();

        

        void drawDebug(std::shared_ptr<iDockArea> area, const iaRectanglef &rect, int nesting);

        /*! updates position of all dialogs based on desktop size

        \param parentRect parent rect absolute in pixel
        \param mousePos the mouse position absolute in pixel
        */
        void update(const iaRectanglef &parentRect, const iaVector2f &mousePos);

        const iaVector2f calcMinSize(std::shared_ptr<iDockArea> area) const;

        /*! draws the widget
         */
        void draw() override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! updates widget alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        void updateAlignment(int32 clientWidth, int32 clientHeight) override;
    };

    /*! widget grid pointer definition
     */
    typedef iWidgetDockingLayout *iWidgetDockingLayoutPtr;

} // namespace igor

#endif // __IGOR_WIDGET_BOX_LAYOUT__
