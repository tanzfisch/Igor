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

#ifndef __IAUX_THREAD__
#define __IAUX_THREAD__

#include <iaux/iaDefines.h>
#include <iaux/system/iaEvent.h>
#include <iaux/data/iaIDGenerator.h>
#include <iaux/data/iaString.h>

#include <thread>
#include <map>

namespace iaux
{

    class iaThread;

    /*! thread delegate calls the actual function run by this thread
     */
    typedef iaDelegate<void, iaThread *> iThreadCallbackDelegate;

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
        iaThread(const iaString &type = "");

        /*! delete thread handle
         */
        virtual ~iaThread();

        /*! \returns true: if thread is initialized; false: if not
         */
        iaThreadState getState() const;

        /*! start thread with a specified delegate

        \param threadDelegate the specified delegate
        */
        void run(iThreadCallbackDelegate threadDelegate);

        /*! waits for the thread to finish
         */
        void join();

        /*! \returns igor aux thread id
         */
        iaID32 getID() const;

        /*! \returns id of the thread this is running in
         */
        static iaID32 getThisThreadID();

        /*! \returns type of thread
         */
        const iaString &getType() const;        

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
        iaID32 _id = IGOR_INVALID_ID;

        /*! type
         */
        iaString _type;        

        /*! current state of this thread
         */
        iaThreadState _currentState = iaThreadState::Init;

        /*! thread handle
         */
        std::thread *_thread = nullptr;

        /*! the delegate to be called by the thread
         */
        iThreadCallbackDelegate _threadDelegate;

        /*! id generator
         */
        static iaIDGenerator32 _idGenerator;        

        /*! maps thread IDs to thread names
        */
        static std::map<size_t, iaID32> _threadIDs;

        /*! mutex to protect thread ids
        */
        static iaMutex _mutex;        
    };

}; // namespace iaux

#endif
