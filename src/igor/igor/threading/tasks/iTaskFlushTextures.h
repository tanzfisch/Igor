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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iTASKFLUSHTEXTURES__
#define __iTASKFLUSHTEXTURES__

#include <igor/threading/tasks/iTask.h>

namespace Igor
{

    /*! this task triggers the texture resource manager flush function repedatly

    when ever there was a texture requested this task get's it actually loaded from disk

    \see iTextureResourceFactory
    */
    class Igor_API iTaskFlushTextures : public iTask
    {

    public:
        /*! initializes member variables

        \param window window connected to render context
        */
        iTaskFlushTextures(iWindow *window);

        /*! does nothing
        */
        virtual ~iTaskFlushTextures() = default;

        /*! aborts the task
        */
        void abort();

    protected:
        /*! runs the task
        */
        void run();
    };

}; // namespace Igor

#endif
