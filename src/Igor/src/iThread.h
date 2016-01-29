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

#ifndef __iTHREAD__
#define __iTHREAD__

#include <iDefines.h>

#include <iaDelegate.h>
using namespace IgorAux;

#include <thread>
using namespace std;

namespace Igor
{

	class iThreadHandle;
    class iThread;

    /*! thread delegate calls the actuall function run by this thread
    */
    iaDELEGATE(ThreadDelegate, void, (iThread* thread), (thread));

    /*! the state a thread is currently in
    */
    enum class iThreadState
    {
        Init,
        Running,
        Deinit,
        Done
    };

    /*! basic thread
    */
	class iThread
	{

		friend void* threadFunc(void* data);

	private:

        iThreadState _currentState = iThreadState::Init;

        /*! thread handle
        */
        thread* _thread = nullptr;

        /*! the delegate to be called by the thread
        */
		ThreadDelegate _threadDelegate;

	protected:

        /*! init function will be called as first by the thread
        */
		virtual void init();

        /*! deinit function is called last by the thread. right before join
        */
		virtual void deinit();		

	public:

        /*! \returns true: if thread is initialized; false: if not
        */
        iThreadState getState();

        /*! does nothing
        */
		iThread() = default;

        /*! delete thread handle
        */
		virtual ~iThread();

        /*! start thread with a specified delegate

        \param threadDelegate the specified delegate 
        */
		void run(ThreadDelegate threadDelegate);

        /*! waits for the thread to finish
        */
        void join();
        
	};

};

#endif
