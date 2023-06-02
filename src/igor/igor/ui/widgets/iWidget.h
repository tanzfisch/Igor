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

#ifndef __IGOR_WIDGET__
#define __IGOR_WIDGET__

#include <igor/iDefines.h>
#include <igor/system/iMouse.h>
#include <igor/system/iTimerHandle.h>

#include <iaux/math/iaVector4.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaIDGenerator.h>
using namespace iaux;

#include <set>
#include <vector>

namespace igor
{

    class iWidgetManager;
    class iWidget;

    /*! define pointer to widget
     */
    typedef iWidget *iWidgetPtr;

    /*! widget id definition
     */
    typedef uint64 iWidgetID;

    /*! widget click event
     */
    IGOR_EVENT_DEFINITION(iClick, void, const iWidgetPtr);

    /*! mouse off click event

    so when there was a click outside the range of a widget
    */
    IGOR_EVENT_DEFINITION(iMouseOffClick, void, const iWidgetPtr);

    /*! context menu event
     */
    IGOR_EVENT_DEFINITION(iContextMenu, void, const iWidgetPtr);

    /*! wheel up event
     */
    IGOR_EVENT_DEFINITION(iWheelUp, void, const iWidgetPtr);

    /*! wheel down event
     */
    IGOR_EVENT_DEFINITION(iWheelDown, void, const iWidgetPtr);

    /*! double click event
     */
    IGOR_EVENT_DEFINITION(iDoubleClick, void, const iWidgetPtr);

    /*! mouse over event
     */
    IGOR_EVENT_DEFINITION(iMouseOver, void, const iWidgetPtr);

    /*! mouse off event
     */
    IGOR_EVENT_DEFINITION(iMouseOff, void, const iWidgetPtr);

    /*! change event
     */
    IGOR_EVENT_DEFINITION(iChange, void, const iWidgetPtr);

    /*! keyboard focus changed event
     */
    IGOR_EVENT_DEFINITION(iFocus, void, const iWidgetPtr);

    /*! selection changed event
     */
    IGOR_EVENT_DEFINITION(iSelectionChanged, void, int32);

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
        iWidgetGrid,
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

        iUserControl,
        iUserControlAction,
        iUserControlColorChooser,
        iUserControlFileChooser,

        iDialog,
        iDialogColorChooser,
        iDialogColorGradient,
        iDialogDecisionBox,
        iDialogFileSelect,
        iDialogGraph,
        iDialogIndexMenu,
        iDialogMenu,
        iDialogMessageBox,

        iUserType = 100,
        iUndefinedType = 100
    };

    /*! defines the kind of widget
     */
    enum class iWidgetKind
    {
        Widget,
        UserControl,
        Dialog
    };

    /*! GUI widget base class

    coordinatesystems origin within widgets is the upper left corner with x positive to the right and y positive down

    \todo to many friends
    \todo fix one click and double click handling
    \todo maybe we should have a widget base class on top so we can treat widgets usercontrols and dialogs more equaly
    \todo replace relative and actual pos variables with two rectangles
    */
    class IGOR_API iWidget
    {
        // iWidget has sooo many friends <3
        friend class iWidgetManager;
        friend class iDialog;
        friend class iWidgetGrid;
        friend class iWidgetScroll;
        friend class iWidgetGroupBox;
        friend class iWidgetCheckBox;
        friend class iWidgetNumberChooser;
        friend class iWidgetPicture;
        friend class iWidgetLineTextEdit;
        friend class iWidgetTextEdit;
        friend class iWidgetSlider;

    public:
        /*! \returns the widgets type
         */
        iWidgetType getWidgetType() const;

        /*! \return the widgets kind
         */
        iWidgetKind getWidgetKind() const;

        /*! sets the z value which determines the render order of siglings

        \param zvalue the z value to set
        */
        void setZValue(int32 zvalue);

        /*! \returns the z value of this widget
         */
        int32 getZValue() const;

        /*! sets background color

        \param color the new background color
        */
        void setBackground(const iaColor4f &color);

        /*! \returns the background color
         */
        iaColor4f getBackground() const;

        /*! sets foreground color

        \param color the new foreground color
        */
        void setForeground(const iaColor4f &color);

        /*! \returns the foreground color
         */
        iaColor4f getForeground() const;

        /*! blocks all outgoing events from this widget
         */
        virtual void blockEvents();

        /*! unblocks all outgoing events from this widget
         */
        virtual void unblockEvents();

        /*! @returns true if events on this widget are blocked
         */
        bool isBlocked() const;

        /*! invalid widget ID
         */
        static const iWidgetID INVALID_WIDGET_ID = 0;

        /*! \returns true if widget has keyboard focus
         */
        bool hasKeyboardFocus() const;

        /*! \returns current interaction state of widget
         */
        iWidgetState getState() const;

        /*! registers delegate to click event (click is left mouse button)

        \param clickDelegate the delegate to register
        */
        void registerOnClickEvent(iClickDelegate clickDelegate);

        /*! registers delegate to mouse out of bounds click events

        \param clickDelegate the delegate to register
        */
        void registerOnMouseOffClickEvent(iMouseOffClickDelegate clickDelegate);

        /*! registers delegate to double click event

        \param doubleClickDelegate the delegate to register
        */
        void registerOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate);

        /*! registers delegate to mouse over event

        \param mouseOverDelegate the delegate to register
        */
        void registerOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate);

        /*! registers delegate to mouse off event (oposite of mouse over)

        \param mouseOffDelegate the delegate to register
        */
        void registerOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate);

        /*! registers delegate to content change event

        \param changeDelegate the delegate to register
        */
        void registerOnChangeEvent(iChangeDelegate changeDelegate);

        /*! registers delegate to keyboard focus event

        \param focusDelegate the delegate to register
        */
        void registerOnFocusEvent(iFocusDelegate focusDelegate);

        /*! registers delegate to wheel up event

        \param wheelUpDelegate the delegate to register
        */
        void registerOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate);

        /*! unregister delegate from wheel up event

        \param wheelUpDelegate the delegate to unregister
        */
        void unregisterOnWheelUpEvent(iWheelUpDelegate wheelUpDelegate);

        /*! registers delegate to wheel down event

        \param wheelDownDelegate the delegate to register
        */
        void registerOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate);

        /*! unregister delegate from wheel down event

        \param wheelDownDelegate the delegate to unregister
        */
        void unregisterOnWheelDownEvent(iWheelDownDelegate wheelDownDelegate);

        /*! unregisters delegate from click event (click is left mouse button)

        \param clickDelegate the delegate to unregister
        */
        void unregisterOnClickEvent(iClickDelegate clickDelegate);

        /*! unregisters delegate from mouse off click event

        \param clickDelegate the delegate to unregister
        */
        void unregisterOnMouseOffClickEvent(iMouseOffClickDelegate clickDelegate);

        /*! unregisters delegate from double click event

        \param doubleClickDelegate the delegate to unregister
        */
        void unregisterOnDoubleClickEvent(iDoubleClickDelegate doubleClickDelegate);

        /*! unregisters delegate mouse over event

        \param mouseOverDelegate the delegate to unregister
        */
        void unregisterOnMouseOverEvent(iMouseOverDelegate mouseOverDelegate);

        /*! unregisters delegate mouse off event

        \param mouseOffDelegate the delegate to unregister
        */
        void unregisterOnMouseOffEvent(iMouseOffDelegate mouseOffDelegate);

        /*! unregisters delegate content change event

        \param changeDelegate the delegate to unregister
        */
        void unregisterOnChangeEvent(iChangeDelegate changeDelegate);

        /*! unregisters delegate keyboard focus event

        \param focusDelegate the delegate to unregister
        */
        void unregisterOnFocusEvent(iFocusDelegate focusDelegate);

        /*! registers delegate to context menu event

        \param contextMenuDelegate the delegate to register
        */
        void registerOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate);

        /*! unregister delegate from context menu event

        \param contextMenuDelegate the delegate to unregister
        */
        void unregisterOnContextMenuEvent(iContextMenuDelegate contextMenuDelegate);

        /*! registers delegate to selection changed event

        \param delegate the delegate to register
        */
        void registerOnSelectionChangedEvent(iSelectionChangedDelegate delegate);

        /*! unregisters delegate from seleciton changed event

        \param delegate the delegate to unregister
        */
        void unregisterOnSelectionChangedEvent(iSelectionChangedDelegate delegate);

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

        /*! \returns actual rectangle
         */
        iaRectanglef getActualRect() const;

        /*! \returns actual relative horizontal position
         */
        int32 getRelativePosX() const;

        /*! \returns actual relative vertical position
         */
        int32 getRelativePosY() const;

        /*! \returns minimum width
         */
        int32 getMinWidth() const;

        /*! \returns minimum height
         */
        int32 getMinHeight() const;

        /*! \returns actual width
         */
        int32 getConfiguredWidth() const;

        /*! \returns actual height
         */
        int32 getConfiguredHeight() const;

        /*! sets the configured width

        \param width the width defined
        */
        void setWidth(int32 width);

        /*! sets the configured height

        \param height the height defined
        */
        void setHeight(int32 height);

        /*! sets width and height of the widget

        \param width the width defined
        \param height the height defined
        */
        void setSize(int32 width, int32 height);

        /*! sets if the widget accepts mouse clicks outside of the widget's area

        \param acceptOutOfBoundsClick if true mouse clicks outside will result in iMouseOffClickEvent
        */
        void setAcceptOutOfBoundsClicks(bool acceptOutOfBoundsClick = true);

        /*! \returns true if out of bound mouse clicks will be handeled
         */
        bool getAcceptOutOfBoundsClicks() const;

        /*! draws the widget
         */
        virtual void draw();

        /*! \returns horizontal alignment relative to parent widget
         */
        iHorizontalAlignment getHorizontalAlignment();

        /*! \returns vertical alignment relative to parent widget
         */
        iVerticalAlignment getVerticalAlignment();

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

        /*! \returns true if mouse over widget
         */
        bool isMouseOver();

        /*! sets the drop accept flag

        \param acceptDrop if true widget accepts drop from drag
        */
        void setAcceptDrop(bool acceptDrop);

        /*! \returns true: widget accepts drop from drag; false: it does not accept drop
         */
        bool isAcceptingDrop();

        /*! adds a child widget to this widget

        \param widget the child widget to be added
        */
        virtual void addWidget(iWidgetPtr widget);

        /*! removes a child widget frmo this widget

        \param widget the child widget to be removed
        */
        virtual void removeWidget(iWidgetPtr widget);

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

        /*! returns a list with children

        \param children[out] list with children
        */
        void getChildren(std::vector<iWidgetPtr> &children);

        /*! removes and deletes all children

        this will also cause all children deleting their children and so on
        */
        void clearChildren();

        /*! sets id this widget handles all events regardles whether or not its child already has handled the events

        \param value if true this widget handles all events
        */
        void setIgnoreChildEventHandling(bool value = true);

        /*! \returns true if event handling is done an any case
         */
        bool isIgnoringChildEventHandling() const;

        /*! \returns the root widget which owns this widget

        returns nullptr if there is no parent
        */
        iWidgetPtr getRoot();

    protected:
        /*! list of children
         */
        std::vector<iWidgetPtr> _children;

        /*! configured width of the widget
         */
        int32 _configuredWidth = 0;

        /*! configured height of the widget
         */
        int32 _configuredHeight = 20;

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

        /*! moouse over event
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

        /*! z value of this widget
         */
        int32 _zValue = 0;

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

        /*! true: if currently mouse is over widget
         */
        bool _isMouseOver = false;

        /*! if true this widget handles all events regardles whether or not its child already has handled the events
         */
        bool _ignoreChildEventHandling = false;

        /*! initializes members

        \param parent the optional parent
        */
        iWidget(iWidgetType type, iWidgetKind kind, const iWidgetPtr parent = nullptr);

        /*! clean up
         */
        virtual ~iWidget();

        /*! handles incomming mouse wheel event

        \param d mouse wheel delta
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool handleMouseWheel(int32 d);

        /*! handles incomming mouse key down events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool handleMouseKeyDown(iKeyCode key);

        /*! handles incomming double click

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool handleMouseDoubleClick(iKeyCode key);

        /*! handles mouse key up events

        \param key the key that was pressed
        \returns true: if event was consumed and therefore ignored by the parent
        */
        virtual bool handleMouseKeyUp(iKeyCode key);

        /*! handles incomming mouse move events

        \param pos mouse position
        */
        virtual void handleMouseMove(const iaVector2f &pos);

        /*! handles incomming acsii codes from keyboard

        \param c the incomming character from keyboard
        */
        virtual bool handleASCII(uint8 c);

        /*! handles pressed key event

        \param key the pressed key
        */
        virtual bool handleKeyDown(iKeyCode key);

        /*! handles released key event

        \param key the released key
        */
        virtual bool handleKeyUp(iKeyCode key);

        /*! handles lost keayboard focus
         */
        virtual void handleLostKeyboardFocus();

        /*! handles gained keayboard focus
         */
        virtual void handleGainedKeyboardFocus();

        /*! sets the widget's min size
         */
        void setMinSize(int32 width, int32 height);

        /*! set parent of widget

        \parem parent pointer to parent
        */
        void setParent(iWidgetPtr parent);

        /*! sets the keyboard focus to this widget
         */
        void setKeyboardFocus();

        /*! resets the keyboard focus
         */
        void resetKeyboardFocus();

        /*! sets client area. it's something like a margin but the parent defines it

        \param left left client area border
        \param right right client area border
        \param top top client area border
        \param bottom bottom client area border
        */
        void setClientArea(int32 left, int32 right, int32 top, int32 bottom);

        /*! \returns last mouse position
         */
        iaVector2f getLastMousePos() const;

    private:
        /*! the widgets type
         */
        iWidgetType _type;

        /*! the widgets kind
         */
        iWidgetKind _kind;

        /*! tooltip timer
         */
        iTimerHandle *_timerTooltip = nullptr;

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

        /*! margin left for internal user only
         */
        int32 _clientAreaLeft = 0;

        /*! margin right for internal user only
         */
        int32 _clientAreaRight = 0;

        /*! margin top for internal user only
         */
        int32 _clientAreaTop = 0;

        /*! margin bottom for internal user only
         */
        int32 _clientAreaBottom = 0;

        /*! last mouse position
         */
        iaVector2f _posLast;

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

        /*! the next node id
         */
        static iaIDGenerator64 _idGenerator;

        /*! pointer to widget that owns the keyboard focus
         */
        static iWidgetPtr _keyboardFocus;

        /*! handles tooltip timer

        \param time the time
        */
        void onToolTipTimer(const iaTime &time);

        /*! called when parent of this widget changes
         */
        virtual void onParentChanged();

        /*! destroy tooltip timer
         */
        void destroyTooltipTimer();

        /*! updates size based on widgets content

        all widgets have to derive from this
        */
        virtual void calcMinSize();

        /*! called once per frame so a widget can update it's content if needed
         */
        virtual void onHandle();

        /*! background color
         */
        iaColor4f _background;

        /*! foreground color
         */
        iaColor4f _foreground;

        /*! updates widget alignment

        \param clientWidth maximum width this widget can align to
        \param clientHeight maximum height this widget can align to
        */
        virtual void updateAlignment(int32 clientWidth, int32 clientHeight);

        /*! updates the absolute position

        \param offsetX absolute horizontal offset based on parents positions
        \param offsetY absolute vertical offset based on parents positions
        */
        void updatePosition(int32 offsetX, int32 offsetY);

        /*! calculates childrens ofsets relative to thair parent

        \param offsets vector to be filled with childrens offsets
        */
        virtual void calcChildOffsets(std::vector<iaRectanglef> &offsets);
    };

#include <igor/ui/widgets/iWidget.inl>

    /*! stream operator

    \param stream the destination
    \param widgetType the widget type to stream
    \returns the resulting stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iWidgetType &widgetType);

} // namespace igor

#endif // __IGOR_WIDGET__
