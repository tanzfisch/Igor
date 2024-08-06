//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef __IGOR_WIDGET_SPLITTER__
#define __IGOR_WIDGET_SPLITTER__

#include <igor/ui/widgets/iWidget.h>
#include <igor/ui/actions/iAction.h>
#include <igor/resources/texture/iTexture.h>

#include <memory>

namespace igor
{

    enum class iSplitterOrientation
    {
        Vertical,
        Horizontal
    };

    enum class iSplitterState
    {
        MouseOverVertical,
        MouseOverHorizontal,
        Moving,
        Inactive
    };

    /*! splitter widget
     */
    class IGOR_API iWidgetSplitter : public iWidget
    {

    public:
        /*! ctor initializes member variables

        \param dockingSplitter if true (and it allows to grow by content) dialogs can dynamically be docked to this splitter
        \param parent the optional parent
        */
        iWidgetSplitter(bool dockingSplitter = false, const iWidgetPtr parent = nullptr);

        /*! does nothing
         */
        ~iWidgetSplitter() = default;

        /*! sets the split ratio

        \param ratio the new ratio to set
        */
        void setRatio(float32 ratio);

        /*! \returns ratio of this splitter
         */
        float32 getRatio() const;

        /*! sets splitter orientation

        \param orientation if Vertical the children will be left and right from the splitter and vice versa
        */
        void setOrientation(iSplitterOrientation orientation);

        /*! \returns splitter orientation
         */
        iSplitterOrientation getOrientation() const;

        /*! adds a child widget to this widget

        This will add max two widgets. The first one will be left/top and the second one will be right/bottom

        \param widget the child widget to be added
        */
        void addWidget(iWidgetPtr widget) override;

    private:
        /*! splitter ration
         */
        float32 _ratio = 0.5f;

        /*! splitter orientation
         */
        iSplitterOrientation _orientation = iSplitterOrientation::Vertical;

        /*! if true this is a docking splitter which allows dialogs to dock on it
         */
        bool _dockingSplitter;

        /*! left section button
         */
        iaRectanglef _centerSectionButton;

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

        /*! left edge section button
         */
        iaRectanglef _leftEdgeSectionButton;

        /*! right edge section button
         */
        iaRectanglef _rightEdgeSectionButton;

        /*! top edge section button
         */
        iaRectanglef _topEdgeSectionButton;

        /*! bottom edge section button
         */
        iaRectanglef _bottomEdgeSectionButton;

        /*! the section to highlight
         */
        iaRectanglef _highlightSection;

        /*! highlight section flag
         */
        bool _validDockSection = false;

        /*! if true we dock in the center
         */
        bool _dockSectionCenter = false;

        /*! if true we dock on the left
         */
        bool _dockSectionLeft = false;

        /*! if true we dock on the right
         */
        bool _dockSectionRight = false;

        /*! if true we dock on the top
         */
        bool _dockSectionTop = false;

        /*! if true we dock on the bottom
         */
        bool _dockSectionBottom = false;

        /*! if true we dock on the left
         */
        bool _dockSectionLeftEdge = false;

        /*! if true we dock on the right
         */
        bool _dockSectionRightEdge = false;

        /*! if true we dock on the top
         */
        bool _dockSectionTopEdge = false;

        /*! if true we dock on the bottom
         */
        bool _dockSectionBottomEdge = false;

        /*! dock on sub section (-1 for not subsection)
         */
        int32 _dockSubSectionIndex = -1;

        /*! center button texture
         */
        iTexturePtr _selectorCenterTexture;

        /*! left button texture
         */
        iTexturePtr _selectorLeftTexture;

        /*! right button texture
         */
        iTexturePtr _selectorRightTexture;

        /*! top button texture
         */
        iTexturePtr _selectorTopTexture;

        /*! bottom button texture
         */
        iTexturePtr _selectorBottomTexture;

        /*! left edge section texture
         */
        iTexturePtr _selectorLeftEdgeTexture;

        /*! right edge section texture
         */
        iTexturePtr _selectorRightEdgeTexture;

        /*! top edge section texture
         */
        iTexturePtr _selectorTopEdgeTexture;

        /*! bottom edge section texture
         */
        iTexturePtr _selectorBottomEdgeTexture;

        /*! current splitter state
         */
        iSplitterState _splitterState = iSplitterState::Inactive;

        /*! saving mouse pos when last time pressed a button
         */
        iaVector2f _lastMousePos;

        /*! simulating drag drop
        */
        iWidgetID _draggedWidgetID = iWidget::INVALID_WIDGET_ID;

        /*! if true draw/handle the overlay
        */
        bool _activeOverlay = false;

        /*! if true display cross
        */
        bool _displayCross = false;

        /*! if true center button is displayed
        */
        bool _displayCenter = false;

        /*! if true display edges
        */
        bool _displayEdges = false;

        /*! updates size based on it's content
         */
        void calcMinSize() override;

        /*! calculates childrens offsets relative to their parent

        \param[out] offsets vector to be filled with childrens offsets
        */
        void calcChildOffsets(std::vector<iaRectanglef> &offsets) override;

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyDown(iEventMouseKeyDown &event) override;

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        bool onMouseKeyUp(iEventMouseKeyUp &event) override;

        /*! handles incoming mouse move events

        \param event mouse move event
        */
        void onMouseMove(iEventMouseMove &event) override;

        /*! simulates drop of widget id

        \param widgetID the widget to be dropped on this splitter
        */
        void simulateDrop(iWidgetID widgetID);

        /*! draws overlay
         */
        void drawOverlay() override;

        /*! draws the widget
         */
        void draw() override;

        /*! called once per frame
         */
        void onUpdate() override;

        /*! loads some resources
         */
        void loadResources();

        /*! \returns splitter state based on mouse pos

        \param pos the mouse pos
        */
        iSplitterState calcSplitterState(const iaVector2f &pos);

        /*! trying to merge splitter widgets

        \param splitter the curent splitter
        */
        void tryMerge(iWidgetSplitter* splitter);

        /*! reverses order of children 
        */
        void reverseChildren();

        /*! updates cursor based on splitter state

        \param splitterState the given splitter state
        */
        void updateCursor(iSplitterState splitterState);

    };

    /*! widget button pointer definition
     */
    typedef iWidgetSplitter *iWidgetSplitterPtr;
} // namespace igor

#endif // __IGOR_WIDGET_SPLITTER__
