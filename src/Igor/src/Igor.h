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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __IGOR__
#define __IGOR__

#include <iDefines.h>

/*! Igor namespace that contains all high level functionality of Igor
*/
namespace Igor
{
	/*! should be called before any other interface of Igor is used	

	\param argc command line parameter count
	\param argv command line parameters
	*/
	void Igor_API startup(int argc, wchar_t** argv);

	/*! alternative startup
	*/
	void Igor_API startup();

	/*! should be called last in your application. do not use any Igor interface after that
	
	Shuts down all the singleton instances.
	Not thread save. make shure only one thread is left when calling this.

    \todo it would be nice to have the order of shutting down singletons implicitly or configurable
	*/
	void Igor_API shutdown();

}

#endif
