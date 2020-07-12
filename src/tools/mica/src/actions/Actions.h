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

#include "ActionContext.h"

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

/*! add transformation action
*/
class Igor_API ActionAddTransform : public iAction
{

public:
    /*! init members
    */
    ActionAddTransform();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! add group action
*/
class Igor_API ActionAddGroup : public iAction
{

public:
    /*! init members
    */
    ActionAddGroup();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! add switch action
*/
class Igor_API ActionAddSwitch : public iAction
{

public:
    /*! init members
    */
    ActionAddSwitch();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! add emitter action
*/
class Igor_API ActionAddEmitter : public iAction
{

public:
    /*! init members
    */
    ActionAddEmitter();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! add particle system action
*/
class Igor_API ActionAddParticleSystem : public iAction
{

public:
    /*! init members
    */
    ActionAddParticleSystem();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! add model action
*/
class Igor_API ActionAddModel : public iAction
{

public:
    /*! init members
    */
    ActionAddModel();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;
};

/*! registers mica actions to action manager
*/
void registerMicaActions();

#endif // __ACTIONS_H__
