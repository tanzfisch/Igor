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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IAUX_THREAD_H__
#define __IAUX_THREAD_H__

#include <iaux/iaDefines.h>
#include <iaux/system/iaDelegate.h>
#include <iaux/data/iaIDGenerator.h>

#include <thread>

namespace iaux
{

    class iaThread;

    /*! thread delegate calls the actuall function run by this thread
    */
    iaDELEGATE(ThreadDelegate, void, (iaThread * thread), (thread));

    /*! the state a thread is currently in
    */
    enum class iaThreadState
    {
        Init,
        Running,
        Deinit,
        Done
    };

    /*! basic thread
    */
    class IAUX_API iaThread
    {

        friend void *threadFunc(void *data);

    public:
        /*! does nothing
        */
        iaThread();

        /*! delete thread handle
        */
        virtual ~iaThread();

        /*! \returns true: if thread is initialized; false: if not
        */
        iaThreadState getState() const;

        /*! start thread with a specified delegate

        \param threadDelegate the specified delegate
        */
        void run(ThreadDelegate threadDelegate);

        /*! waits for the thread to finish
        */
        void join();

        /*! \returns igor aux thread id
        */
        iaID32 getID() const;

    protected:
        /*! init function will be called as first by the thread
        */
        virtual void init();

        /*! deinit function is called last by the thread. right before join
        */
        virtual void deinit();

    private:
        /*! thread id
        */
        iaID32 _id = 0;

        /*! the next node id
        */
        static iaIDGenerator32 _idGenerator;

        /*! current state of this thread
        */
        iaThreadState _currentState = iaThreadState::Init;

        /*! thread handle
        */
        std::thread *_thread = nullptr;

        /*! the delegate to be called by the thread
        */
        ThreadDelegate _threadDelegate;
    };

}; // namespace iaux

#endif
