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

#ifndef __iOSEVENTLISTENER__
#define __iOSEVENTLISTENER__



#ifdef __IGOR_WIN__
#include <windows.h>
#endif

namespace Igor
{

	class iWindow;

	/*! classes that need operating system messages to work with derive from this class

	currently the window registers all known listeners when opened
	*/
	class Igor_API iOSEventListener
	{
		friend class iWindow;
#ifdef __IGOR_WIN__
		friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

	private:

		/*! called by an os event like key down or close window

		\param data contains the platformspecific data
		\return false if your implementation didn't handle the event
		*/
		virtual bool onOSEvent(const void *data) = 0;

		/*! does what ever necessairy to get the device running
		*/
		virtual bool registerDevice() = 0;

	protected:

		/*! pointer to the parent window
		*/
		iWindow* _window;

		/*! dtor
		*/
		virtual ~iOSEventListener()
		{
		};

    public:

        /*! \retruns pointer to corresponding window
        */
        iWindow* getWindow()
        {
            return _window;
        }
	};

};

#endif