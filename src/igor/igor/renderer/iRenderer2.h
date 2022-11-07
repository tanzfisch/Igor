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

#include <igor/renderer/buffers/iVertexArray.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/texture/iAtlas.h>

#include <iaux/data/iaColor4.h>
#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaVector2.h>
#include <iaux/math/iaVector3.h>

namespace igor
{

    class iRendererData;

    /*! this will eventually replace iRenderer
     */
    class iRenderer2 : public iModule<iRenderer2>
    {

        friend class iModule<iRenderer2>;

    public:

        /*! initializes renderer
         */
        void init();

        /*! cleanup renderer
         */
        void deinit();

        /*! draws everything that is still in the queue
         */
        void flush();

        /*! 

        \param ignoreRenderOrder if true render order will be ignored
        */
        void setIgnoreRenderOrder(bool ignoreRenderOrder = true);

        /*! \returns true if render order is kept
         */
        bool isIgnoringRenderOrder();

        void drawPoint(float32 x, float32 y, const iaColor4f &color);
        void drawPoint(const iaVector2f &v, const iaColor4f &color);
        void drawPoint(const iaVector3f &v, const iaColor4f &color);

        void drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color);
        void drawLine(const iaVector2f &v1, const iaVector2f &v2, const iaColor4f &color);
        void drawLine(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color);

        void drawRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color);
        void drawRectangle(const iaRectanglef &rect, const iaColor4f &color);

        void drawFilledRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color);
        void drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color);

        void drawTexturedRectangle(float32 x, float32 y, float32 width, float32 height, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f(1.0, 1.0, 1.0, 1.0));
        void drawTexturedRectangle(const iaRectanglef &rect, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f(1.0, 1.0, 1.0, 1.0));

        void drawQuad(const iaMatrixf &matrix, const iaColor4f &color);
        void drawTexturedQuad(const iaMatrixf &matrix, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f(1.0, 1.0, 1.0, 1.0));

        // void drawSprite(const iaMatrixf &matrix, const iAtlasPtr sprite, uint32 frameIndex, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f(1.0, 1.0, 1.0, 1.0));

        void setLineWidth(float32 lineWidth);
        float32 getLineWidth() const;

        void setPointSize(float32 pointSize);
        float32 getPointSize() const;

    private:        

        /*! internal render data
        */
        std::unique_ptr<iRendererData> _data;

        /*! init
        */
        iRenderer2();

        /*! deinit 
        */
        ~iRenderer2();    

        void flushTexQuads();
        void flushQuads();
        void flushLines();
        void flushPoints();

        void flushLastUsed();
    };

}

#endif // __IGOR_RENDERER_2D__