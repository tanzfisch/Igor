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
// (c) Copyright 2014-2015 by Martin Loga
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

#include <vector>
using namespace std;

namespace Igor
{
	class iFont;
	class iWidgetBaseTheme;
	class iWidgetManager;

    /*! horrizontal alignment relative to parent widget
    */
	enum class iHorrizontalAlignment
	{
		Left,
		Center,
		Right,
        Absolut,
		Undefined
	};

    /*! vertical alignment relative to parent widget
    */
    enum class iVerticalAlignment
	{
		Top,
		Center,
		Bottom,
        Absolut,
		Undefined
	};

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

    /*! type of widget. needed for factory functionality
    */
	enum class iWidgetType
	{
		Dialog,
		Label,
		Button,
		GroupBox,
		Grid,
		CheckBox,
		NumberChooser,
		TextEdit,
        Picture,
		Scroll,
        Spacer,
        ColorView,
        Slider,
		Undefined
    };

    class iWidget;

    iaEVENT(iClickEvent, iClickDelegate, void, (iWidget* source), (source));
    iaEVENT(iWheelUpEvent, iWheelUpDelegate, void, (iWidget* source), (source));
    iaEVENT(iWheelDownEvent, iWheelDownDelegate, void, (iWidget* source), (source));
    iaEVENT(iDoubleClickEvent, iDoubleClickDelegate, void, (iWidget* source), (source));
	iaEVENT(iMouseOverEvent, iMouseOverDelegate, void, (iWidget* source), (source));
	iaEVENT(iMouseOffEvent, iMouseOffDelegate, void, (iWidget* source), (source));
	iaEVENT(iChangeEvent, iChangeDelegate, void, (iWidget* source), (source));
	iaEVENT(iFocusEvent, iFocusDelegate, void, (iWidget* source), (source));

    /*! GUI widget base class

    coordinatesystems origin within widgets is the upper left corner with x positive to the right and y positive down

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    \todo to many friends
    \todo fix one click and double click handling
    */
	class Igor_API iWidget
	{

        /*! needs to be friend because of private constructor
        */
		friend class iWidgetManager;

		friend class iWidgetDialog; // todo why can't we access protected members of iWidget there?
		friend class iWidgetGroupBox;
		friend class iWidgetGrid;
		friend class iWidgetCheckBox;
		friend class iWidgetNumberChooser;
		friend class iWidgetScroll;
        friend class iWidgetButton;
        friend class iWidgetSpacer;
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

        /*! \returns actual horrizontal position
        */
        __IGOR_INLINE__ int32 getX();

        /*! \returns actual vertical position
        */
        __IGOR_INLINE__ int32 getY();

        /*! \returns actual width
        */
        __IGOR_INLINE__ int32 getWidth();

        /*! \returns actual height
        */
        __IGOR_INLINE__ int32 getHeight();

        /*! \returns type of widget
        */
        __IGOR_INLINE__ iWidgetType getType() const;

        /*! draws the widget on screen
        */
		virtual void draw();

        /*! \returns horrizontal alignment relative to parent widget
        */
		iHorrizontalAlignment getHorrizontalAlignment();

        /*! \returns vertical alignment relative to parent widget
        */
        iVerticalAlignment getVerticalAlignment();

        /*! set horrizontal alignment relative to parent widget

        \param horrizontalAlignment the horrizontal alignment
        */
        virtual void setHorrizontalAlignment(iHorrizontalAlignment horrizontalAlignment);

        /*! set vertical alignment relative to parent widget

        \param verticalAlignment the horrizontal vertical
        */
        virtual void setVerticalAlignment(iVerticalAlignment verticalAlignment);

        /*! set this widget exclusively modal
        */
        void setModal();

        /*! reset modal flag
        */
        void resetModal();

        /*! \returns true: if widget is modal
        */
        __IGOR_INLINE__ bool isModal();

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

	protected:

        vector<iWidget*> _widgets;

        /*! flag if widget accepts drop
        */
        bool _acceptDrop = false;

        /*! true: if currently mouse is over widget
        */
        bool _isMouseOver = false;

        /*! width of the widget
        */
		int32 _width = 100;

        /*! height of the widget
        */
		int32 _height = 20;

        /*! horrizontal position of the widget relative to parent
        */
		int32 _posx = 0;

        /*! vertical position of the widget relative to parent
        */
        int32 _posy = 0;

        /*! click event
        */
		iClickEvent _click;

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

        \param x horrizontal mouse position
        \param y horrizontal mouse position
        */
        virtual void handleMouseMove(int32 x, int32 y);

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

        /*! updates the widget
        */
		virtual void update() = 0;

        /*! updates the widget parent
        */
		void updateParent();

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

        /*! calculates absolute position and size based on parent position and size

        \param parentPosX horrizontal position of parent
        \param parentPosY vertical position of parent
        \param parentWidth width of parent
        \param parentHeight height of parent
        */
        void calcPosition(int32 parentPosX, int32 parentPosY, int32 parentWidth, int32 parentHeight);

		/*! initializes members

		\param widgetType the tpe of the widget created
		*/
		iWidget(iWidgetType widgetType);

		/*! clean up
		*/
		virtual ~iWidget();

	private:

        /*! here you get the next id from
        */
        static uint64 _nextID;

        /*! id of widget
        */
        uint64 _id = 0;

        /*! pointer to parent widget
        */
		iWidget* _parent = nullptr;

        /*! type of widget
        */
		iWidgetType _type = iWidgetType::Undefined;

        /*! flag if widget is active
        */
		bool _active = true;

        /*! flag if widget id visible
        */
		bool _visible = true;

        /*! horrizontal alignment relative to parent
        */
		iHorrizontalAlignment _horrizontalAlignment = iHorrizontalAlignment::Center;
        
        /*! vertical alignment relative to parent
        */
        iVerticalAlignment _verticalAlignment = iVerticalAlignment::Center;

        /*! current widget state
        */
        iWidgetAppearanceState _widgetAppearanceState = iWidgetAppearanceState::Standby;

        /*! pointer to widget that owns the keyboard focus
        */
		static iWidget* _keyboardFocus;

        /*! modal marker
        */
        static iWidget* _modal;

	};

#include <iWidget.inl>

}

#endif
