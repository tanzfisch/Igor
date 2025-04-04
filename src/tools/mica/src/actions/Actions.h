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

#ifndef ACTIONS_H
#define ACTIONS_H

#include "MicaActionContext.h"

/*! copy node action
*/
class ActionCopyNode : public iAction
{

public:
    /*! init members
    */
    ActionCopyNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! paste node action
*/
class ActionPasteNode : public iAction
{

public:
    /*! init members
    */
    ActionPasteNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! cut node action
*/
class ActionCutNode : public iAction
{

public:
    /*! init members
    */
    ActionCutNode();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! add transformation action
*/
class ActionAddTransform : public iAction
{

public:
    /*! init members
    */
    ActionAddTransform();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! add group action
*/
class ActionAddGroup : public iAction
{

public:
    /*! init members
    */
    ActionAddGroup();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! add switch action
*/
class ActionAddSwitch : public iAction
{

public:
    /*! init members
    */
    ActionAddSwitch();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! add emitter action
*/
class ActionAddEmitter : public iAction
{

public:
    /*! init members
    */
    ActionAddEmitter();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! add particle system action
*/
class ActionAddParticleSystem : public iAction
{

public:
    /*! init members
    */
    ActionAddParticleSystem();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;
};

/*! bake mesh to world
*/
class ActionBakeMeshToWorld : public iAction
{

public:
    /*! init members
    */
    ActionBakeMeshToWorld();

    /*! executed when action gets triggered

    \param context the context the action was called with
    */
    void execute(const iActionContext &context) override;

    /*! \returns true if this action will execute with given context

	\param context the context the action was called with
    */
    bool isCompatible(const iActionContext &context) override;

private:
    /*! bakes a mesh node to world

    \param meshNode the given mesh node
    \param root the root of the mica scene
    */
    void bakeToWorld(iNodeMeshPtr meshNode, iNodePtr root);
};

#endif // ACTIONS_H