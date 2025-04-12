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

#ifndef IGOR_ENTITY_ACTIONS_H
#define IGOR_ENTITY_ACTIONS_H

#include <igor/ui/actions/iAction.h>

namespace igor
{
    /*! set entity active action
     */
    class iActionSetEntityActive : public iAction
    {

    public:
        /*! init members
         */
        iActionSetEntityActive();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! set entity inactive action
     */
    class iActionSetEntityInactive : public iAction
    {

    public:
        /*! init members
         */
        iActionSetEntityInactive();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! delete entity action
     */
    class iActionDeleteEntity : public iAction
    {

    public:
        /*! init members
         */
        iActionDeleteEntity();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! copy entity to clipboard action
     */
    class iActionCopyEntity : public iAction
    {

    public:
        /*! init members
         */
        iActionCopyEntity();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! cut entity (to clipboard) action
     */
    class iActionCutEntity : public iAction
    {

    public:
        /*! init members
         */
        iActionCutEntity();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! paste entity from clipboard action
     */
    class iActionPasteEntity : public iAction
    {

    public:
        /*! init members
         */
        iActionPasteEntity();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

    /*! creates a new entity under a given entity
     */
    class iActionCreateEntity : public iAction
    {

    public:
        /*! init members
         */
        iActionCreateEntity();

        /*! executed when action gets triggered

        \param context the context the action was called with
        */
        void execute(const iActionContext &context) override;

        /*! \returns true if this action will execute with given context

        \param context the context the action was called with
        */
        bool isCompatible(const iActionContext &context) override;
    };

} // namespace igor

#endif // IGOR_ENTITY_ACTIONS_H