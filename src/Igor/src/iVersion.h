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
// (c) Copyright 2012-2017 by Martin Loga
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

#ifndef __IGOR_VERSION_FILE__
#define __IGOR_VERSION_FILE__

#include <iDefines.h>

//! major version of Igor
#define __IGOR_VERSION_MAJOR__ 0
//! minor version of Igor
#define __IGOR_VERSION_MINOR__ 9
//! patch version of Igor
#define __IGOR_VERSION_PATCH__ 0

//! Igor configuration
#define __IGOR_CONFIGURATION__ STR(__IGOR_CONFIG_STR__) ", " STR(__IGOR_BIT_STR__)
//! Igor full version including revision
#define __IGOR_VERSION__ STR(__IGOR_VERSION_MAJOR__) "." STR(__IGOR_VERSION_MINOR__) "." STR(__IGOR_VERSION_PATCH__)

#endif