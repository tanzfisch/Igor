//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __OMPF_VERSION__
#define __OMPF_VERSION__

#include <ompfDefines.h>

#ifdef __DEBUG__
	#define OMPF_CONFIGURATION "debug"
#else
	#define OMPF_CONFIGURATION "release"
#endif

// Version Informations
#define OMPF_VERSION_MAJOR 1 // always the same number as the underlying OMPF definitions
#define OMPF_VERSION_MINOR 0
#define OMPF_VERSION_PATCH 0

#define STR2(x) #x
#define STR(x) STR2(x)

#define OMPF_VERSION STR(OMPF_VERSION_MAJOR) "." STR(OMPF_VERSION_MINOR) "." STR(OMPF_VERSION_PATCH)

#endif