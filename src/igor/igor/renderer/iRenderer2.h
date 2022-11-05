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

#ifndef __IGOR_RENDERER_2D__
#define __IGOR_RENDERER_2D__

#include <igor/resources/material/iMaterial.h>
#include <igor/renderer/buffers/iVertexArray.h>

#include <iaux/data/iaColor4.h>
#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>

namespace igor
{

    /*! this will eventually replace iRenderer
     */
    class iRenderer2
    {
    public:
        static void init();
        static void deinit();

        static void flush();

        static void drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color);
        static void drawLine(const iaVector2f &v1, const iaVector2f &v2, const iaColor4f &color);
        static void drawLine(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color);

        static void drawRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color);
        static void drawRectangle(const iaRectanglef &rect, const iaColor4f &color);

        static void drawRectangleOnCenter(const iaVector2f &center, const iaVector2f &size, const iaColor4f &color);

    private:
        static void flushLines();
    };

}

#endif // __IGOR_RENDERER_2D__