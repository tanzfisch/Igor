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

#ifndef IGOR_TASK_GENERATE_THUMBNAILS_H
#define IGOR_TASK_GENERATE_THUMBNAILS_H

#include <igor/threading/tasks/iTask.h>

namespace igor
{

    /*! task that generates thumbnails
     */
    class iTaskGenerateThumbnails : public iTask
    {

    public:
        /*! init
         */
        iTaskGenerateThumbnails(iWindowPtr window);

        /*! does nothing
         */
        virtual ~iTaskGenerateThumbnails() = default;

        /*! aborts the task
         */
        void abort() override;

    protected:
        /*! runs the task
         */
        void run() override;
    };

}
#endif // IGOR_TASK_GENERATE_THUMBNAILS_H