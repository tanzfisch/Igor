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
// (c) Copyright 2014-2020 by Martin Loga
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

#ifndef __ACTIONCONTEXT__
#define __ACTIONCONTEXT__

#include "../outliner/Outliner.h"

/*! mica action context
*/
class MicaActionContext : public iActionContext
{

public:
    /*! init members
    */
    MicaActionContext(WorkspacePtr workspace, Outliner *outliner)
        : _workspace(workspace), _outliner(outliner)
    {
    }

    /*! \returns the workspace
    */
    WorkspacePtr getWorkspace() const
    {
        return _workspace;
    }

    /*! returns the outliner
    */
    Outliner *getOutliner() const
    {
        return _outliner;
    }

private:
    /*! the mica workspace
    */
    WorkspacePtr _workspace;

    /*! reference to the outliner
    */
    Outliner *_outliner = nullptr;
};

#endif // __ACTIONS__
