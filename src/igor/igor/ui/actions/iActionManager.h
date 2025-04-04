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
// (c) Copyright 2012-2025 by Martin Loga
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

#ifndef IGOR_ACTIONMANAGER_H
#define IGOR_ACTIONMANAGER_H

#include <igor/ui/actions/iAction.h>
#include <igor/resources/module/iModule.h>

#include <unordered_map>

namespace igor
{

    /*! action manager class
    */
    class IGOR_API iActionManager : public iModule<iActionManager>
    {

        friend class iModule<iActionManager>;

    public:
        /*! registers an action to the manager

		if it's name is not unique it will not be added

		registering an action passes ownership to iActionManager

		\param action the action to be registered
		*/
        void registerAction(iActionPtr action);

        /*! unregister action from manager by instance

		unregistering passes ownership of action back to caller

		\param action the action to unregister
		*/
        void unregisterAction(const iActionPtr action);

        /*! \returns true if an action was registered to the action manager

        \param action the action to check
        */
        bool isRegistered(const iActionPtr action);

        /*! \returns true if an action was registered to the action manager

        \param actionName name of the action to check
        */
        bool isRegistered(const iaString &actionName);

        /*! \returns the action by name

		\param name the action name
		*/
        iActionPtr getAction(const iaString &name) const;

        /*! \returns the action by hash value

        \param hash the hash of the action name
        */
        iActionPtr getAction(const int64 hash) const;

    private:
        /*! map of all actions
		*/
        std::unordered_map<int64, iActionPtr> _actions;

        /*! registers actions
		*/
        iActionManager();

        /*! clean up
		*/
        ~iActionManager();

        /*! registers all internal actions
		*/
        void registerDefaultActions();
    };

} // namespace igor

#endif // IGOR_ACTIONMANAGER_H