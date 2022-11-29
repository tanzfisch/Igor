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
// (c) Copyright 2012-2022 by Martin Loga
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

#ifndef __IGOR_RENDERER_DEFINES__
#define __IGOR_RENDERER_DEFINES__

#include <igor/iDefines.h>

namespace igor
{

    /*! definition of invalid buffer id
     */
    static constexpr const uint32 INVALID_BUFFER_ID = 0;

    /*! maximum texture units supported
     */
    static constexpr const int32 MAX_TEXTURE_UNITS = 8;

    /*! render target id definition
     */
    typedef uint32 iRenderTargetID;

    /*! definition of render target types
     */
    enum class iRenderTargetType
    {
        ToTexture,
        ToRenderBuffer
    };

    /*! the default render target ID
     */
    static const iRenderTargetID DEFAULT_RENDER_TARGET = 0;

}

#endif // __IGOR_RENDERER_DEFINES__