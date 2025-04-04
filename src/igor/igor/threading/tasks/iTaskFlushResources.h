//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __IGOR_TASKFLUSHRESOURCES__
#define __IGOR_TASKFLUSHRESOURCES__

#include <igor/threading/tasks/iTask.h>

namespace igor
{

    /*! this task triggers the resource manager flush function repeatedly

    when ever there was a resource requested this task get's it actually loaded from disk

    will be added to the task manager after window was opened
    */
    class iTaskFlushResources : public iTask
    {

    public:
        /*! initializes member variables

        \param window window connected to render context
        */
        iTaskFlushResources(iWindowPtr window);

        /*! does nothing
         */
        virtual ~iTaskFlushResources() = default;

        /*! aborts the task
         */
        void abort() override;

    protected:
        /*! runs the task
         */
        void run() override;
    };

}; // namespace igor

#endif // __IGOR_TASKFLUSHRESOURCES__