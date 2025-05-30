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

#ifndef IGOR_TASK_WATCH_FILESYSTEM_H
#define IGOR_TASK_WATCH_FILESYSTEM_H

#include <igor/threading/tasks/iTask.h>

#include <iaux/system/iaDirectory.h>

#include <unordered_map>

namespace igor
{

    /*! task to watch changes in filesystem
     */
    class iTaskWatchFilesystem : public iTask
    {

    public:
        /*! initializes member variables

        \param path the path to watch recursively
        \param recursive if true watch recursively
        */
        iTaskWatchFilesystem(const iaString &path, bool recursive);

        /*! does nothing
         */
        virtual ~iTaskWatchFilesystem() = default;

        /*! aborts the task
         */
        void abort() override;

    protected:
        /*! path to watch
        */
        iaString _path;

        /*! if true watch path recursively
        */
        bool _recursive;

        /*! subdirectories to watch
        */
        std::unordered_map<int, iaString> _subdirs;

        /*! if true this task keeps running
        */
        bool _running;

        /*! inotify handle
        */
        int _inotify;

        /*! runs the task
         */
        void run() override;

        /*! watch given directory for changes

        \param dir the given directory
        */
        int watchDir(const iaDirectory &dir);

        /*! handle created directory event

        \param dir the created directory
        \returns true if successful
        */
        bool onWatchDir(const iaString &dir);
    };

}; // namespace igor

#endif // IGOR_TASK_WATCH_FILESYSTEM_H