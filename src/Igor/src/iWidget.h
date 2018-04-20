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
// (c) Copyright 2012-2018 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iWIDGET__
#define __iWIDGET__

#include <iMouse.h>
#include <iStopWatch.h>

#include <iaVector4.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{
	class iFont;
	class iWidgetBaseTheme;
	class iWidgetManager;

    /*! interaction state of widget
    */
	enum class iWidgetAppearanceState
	{
        Highlighted,
        Pressed,
        Clicked,
        DoubleClicked,
		Context,
        Standby
	};

    class iWidget;

    /*! widget click event
    */
    iaEVENT(iClickEvent, iClickDelegate, void, (iWidget* source), (source));

    /*! mouse off click event

    so when there was a click outside the range of a widget
    */
    iaEVENT(iMouseOffClickEvent, iMouseOffClickDelegate, void, (iWidget* source), (source));
    
    /*! context menu event
    */
    iaEVENT(iContextMenuEvent, iContextMenuDelegate, void, (iWidget* source), (source));

    /*! wheel up event
    */
    iaEVENT(iWheelUpEvent, iWheelUpDelegate, void, (iWidget* source), (source));
    
    /*! wheel down event
    */
    iaEVENT(iWheelDownEvent, iWheelDownDelegate, void, (iWidget* source), (source));

    /*! double click event
    */
    iaEVENT(iDoubleClickEvent, iDoubleClickDelegate, void, (iWidget* source), (source));

    /*! mouse over event
    */
	iaEVENT(iMouseOverEvent, iMouseOverDelegate, void, (iWidget* source), (source));

    /*! mouse off event
    */
	iaEVENT(iMouseOffEvent, iMouseOffDelegate, void, (iWidget* source), (source));

    /*! change event
    */
	iaEVENT(iChangeEvent, iChangeDelegate, void, (iWidget* source), (source));

    /*! keyboard focus changed event
    */
	iaEVENT(iFocusEvent, iFocusDelegate, void, (iWidget* source), (source));

    /*! selection changed event
    */
    iaEVENT(iSelectionChangedEvent, iSelectionChangedDelegate, void, (int32 index), (index));

    /*! GUI widget base class

    coordinatesystems origin within widgets is the upper left corner with x positive to the right and y positive down

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    \todo to many friends
    \todo fix one click and double click handling
    \todo maybe we should have a widget base class on top so we can treat widgets usercontrols and dialogs more equaly
    */
	class Igor_API iWidget
	{

        /*! needs to be friend because of private constructor
        */
		friend class iWidgetManager;

		/*! has to be friend so it can manipulate actual size and position

		\todo maybe design problem
		*/
		friend class iDialog; 

		/*! has to be friend so it can fake actual size and position for it's children
		*/
		friend class iWidgetGrid;

		/*! has to be friend so it can fake actual size and position for it's children
		*/
		friend class iWidgetScroll;

		friend class iWidgetGroupBox;
		
		friend class iWidgetCheckBox;
		friend class iWidgetNumberChooser;
        friend class iWidgetPicture;
        friend class iWidgetTextEdit;
        friend class iWidgetSlider;

	public:

        /*! invalid widget ID
        */
        static const uint64 INVALID_WIDGET_ID = 0;

        /*! \returns true if widget has keyboard focus
        */
        __IGOR_INLINE__ bool hasKeyboardFocus();

        /*! \returns current interaction state of widget
        */
        __IGOR_INLINE__ iWidgetAppearanceState getAppearanceState();

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

        /*! \returns actual absolute horizontal position
        */
        __IGOR_INLINE__ int32 getActualPosX() const;

        /*! \returns actual absolute vertical position
        */
        __IGOR_INLINE__ int32 getActualPosY() const;

        /*! \returns actual width
        */
        __IGOR_INLINE__ int32 getActualWidth() const;

        /*! \returns actual height
        */
        __IGOR_INLINE__ int32 getActualHeight() const;

        /*! \returns actual relative horizontal position
        */
        __IGOR_INLINE__ int32 getRelativePosX() const;

        /*! \returns actual relative vertical position
        */
        __IGOR_INLINE__ int32 getRelativePosY() const;

        /*! \returns minimum width
        */
        __IGOR_INLINE__ int32 getMinWidth() const;

        /*! \returns minimum height
        */
        __IGOR_INLINE__ int32 getMinHeight() const;

		/*! \returns actual width
		*/
		__IGOR_INLINE__ int32 getConfiguredWidth() const;

		/*! \returns actual height
		*/
		__IGOR_INLINE__ int32 getConfiguredHeight() const;

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
        __IGOR_INLINE__ bool isVisible();

        /*! \returns true if active aka enabled
        */
		__IGOR_INLINE__ bool isActive();

        /*! set widget visible

        \parma visible boolean to set is visible or invisible
        */
		void setVisible(bool visible = true);

        /*! set widget active aka enabled

        \param active boolean to set is active or inactive
        */
        void setActive(bool active = true);

        /*! \returns id of widget
        */
        __IGOR_INLINE__ uint64 getID();

        /*! \returns id of parenting widget
        */
        __IGOR_INLINE__ uint64 getParentID();

        /*! \returns true if has parent
        */
        __IGOR_INLINE__ bool hasParent();

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
        virtual void addWidget(iWidget* widget);

        /*! removes a child widget frmo this widget

        \param widget the child widget to be removed
        */
        virtual void removeWidget(iWidget* widget);

        /*! adds a child widget to this widget by id

        \param id the widget id
        */        
        virtual void addWidget(uint64 id);

        /*! removed a child widget from this widget by id

        \param id the widget id
        */
        virtual void removeWidget(uint64 id);

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

	protected:

        /*! if true this widget will process mouse clicks outside of the widgets boundings
        */
        bool _acceptOutOfBoundsClicks = false;

        /*! list of children
        */
        vector<iWidget*> _children;

        /*! flag if widget accepts drop
        */
        bool _acceptDrop = false;		

        /*! true: if currently mouse is over widget
        */
        bool _isMouseOver = false;

		/*! configured width of the widget
		*/
		int32 _configuredWidth = 100;

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

        /*! if true widget will react on mouse wheel
        */
        bool _reactOnMouseWheel = true;

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
        virtual void handleMouseMove(const iaVector2i& pos);

        /*! handles incomming acsii codes from keyboard

        \param c the incomming character from keyboard
        */
        virtual bool handleASCII(uint8 c);

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
		void setParent(iWidget* parent);

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

		/*! initializes members
		*/
		iWidget();

		/*! clean up
		*/
		virtual ~iWidget();

    protected:

        /*! \returns last mouse position
        */
        iaVector2i getLastMousePos() const;

	private:

        /*! min size to make also children fit in
        */
        int32 _minWidth = 0;

        /*! min size to make also children fit in
        */
        int32 _minHeight = 0;

		/*! actual (or rendered) width of the widget
		*/
		int32 _actualWidth = 0;

		/*! actual (or rendered) height of the widget
		*/
		int32 _actualHeight = 0;

		/*! horizontal position of the widget relative to parent
		*/
		int32 _relativeX = 0;

		/*! vertical position of the widget relative to parent
		*/
		int32 _relativeY = 0;

		/*! absolute horizontal position of the widget
		*/
		int32 _absoluteX = 0;

		/*! absolute vertical position of the widget
		*/
		int32 _absoluteY = 0;

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

		/*! grow by content flag
		*/
		bool _growsByContent = true;

        /*! last mouse position
        */
        iaVector2i _posLast;

        /*! here you get the next id from
        */
        static uint64 _nextID;

        /*! id of widget
        */
        uint64 _id = 0;

        /*! pointer to parent widget
        */
		iWidget* _parent = nullptr;

        /*! flag if widget is active
        */
		bool _active = true;

        /*! flag if widget id visible
        */
		bool _visible = true;

        /*! horizontal alignment relative to parent
        */
		iHorizontalAlignment _horizontalAlignment = iHorizontalAlignment::Center;
        
        /*! vertical alignment relative to parent
        */
        iVerticalAlignment _verticalAlignment = iVerticalAlignment::Center;

        /*! current widget state
        */
        iWidgetAppearanceState _widgetAppearanceState = iWidgetAppearanceState::Standby;

        /*! pointer to widget that owns the keyboard focus
        */
		static iWidget* _keyboardFocus;

        /*! updates size based on widgets content

        all widgets have to derive from this
        */
        virtual void calcMinSize() = 0;

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
        virtual void calcChildOffsets(vector<iRectanglei>& offsets);

	};

#include <iWidget.inl>

}

#endif

