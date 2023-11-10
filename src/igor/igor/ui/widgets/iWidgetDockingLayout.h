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
#include <map>

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
        /*! left section texture
         */
        iTexturePtr _sectionLeftTexture;

        /*! right section texture
         */
        iTexturePtr _sectionRightTexture;

        /*! top section texture
         */
        iTexturePtr _sectionTopTexture;

        /*! bottom section texture
         */
        iTexturePtr _sectionBottomTexture;

        /*! the section to highlight
         */
        iaRectanglef _highlightSection;

        /*! highlight section flag
         */
        bool _highlightSectionVisible = false;

        /*! left section button
         */
        iaRectanglef _leftSectionButton;

        /*! right section button
         */
        iaRectanglef _rightSectionButton;

        /*! top section button
         */
        iaRectanglef _topSectionButton;

        /*! bottom section button
         */
        iaRectanglef _bottomSectionButton;

        /*! if true we subdivide on the left
        */
        bool _subdivideLeftEdge = false;

        /*! if true we subdivide on the right
        */
        bool _subdivideRightEdge = false;

        /*! if true we subdivide on the top
        */
        bool _subdivideTopEdge = false;

        /*! if true we subdivide on the bottom
        */
        bool _subdivideBottomEdge = false;

        /*! load some resources
         */
        void loadResources();

        /*! called once per frame
         */
        void onUpdate() override;

        /*! draws the widget
         */
        void draw() override;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! handles incoming mouse move events

        \param position mouse position
        */
        void handleMouseMove(const iaVector2f &pos) override;
    };

    /*! widget grid pointer definition
     */
    typedef iWidgetDockingLayout *iWidgetDockingLayoutPtr;

} // namespace igor

#endif // __IGOR_WIDGET_BOX_LAYOUT__
