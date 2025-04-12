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

#ifndef IGOR_FILESYSTEM_ACTION_CONTEXT_H
#define IGOR_FILESYSTEM_ACTION_CONTEXT_H

#include <igor/ui/actions/context/iActionContext.h>
#include <iaux/data/iaString.h>

namespace igor
{

    /*! filesystem action context
     */
    class IGOR_API iFilesystemActionContext : public iActionContext
    {

    public:
        /*! init context

        \param entitySceneID the scene those entities are in
        \param entityIDs list of entity IDs to run the action on
        */
        iFilesystemActionContext(const iaString &path)
            : _path(path)
        {
        }

        const iaString &getPath() const
        {
            return _path;
        }

    private:
        /*! context path
         */
        iaString _path;
    };

}

#endif // IGOR_FILESYSTEM_ACTION_CONTEXT_H