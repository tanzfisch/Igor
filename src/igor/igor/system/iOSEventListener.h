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

#ifndef __IGOR_OSEVENTLISTENER__
#define __IGOR_OSEVENTLISTENER__

namespace igor
{
	class iWindow;
	typedef iWindow *iWindowPtr;

	/*! classes that need operating system messages to work with derive from this class

	currently the window registers all known listeners when opened
	*/
	class iOSEventListener
	{

		friend class iWindowImplLinux;
		friend class iWindowImplWindows;

	public:
		/*! does nothing
		*/
		iOSEventListener() = default;

		/*! does nothing
		*/
		virtual ~iOSEventListener() = default;

		/*! \retruns pointer to corresponding window
		if it returns nullptr the listener was not registred to a window yet
		*/
		virtual iWindowPtr getWindow() const = 0;

	private:
		/*! called by an os event like key down or close window

		\param data contains the platformspecific data
		\return false if your implementation didn't handle the event
		*/
		virtual bool onOSEvent(const void *data) = 0;

		/*! does what ever necessairy to get the device running

		\param window handle to the window
		\param data os specific data (see iOSEventregisterData)
		*/
		virtual bool initDevice(const void *data) = 0;

		/*! unregisters device from listening
		*/
		virtual void deinitDevice() = 0;
	};

}; // namespace igor

#endif // __IGOR_OSEVENTLISTENER__