
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
// (c) Copyright 2012-2026 by Martin A. Loga
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

#ifndef IGOR_SHADER_UTILS_H
#define IGOR_SHADER_UTILS_H

#include <igor/resources/shader/iShader.h>
#include <igor/resources/material/iMaterial.h>

namespace igor
{

    class IGOR_API iShaderUtils
    {
    public:
        /*! generates a texture with a sphere using the given shader

        \param shader the given shader
        \param width width of texture
        \param height height of texture
        \returns generated pixmap
        */
        static iPixmapPtr shaderToPixmap(iShaderPtr shader, uint32 width, uint32 height);

        /*! generates a texture with a sphere using the given material

        \param material the given material
        \param width width of texture
        \param height height of texture
        \returns generated pixmap
        */
        static iPixmapPtr materialToPixmap(iMaterialPtr material, uint32 width, uint32 height);
    };
}

#endif // IGOR_SHADER_UTILS_H