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

#ifndef __iDEFINES__
#define __iDEFINES__

#include <iaDefines.h>

#ifdef __IGOR_MSCOMPILER__
	#ifdef __IGOR_BUILDING_DLL__
		#define Igor_API __declspec(dllexport)
		#define Igor_API_Template __declspec(dllexport)
	#else
		#define Igor_API __declspec(dllimport)
		#define Igor_API_Template
	#endif
#endif

#ifdef __IGOR_DEBUG__
    #define __IGOR_CONFIG_STR__ debug
#else
    #define __IGOR_CONFIG_STR__ release
#endif

#ifdef __IGOR_X64__
    #define __IGOR_BIT_STR__ x64
#endif

#ifdef __IGOR_X32__
    #define __IGOR_BIT_STR__ x32
#endif

/*! default igor window title
*/
#define IGOR_DFAULT_WINDOW_TITLE TEXT("#Igor")

namespace Igor
{

    /*! horizontal alignment relative to parent widget
    */
    enum class iHorizontalAlignment
    {
        Left,
        Center,
        Right,
        Absolut,
        Strech, // currently only supported by dialogs
        Undefined
    };

    /*! vertical alignment relative to parent widget
    */
    enum class iVerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Absolut,
        Strech, // currently only supported by dialogs
        Undefined
    };

    /*! color format of textures pixmaps etc.
    */
    enum class iColorFormat
    {
        Undefined,
        RGB,
        RGBA,
        RED,
        GREEN,
        BLUE,
        ALPHA,
        DEPTH,
        BGR,
        BGRA
    };

    /*! how to generate a texture
    */
    enum class iTextureBuildMode
    {
        Normal,
        Mipmapped
    };

    /*! texture wrap mode
    */
    enum class iTextureWrapMode
    {
        Repeat,
        Clamp,
        MirrorRepeat
    };

    /*! cache mode for resources
    */
    enum class iResourceCacheMode
    {
        Free = 0,       //! free immidiatly after not in use
        Cache = 1,      //! free when cache is flushed
        Keep = 2        //! keep until corresponding resource manager was destroyed
    };

};

#endif
