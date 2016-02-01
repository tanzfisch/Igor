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

#ifndef __iDIALOGMANAGER__
#define __iDIALOGMANAGER__

#include <iDefines.h>
#include <iaSingleton.h>

#include <iWidget.h>

#include <vector>
#include <map>
using namespace std;

namespace Igor
{

	class iWidgetBaseTheme;
    class iWidgetDialog;

    /*! manages the widgets in use

    is also a factory that creates and destroys all the widgets

    Example:
    \ref Widgets/src/WidgetsExample.cpp "Widgets usage example"

    */
	class Igor_API iWidgetManager : public iaSingleton<iWidgetManager>
	{

        friend class iaSingleton<iWidgetManager>;

	public:
		
        /*! creates a widget of given type

        \param type type of widget to create
        */
		iWidget* createWidget(iWidgetType type);

        /*! destroyes widget

        \param widget the widget to destroy
        */
		void destroyWidget(iWidget* widget);

        /*! destroyes widget by id

        \param id id of the widget to be destroyed
        */
        void destroyWidget(uint64 id);

        /*! \returns widget by id

        \param id id of widget
        */
        iWidget* getWidget(uint64 id);

        /*! \returns the theme in use
        */
		iWidgetBaseTheme* getTheme();

        /*! sets the theme to use

        \param theme the theme to use
        */
		void setTheme(iWidgetBaseTheme *theme);

        /*! sets the desktop dimensions

        you should update this when window size changes

        \param width width of desktop
        \param height height of desktop
        */
		void setDesktopDimensions(uint32 width, uint32 height);

        /*! \returns dektop width
        */
		uint32 getDesktopWidth() const;

        /*! \returns dektop height
        */
        uint32 getDesktopHeight() const;

        /*! draws the widgets if theme is defined

        needs to be called by application

        make sure you have set up an orthogonal coordinate system with the same dimensions as set via setDesktopDimensions
        */
        void draw();

	private:

        /*! pointer to current theme
        */
		iWidgetBaseTheme* _currentTheme = nullptr;

        /*! list of all widgets
        */
		map<uint64, iWidget*> _widgets;

        /*! list of all dialogs
        */
        vector<iWidgetDialog*> _dialogs;

        /*! current desktop width
        */
		uint32 _desktopWidth = 0;

        /*! current desktop height
        */
		uint32 _desktopHeight = 0;

        /*! handle for mouse key down event

        \param key mouse key pressed
        */
        void onMouseKeyDown(iKeyCode key);

        /*! handle for mouse key up event

        \param key mouse key released
        */
        void onMouseKeyUp(iKeyCode key);

        /*! handle for mouse double click

        \param key contains which key was pressed
        */
        void onMouseDoubleClick(iKeyCode key);

        /*! handle for mouse move event

        \param x horrizontal mouse position
        \param y vertical mouse position
        */
        void onMouseMove(int32 x, int32 y);

        /*! handle for mouse wheel event

        \param d mouse wheel delta
        */
        void onMouseWheel(int32 d);

        /*! internal handler for incomming keyboard events
        */
        void onASCII(char c);

        /*! registers to mouse and keyboard evetns
        */
        void registerIOEvents();

        /*! unregister from mouse and keyboard evetns
        */
        void unregisterIOEvents();

        /*! updates all widgets

        \todo actually only the dialogs need to be updated here
        */
        void updateDialogs();

        /*! does nothing
        */
        iWidgetManager() = default;

        /*! checks for mem leaks and releases all left over widgets
        */
        virtual ~iWidgetManager();

	};

}

#endif
