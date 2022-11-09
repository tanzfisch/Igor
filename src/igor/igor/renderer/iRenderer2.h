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
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/simulation/iParticleSystem2D.h>

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
        friend class iWindow;

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

        /*! set projection matrix with perspective projection

        \param fov field of view in degrees
        \param aspect aspect ratio of screen
        \param nearplane near plane distance
        \param farplane far plane distance
        */
        void setPerspective(float32 fov, float32 aspect, float32 nearplane, float32 farplane);

        /*! set projection matrix with orthogonal projection

        \param left left bounding
        \param right right bounding
        \param bottom bottom bounding
        \param top top bounding
        \param nearplane near plane distance
        \param farplane far plane distance
        */
        void setOrtho(float32 left, float32 right, float32 bottom, float32 top, float32 nearplane, float32 farplane);

        /*! sets the model matrix

        \param matrix matrix to set the model matrix
        */
        void setModelMatrix(const iaMatrixd &matrix);

        /*! \returns current model matrix
         */
        const iaMatrixd &getModelMatrix() const;

        /*! \returns current view matrix
         */
        const iaMatrixd &getViewMatrix() const;

        /*! \returns current projection matrix
         */
        const iaMatrixd &getProjectionMatrix() const;

        /*! \returns current model view matrix
         */
        const iaMatrixd &getModelViewMatrix() const;

        /*! \returns current model view projection matrix
         */
        const iaMatrixd &getModelViewProjectionMatrix() const;

        void drawPoint(float32 x, float32 y, const iaColor4f &color = iaColor4f::white);
        void drawPoint(const iaVector2f &v, const iaColor4f &color = iaColor4f::white);
        void drawPoint(const iaVector3f &v, const iaColor4f &color = iaColor4f::white);

        void drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color = iaColor4f::white);
        void drawLine(const iaVector2f &v1, const iaVector2f &v2, const iaColor4f &color = iaColor4f::white);
        void drawLine(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color = iaColor4f::white);

        void drawLineStrip(const std::vector<iaVector3f> &points, const iaColor4f &color = iaColor4f::white);

        void drawRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color = iaColor4f::white);
        void drawRectangle(const iaRectanglef &rect, const iaColor4f &color = iaColor4f::white);

        void drawFilledRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color = iaColor4f::white);
        void drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color = iaColor4f::white);

        void drawTexturedRectangle(float32 x, float32 y, float32 width, float32 height, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f::white);
        void drawTexturedRectangle(const iaRectanglef &rect, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f::white);

        void drawQuad(const iaMatrixf &matrix, const iaColor4f &color = iaColor4f::white);

        void drawTexturedQuad(const iaMatrixf &matrix, const iTexturePtr &texture, const iaVector2f &tiling = iaVector2f(1.0, 1.0), const iaColor4f &color = iaColor4f::white);
        void drawTexturedQuad(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iTexturePtr &texture, const iaVector2f &tiling, const iaColor4f &color = iaColor4f::white);

        /*! draw specified frame from given atlas
        */
        void drawFrame(const iaMatrixf &matrix, const iAtlasPtr& sprite, uint32 frameIndex, const iaColor4f &color = iaColor4f::white);

        void drawParticles(iParticle2DPtr particles, int32 particleCount, const iTexturePtr &texture, const iaGradientColor4f &gradient);

        void drawString(float32 x, float32 y, const iaString &text, iHorizontalAlignment horz, iVerticalAlignment vert, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);
        void drawString(float32 x, float32 y, const iaString &text, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);

        

        /*! sets line render width

        \param lineWidth line render width
        */
        void setLineWidth(float32 lineWidth);

        /*! \returns line width
         */
        float32 getLineWidth() const;

        /*! sets point size

        \param pointSize point size
        */
        void setPointSize(float32 pointSize);

        /*! \returns point size
         */
        float32 getPointSize() const;

        /*! sets ignore render order flag

        If render order is ignored performance might increase. This might make sense if you know what you are doing.

        \param ignoreRenderOrder if true render order will be ignored
        */
        void setIgnoreRenderOrder(bool ignoreRenderOrder = true);

        /*! \returns true if render order is kept
         */
        bool isIgnoringRenderOrder();

        /*! sets font to use

        \param font the font to use
        */
        void setFont(const iTextureFontPtr &font);

        /*! \returns current font in use
         */
        const iTextureFontPtr &getFont() const;

        /*! sets font size to use

        \param fontSize the font size to use
        */
        void setFontSize(float32 fontSize);

        /*! \returns the font size
         */
        float32 getFontSize() const;

        /*! sets font line height

        \param lineHeight the font line height
        */
        void setFontLineHeight(float32 lineHeight);

        /*! \returns the font line height
         */
        float32 getFontLineHeight() const;

        /*! render statistics definition
        */
        struct iRendererStats
        {
            uint32 _vertices;
            uint32 _triangles;
            uint32 _indices;
            uint32 _drawCalls;
        };

        /*! \returns copy of render stats
        */
        iRendererStats getStats() const;

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

        /*! flushes textured quads queue
         */
        void flushTexQuads();

        /*! flushes quads queue
         */
        void flushQuads();

        /*! flushes lines queue
         */
        void flushLines();

        /*! flushes points queue
         */
        void flushPoints();

        /*! flushes last used queue
         */
        void flushLastUsed();

        /*! updats internal matrices
         */
        void updateMatrices();

        /*! \returns current model view projection matrix with single precision
         */
        const iaMatrixf getMVP() const;

        /*! begin textured quad

        \param texture the texture to use
        */
        int32 beginTexturedQuad(const iTexturePtr &texture);

        /*! end textured quad
        */
        void endTexturedQuad();

        /*! clears stats
        */
        void clearStats();
    };

}

#endif // __IGOR_RENDERER_2D__
