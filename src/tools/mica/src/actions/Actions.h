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

#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include <igor/ui/actions/iAction.h>
using namespace igor;

#if 0
/*! copy node action
*/
class Igor_API ActionCopyNode : public iAction
{

public:
    /*! init members
    */
    ActionCopyNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! paste node action
*/
class Igor_API ActionPasteNode : public iAction
{

public:
    /*! init members
    */
    ActionPasteNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};
#endif

/*! delete node action
*/
class Igor_API ActionDeleteNode : public iAction
{

public:
    /*! init members
    */
    ActionDeleteNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! registers mica actions to action manager
*/
void registerMicaActions();

#endif // __ACTIONS_H__
