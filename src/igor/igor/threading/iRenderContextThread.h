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

#ifndef __IGOR_RENDERCONTEXTTHREAD__
#define __IGOR_RENDERCONTEXTTHREAD__

#include <igor/threading/iThread.h>
#include <igor/system/iWindow.h>

namespace igor
{

    class iWindow;

    /*! thread that in addition has a render context
    */
    class iRenderContextThread : public iThread
    {

    public:
        /*! creates render context and starts sharing lists with parent thread
        */
        iRenderContextThread(iWindowPtr window);

        /*! does nothing
        */
        virtual ~iRenderContextThread() = default;

        /* \returns true if the context was correctly created and initialized
        */
        bool isValid();

    protected:
        /*! init render context
        */
        void init();

        /*! deletes render context
        */
        void deinit();

    private:
        /*! pointer to window to get the device context from
        */
        iWindowPtr _window = nullptr;

        /*! handle to render context
        */
        iRenderContextPtr _renderContext = nullptr;

        /*! was this thread correctly created
        */
        bool _isValid = false;
    };

}; // namespace igor

#endif
