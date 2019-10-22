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

#ifndef __iACTION_SIMPLE_DELEGATE__
#define __iACTION_SIMPLE_DELEGATE__

#include <iAction.h>

#include <iaDelegate.h>
using namespace IgorAux;

namespace Igor
{
    
    /*! simple delegate with no parameters
    */
    iaDELEGATE(iSimpleDelegate, void, (), ());

	/*! action class providing a delegate interface
	*/
	class Igor_API iActionSimpleDelegate : public iAction
	{

		friend class iActionManager;

	public:

		/*! init members
		*/
        iActionSimpleDelegate(iSimpleDelegate actionDelegate);

		/*! does nothing
		*/
		virtual ~iActionSimpleDelegate() = default;

		/*! executed when action gets triggered
		*/
		void execute() override;

	private:

        /*! the action delegate to trigger with this action
        */
        iSimpleDelegate _actionDelegate;

	};

    /*! action delegate pointer definition
    */
    typedef iActionSimpleDelegate* iActionDelegatePtr;

}

#endif