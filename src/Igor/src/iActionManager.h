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
// (c) Copyright 2012-2019 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iACTIONMANAGER__
#define __iACTIONMANAGER__

#include <iDefines.h>

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <unordered_map>

namespace Igor
{

	class iAction;

    /*! action manager class
    */
    class Igor_API iActionManager : public iaSingleton<iActionManager>
    {

		friend class iaSingleton<iActionManager>;

    public:

		/*! registers an aciton to the manager

		if it's name is not unique it will not be added

		registering an action passes ownership to iActionManager

		\param action the action to be registered
		*/
		void registerAction(const iaString& name, iAction* action);

		/*! unregister action from manager by instance

		\param action the action to unregister
		*/
		void unregisterAction(const iAction* action);

		/*! unregister action by name

		\param name the name of the action to unregister
		\returns pointer to unregistered action bbecause ownership was passed to caller
		*/
		iAction* unregisterAction(const iaString& name);

		/*! \returns the action by name

		\param name the action name
		*/
		iAction* getAction(const iaString& name) const;

	private:

		/*! map of all actions
		*/
		std::unordered_map<int64, iAction*> _actions;

		/*! does nothing
		*/
		iActionManager() = default;

		/*! clean up
		*/
		~iActionManager();

    };

}

#endif