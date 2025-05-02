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
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_WIDGET_H
#define IGOR_WIDGET_H

#include <igor/iDefines.h>
#include <igor/system/iMouse.h>
#include <igor/system/iTimerHandle.h>
#include <igor/events/iEventMouse.h>
#include <igor/events/iEventKeyboard.h>

#include <iaux/math/iaVector4.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaIDGenerator.h>
using namespace iaux;

#include <set>
#include <vector>
#include <any>

namespace igor
{

    class iWidgetManager;
    class iWidget;
    class iDrag;

    /*! define pointer to widget
     */
    typedef iWidget *iWidgetPtr;

    /*! widget id definition
     */
    typedef uint64 iWidgetID;

    /*! widget click event
     */
    IGOR_EVENT_DEFINITION(iClick, const iWidgetPtr);

    /*! mouse off click event

    so when there was a click outside the range of a widget
    */
    IGOR_EVENT_DEFINITION(iMouseOffClick, const iWidgetPtr);

    /*! context menu event
     */
    IGOR_EVENT_DEFINITION(iContextMenu, const iWidgetPtr);

    /*! wheel up event
     */
    IGOR_EVENT_DEFINITION(iWheelUp, const iWidgetPtr);

    /*! wheel down event
     */
    IGOR_EVENT_DEFINITION(iWheelDown, const iWidgetPtr);

    /*! double click event
     */
    IGOR_EVENT_DEFINITION(iDoubleClick, const iWidgetPtr);

    /*! mouse over event
     */
    IGOR_EVENT_DEFINITION(iMouseOver, const iWidgetPtr);

    /*! mouse off event
     */
    IGOR_EVENT_DEFINITION(iMouseOff, const iWidgetPtr);

    /*! change event
     */
    IGOR_EVENT_DEFINITION(iChange, const iWidgetPtr);

    /*! keyboard focus changed event
     */
    IGOR_EVENT_DEFINITION(iFocus, const iWidgetPtr);

    /*! selection changed event
     */
    IGOR_EVENT_DEFINITION(iSelectionChanged, const iWidgetPtr);

    /*! interaction state of widget
     */
    enum class iWidgetState
    {
        /*! widget it highlighted
         */
        Highlighted,

        /*! widget if currently pressed and held down
         */
        Pressed,

        /*! widget was just released from clicking it
         */
        Clicked,

        /*! widget was double clicked
         */
        DoubleClicked,

        /*! the widget does nothing except existing
         */
        Standby
    };

    /*! types of igor widgets
     */
    enum class iWidgetType
    {
        iWidgetButton,
        iWidgetCheckBox,
        iWidgetColor,
        iWidgetColorGradient,
        iWidgetGraph,
        iWidgetGroupBox,
        iWidgetLabel,
        iWidgetMenu,
        iWidgetMenuBar,
        iWidgetNumberChooser,
        iWidgetPicture,
        iWidgetScroll,
        iWidgetSelectBox,
        iWidgetSlider,
        iWidgetSpacer,
        iWidgetLineTextEdit,
        iWidgetTextEdit,
        iWidgetSplitter,
        iWidgetViewport,

        iWidgetGridLayout,
        iWidgetFixedGridLayout,
        iWidgetBoxLayout,
        iWidgetDockingLayout,

        iUserControl,
        iUserControlColor,
        iUserControlFile,
        iUserControlTexture,
        iUserControlMaterial,
        iUserControlShaderMaterial,
        iUserControlTreeView,
        iUserControlVector,
        iUserControlMeshReference,

        iDialog,
        iDialogColorChooser,
        iDialogColorGradient,
        iDialogDecisionBox,
        iDialogFileSelect,
        iDialogGraph,
        iDialogIndexMenu,
        iDialogMenu,
        iDialogMessageBox
    };

    /*! stream operator

    \param stream the destination
    \param widgetType the widget type to stream
    \returns the resulting stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iWidgetType &widgetType);

    /*! defines the kind of widget
     */
    enum class iWidgetKind
    {
        Widget,
        UserControl,
        Layout,
        Dialog
    };

    /*! GUI widget base class

    coordinate systems origin within widgets is the upper left corner with x positive to the right and y positive down

    \todo to many friends
    \todo fix one click and double click handling
    \todo maybe we should have a widget base class on top so we can treat widgets user controls and dialogs more equally
    \todo replace relative and actual pos variables with two rectangles
    */
    class IGOR_API iWidget
    {
        // iWidget has sooo many friends <3
        friend class iWidgetManager;
        friend class iDialog;
        friend class iWidgetGridLayout;
        friend class iWidgetScroll;
        friend class iWidgetGroupBox;
        friend class iWidgetCheckBox;
        friend class iWidgetNumberChooser;
        friend class iWidgetPicture;
        friend class iWidgetLineTextEdit;
        friend class iWidgetTextEdit;
        friend class iWidgetSlider;
        friend class iWidgetSplitter;

    public:
        /*! invalid widget ID
         */
        static const iWidgetID INVALID_WIDGET_ID = 0;

        /*! \returns the widgets type
         */
        iWidgetType getWidgetType() const;

        /*! \return the widgets kind
         */
        iWidgetKind getWidgetKind() const;

        /*! sets the z value which determines the render order of siblings

        0 is furthest in the front

        \param zvalue the z value to set
        */
        void setZValue(uint32 zvalue);

        /*! \returns the z value of this widget

        0 is furthest in the front
         */
        uint32 getZValue() const;

        /*! sets background color

        \param color the new background color
        */
        void setBackground(const iaColor4f &color);

        /*! \returns the background color
         */
        const iaColor4f &getBackground() const;

        /*! sets foreground color

        \param color the new foreground color
        */
        void setForeground(const iaColor4f &color);

        /*! \returns the foreground color
         */
        const iaColor4f &getForeground() const;

        /*! @returns true if events on this widget are blocked
         */
        bool isBlocked() const;

        /*! \returns true if widget has keyboard focus
         */
        bool hasKeyboardFocus() const;

        /*! \returns current interaction state of widget
         */
        iWidgetState getState() const;

        /*! \returns click event
         */
        iClickEvent &getClickEvent();

        /*! \returns mouse off click event
         */
        iMouseOffClickEvent &getMouseOffClickEvent();

        /*! \returns double click event
         */
        iDoubleClickEvent &getDoubleClickEvent();

        /*! \returns mouse over event
         */
        iMouseOverEvent &getMouseOverEvent();

        /*! \returns mouse off event
         */
        iMouseOffEvent &getMouseOffEvent();

        /*! \returns change event
         */
        iChangeEvent &getChangeEvent();

        /*! \returns focus event
         */
        iFocusEvent &getFocusEvent();

        /*! \returns wheel up event
         */
        iWheelUpEvent &getWheelUpEvent();

        /*! \returns wheel down event
         */
        iWheelDownEvent &getWheelDownEvent();

        /*! \returns context menu event
        */
        iContextMenuEvent& getContextMenuEvent();

        /*! \returns selection changed events
        */
        iSelectionChangedEvent& getSelectionChangedEvent();

        /*! \returns actual absolute position
         */
        iaVector2f getActualPos() const;

        /*! \returns actual absolute horizontal position
         */
        int32 getActualPosX() const;

        /*! \returns actual absolute vertical position
         */
        int32 getActualPosY() const;

        /*! \returns actual width
         */
        int32 getActualWidth() const;

        /*! \returns actual height
         */
        int32 getActualHeight() const;

        /*! \returns actual rectangle (absolute and in pixel)
         */
        iaRectanglef getActualRect() const;

        /*! \returns actual relative to it's parent horizontal position
         */
        int32 getRelativePosX() const;

        /*! \returns actual relative to it's parent vertical position
         */
        int32 getRelativePosY() const;

        /*! \returns actual minimum width

        this can vary based on min size of it's children
         */
        int32 getMinWidth() const;

        /*! \returns actual minimum height

        this can vary based on min size of it's children
         */
        int32 getMinHeight() const;

        /*! \returns configured minimum width
         */
        int32 getConfiguredMinWidth() const;

        /*! \returns configured minimum height
         */
        int32 getConfiguredMinHeight() const;

        /*! sets the configured minimum width

        \param width the minimum width defined
        */
        void setMinWidth(int32 width);

        /*! sets the configured minimum height

        \param height the minimum height defined
        */
        void setMinHeight(int32 height);

        /*! sets minimum width and height of the widget

        \param width the minimum width defined
        \param height the minimum height defined
        */
        void setMinSize(int32 width, int32 height);

        /*! sets if the widget accepts mouse clicks outside of the widget's area

        \param acceptOutOfBoundsClick if true mouse clicks outside will result in iMouseOffClickEvent
        */
        void setAcceptOutOfBoundsClicks(bool acceptOutOfBoundsClick = true);

        /*! \returns true if out of bound mouse clicks will be handeled
         */
        bool getAcceptOutOfBoundsClicks() const;

        /*! \returns horizontal alignment relative to parent widget
         */
        iHorizontalAlignment getHorizontalAlignment() const;

        /*! \returns vertical alignment relative to parent widget
         */
        iVerticalAlignment getVerticalAlignment() const;

        /*! set horizontal alignment relative to parent widget

        \param horizontalAlignment the horizontal alignment
        */
        void setHorizontalAlignment(iHorizontalAlignment horizontalAlignment);

        /*! set vertical alignment relative to parent widget

        \param verticalAlignment the horizontal vertical
        */
        void setVerticalAlignment(iVerticalAlignment verticalAlignment);

        /*! \returns true if visible
         */
        bool isVisible() const;

        /*! \returns true if enabled
         */
        bool isEnabled() const;

        /*! set widget visible

        \parma visible boolean to set is visible or invisible
        */
        void setVisible(bool visible = true);

        /*! set widget enabled

        \param enabled if true widget is enabled
        */
        void setEnabled(bool enabled = true);

        /*! \returns id of widget
         */
        iWidgetID getID() const;

        /*! \returns id of parenting widget
         */
        iWidgetID getParentID() const;

        /*! \returns parenting widget
         */
        iWidgetPtr getParent() const;

        /*! \returns true if mouse over widget
         */
        bool isMouseOver();

        /*! sets the drag accept flag

        \param acceptDrag if true widget accepts drag
        */
        void setAcceptDrag(bool acceptDrag);

        /*! \returns true if widget accepts to be dragged
         */
        bool isAcceptingDrag();

        /*! sets the drop accept flag

        \param acceptDrop if true widget accepts drop from drag
        */
        void setAcceptDrop(bool acceptDrop);

        /*! \returns true: widget accepts drop from drag; false: it does not accept drop
         */
        bool isAcceptingDrop();

        /*! \returns true if has parent
         */
        bool hasParent() const;

        /*! sets the grow by content flag

        \param grow if true the widget will grow if it's content is bigger than the configured size
        */
        void setGrowingByContent(bool grow = true);

        /*! \returns if the widget grows by content
         */
        bool isGrowingByContent() const;

        /*! \returns an information string about this widget
         */
        iaString getInfo() const;

        /*! \returns the widget that is currently in focus
         */
        static iWidgetPtr getKeyboardFocusWidget();

        /*! sets the tooltip text

        \param text the tooltip text
        */
        void setTooltip(const iaString &text);

        /*! \returns tooltip text
         */
        iaString getTooltip() const;

        /*! \returns children of this widget
         */
        const std::vector<iWidgetPtr> &getChildren() const;

        /*! clears the widget back to default
         */
        virtual void clear();

        /*! sets wether or not this widget ignores if a child already consumed an event

        \param value if true this widget ignores if a child already has consumed an event
        */
        void setIgnoreChildEventConsumption(bool value = true);

        /*! \returns true if this widget ignores if a child already has consumed an event
         */
        bool isIgnoringChildEventConsumption() const;

        /*! \returns the root widget which owns this widget

        returns nullptr if there is no parent
        */
        iWidgetPtr getRoot();

        /*! blocks all outgoing events from this widget
         */
        virtual void blockEvents();

        /*! unblocks all outgoing events from this widget
         */
        virtual void unblockEvents();

        /*! if true this widget will additionally be called during overlay processing

        Overlay means that after all widgets are processed/drawn there is a second round of processing/drawing.
        This can be used to handle mouse inputs when the widget is not visible. Or to draw on top of widgets that are in front of this widget.

        \param overlay if true widget is part of overlay
        */
        void setOverlayEnabled(bool overlay);

        /*! \returns true if overlay is enabled for this widget
         */
        bool isOverlayEnabled() const;

        /*! draws the widget
         */
        virtual void draw();

        /*! draws overlay of the widget

        Called after all widgets are drawn
        Only called if widget has overlay enabled
         */
        virtual void drawOverlay();

        /*! adds a child widget to this widget

        \param widget the child widget to be added
        */
        virtual void addWidget(iWidgetPtr widget);

        /*! removes a child widget from this widget

        \param widget the child widget to be removed
        */
        virtual void removeWidget(iWidgetPtr widget);

        /*! sets cursor type

        \param cursorType the cursor type to set
        */
        void setCursor(iMouseCursorType cursorType);

        /*! sets user data

        \param userData pointer to user data
        */
        void setUserData(const std::any &userData);

        /*! \returns pointer to user data
         */
        std::any getUserData() const;

        /*! sets widget selectable

        \param selectable if true widget is selectable
        */
        void setSelectable(bool selectable);

        /*! \returns true if selectable
         */
        bool isSelectable() const;

        /*! sets multi selection for children

        \param enabled if true multiple children can be selected at the same time
        */
        void setMultiSelection(bool enabled);

        /*! \returns true if multi selection of children is enabled
         */
        bool isMultiSelectionEnabled() const;

        /*! selects or unselects widget
        */
        void setSelect(bool select);

        /*! \returns true if widget is selected
         */
        bool isSelected() const;

        /*! clear selection of children
         */
        void clearSelection();

        /*! \returns list of selected child widgets
         */
        const std::vector<iWidgetPtr> getSelection() const;

        /*! sets selected children

        \param selection list of children to select
        */
        void setSelection(const std::vector<iWidgetPtr> &selection);

        /*! queue this widget for refresh in next frame
         */
        void refresh();

    protected:
        /*! if true widget is selected
         */
        bool _selected = false;

        /*! if true widget is selectable
         */
        bool _isSelectable = false;

        /*! if true multi selection is enabled
         */
        bool _isMultiSelectionEnabled = false;

        /*! list of children
         */
        std::vector<iWidgetPtr> _children;

        /*! configured width of the widget
         */
        int32 _configuredMinWidth = 0;

        /*! configured height of the widget
         */
        int32 _configuredMinHeight = 0;

        /*! click event
         */
        iClickEvent _click;

        /*! mouse off click event
         */
        iMouseOffClickEvent _mouseOffClick;

        /*! context menu event
         */
        iContextMenuEvent _contextMenu;

        /*! double click event
         */
        iDoubleClickEvent _doubleClick;

        /*! mouse over event
         */
        iMouseOverEvent _mouseOver;

        /*! mouse off event
         */
        iMouseOffEvent _mouseOff;

        /*! content changed event
         */
        iChangeEvent _change;

        /*! got keyboard focus event
         */
        iFocusEvent _focus;

        /*! wheel up event
         */
        iWheelUpEvent _wheelUp;

        /*! wheel down event
         */
        iWheelDownEvent _wheelDown;

        /*! selection changed event
         */
        iSelectionChangedEvent _selectionChanged;

        /*! tooltip text
         */
        iaString _tooltip;

        /*! position for the tooltip to appear
         */
        iaVector2f _tooltipPos;

        /*! tooltip timer
         */
        iaTime _tooltipTime = iaTime(0);

        /*! trigger tooltip
         */
        bool _initTooltip = false;

        /*! z value of this widget
         */
        uint32 _zValue = 0;

        /*! if true widget will react on mouse wheel
         */
        bool _reactOnMouseWheel = true;

        /*! if true events on this widget are blocked
         */
        bool _blockedEvents = false;

        /*! if true this widget will process mouse clicks outside of the widgets boundings
         */
        bool _acceptOutOfBoundsClicks = false;

        /*! flag if widget accepts drop
         */
        bool _acceptDrop = false;

        /*! if true widget accepts to be dragged
         */
        bool _acceptDrag = false;

        /*! true: if currently mouse is over widget
         */
        bool _isMouseOver = false;

        /*! if true this widget ignores if a child already has consumed an event
         */
        bool _ignoreChildEventConsumption = false;

        /*! last mouse position
         */
        iaVector2f _lastMousePos;

        /*! mouse position when last time pressed
         */
        iaVector2f _lastMousePressPos;

        /*! if true widget will be refreshed next frame
         */
        bool _needRefresh = true;

        /*! removes and deletes all children

        this will also cause all children deleting their children and so on
        */
        void clearChildren();

        /*! sets the widget's min size
         */
        void updateMinSize(int32 width, int32 height);

        /*! set/reset parent of widget

        \param parent pointer to parent. if set to nullptr caller must retain ownership
        */
        void setParent(iWidgetPtr parent);

        /*! sets the keyboard focus to this widget
         */
        void setKeyboardFocus();

        /*! resets the keyboard focus
         */
        void resetKeyboardFocus();

        /*! sets client area. it's something like a padding but the parent defines it

        \param left left client area border
        \param right right client area border
        \param top top client area border
        \param bottom bottom client area border
        */
        void setClientArea(int32 left, int32 right, int32 top, int32 bottom);

        /*! \returns last mouse position
         */
        const iaVector2f &getLastMousePos() const;

        /*! initializes members

        \param parent the optional parent
        */
        iWidget(iWidgetType type, iWidgetKind kind, const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        virtual ~iWidget();

        /*! handles incoming generic event

        \param event the event
        */
        virtual bool onEvent(iEvent &event);

        /*! handles incoming mouse wheel event

        \param event mouse wheel event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool onMouseWheel(iEventMouseWheel &event);

        /*! handles incoming mouse key down events

        \param event mouse key down event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool onMouseKeyDown(iEventMouseKeyDown &event);

        /*! handles incoming double click

        \param event the mouse double click event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool onMouseDoubleClick(iEventMouseKeyDoubleClick &event);

        /*! handles mouse key up events

        \param event the mouse key up event
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool onMouseKeyUp(iEventMouseKeyUp &event);

        /*! handles incoming mouse move events

        \param event mouse move event
        \param consumed if true mouse move was already consumed
        */
        virtual void onMouseMove(iEventMouseMove &event);

        /*! handles incoming acsii codes from keyboard

        \param event the key ascii event
        */
        virtual bool onASCII(iEventKeyASCII &event);

        /*! handles pressed key event

        \param event the key down event
        */
        virtual bool onKeyDown(iEventKeyDown &event);

        /*! handles released key event

        \param event the key up event
        */
        virtual bool onKeyUp(iEventKeyUp &event);

        /*! handles lost keyboard focus
         */
        virtual void onLostKeyboardFocus();

        /*! handles gained keyboard focus
         */
        virtual void onGainedKeyboardFocus();

        /*! updates size based on widgets content

        all widgets have to derive from this
        */
        virtual void calcMinSize();

        /*! updates widget alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        virtual void updateAlignment(int32 clientWidth, int32 clientHeight);

        /*! calculates childrens offsets relative to their parent

        Needs to be implemented for widgets with multiple children

        \param[out] offsets vector to be filled with childrens offsets
        */
        virtual void calcChildOffsets(std::vector<iaRectanglef> &offsets);

        /*! drag enter handle

        \param drag the drag data
        */
        virtual void onDragEnter(iDrag &drag);

        /*! drag move handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        virtual void onDragMove(iDrag &drag, const iaVector2f &mousePos);

        /*! drag leave handle

        \param drag the drag data
        */
        virtual void onDragLeave(iDrag &drag);

        /*! drop handle

        \param drag the drag data
        \param mousePos the current mouse pos
        */
        virtual void onDrop(const iDrag &drag, const iaVector2f &mousePos);

        /*! called when dragged
         */
        virtual void onDrag();

        /*! called when widget was queued for refresh in last frame
         */
        virtual void onRefresh();

    private:
        /*! the next node id
         */
        static iaIDGenerator64 _idGenerator;

        /*! pointer to widget that owns the keyboard focus
         */
        static iWidgetPtr _keyboardFocus;

        /*! the widgets type
         */
        iWidgetType _type;

        /*! the widgets kind
         */
        iWidgetKind _kind;

        /*! horizontal position of the widget relative to parent
         */
        int32 _relativeX = 0;

        /*! vertical position of the widget relative to parent
         */
        int32 _relativeY = 0;

        /*! min size to make also children fit in
         */
        int32 _minWidth = 0;

        /*! min size to make also children fit in
         */
        int32 _minHeight = 0;

        /*! absolute horizontal position of the widget
         */
        int32 _absoluteX = 0;

        /*! absolute vertical position of the widget
         */
        int32 _absoluteY = 0;

        /*! actual (or rendered) width of the widget
         */
        int32 _actualWidth = 0;

        /*! actual (or rendered) height of the widget
         */
        int32 _actualHeight = 0;

        /*! padding left for internal user only
         */
        int32 _clientAreaLeft = 0;

        /*! padding right for internal user only
         */
        int32 _clientAreaRight = 0;

        /*! padding top for internal user only
         */
        int32 _clientAreaTop = 0;

        /*! padding bottom for internal user only
         */
        int32 _clientAreaBottom = 0;

        /*! id of widget
         */
        iWidgetID _id = INVALID_WIDGET_ID;

        /*! pointer to parent widget
         */
        iWidgetPtr _parent = nullptr;

        /*! horizontal alignment relative to parent
         */
        iHorizontalAlignment _horizontalAlignment = iHorizontalAlignment::Center;

        /*! vertical alignment relative to parent
         */
        iVerticalAlignment _verticalAlignment = iVerticalAlignment::Center;

        /*! current widget state
         */
        iWidgetState _widgetState = iWidgetState::Standby;

        /*! grow by content flag
         */
        bool _growsByContent = true;

        /*! true if widget is enabled
         */
        bool _enabled = true;

        /*! true if widget id visible
         */
        bool _visible = true;

        /*! background color
         */
        iaColor4f _background = iaColor4f::transparent;

        /*! foreground color
         */
        iaColor4f _foreground = iaColor4f::black;

        /*! true if overlay is enabled
         */
        bool _overlay = false;

        /*! if true this widget can't take keyboard focus
         */
        bool _doNotTakeKeyboard = false;

        /*! user data
         */
        std::any _userData;

        /*! called when parent of this widget changes
         */
        virtual void onParentChanged();

        /*! called once per frame so a widget can update it's content if needed
         */
        virtual void onUpdate();

        /*! updates the absolute position

        \param offsetX absolute horizontal offset based on parents positions
        \param offsetY absolute vertical offset based on parents positions
        */
        virtual void updatePosition(int32 offsetX, int32 offsetY);
    };

#include <igor/ui/widgets/iWidget.inl>

} // namespace igor

#endif // IGOR_WIDGET_H