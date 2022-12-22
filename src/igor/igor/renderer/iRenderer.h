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

#include <igor/data/iAABox.h>
#include <igor/data/iAACube.h>
#include <igor/renderer/buffers/iInstancingBuffer.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iTargetMaterial.h>

#include <iaux/data/iaColor4.h>
#include <iaux/data/iaRectangle.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaVector2.h>

#include <deque>

namespace igor
{

    class iRendererData;

    /*! renderer interface
     */
    class IGOR_API iRenderer : public iModule<iRenderer>
    {

        friend class iModule<iRenderer>;
        friend class iWindow;
        friend class iView;

    public:
        /*! initializes renderer
         */
        void init();

        /*! cleanup renderer
         */
        void deinit();

        /*! sets current material to render with

        \param material the material to render with
        */
        void setMaterial(const iMaterialPtr &material);

        /*! \returns currently active material
         */
        const iMaterialPtr &getMaterial() const;

        /*! \returns render hardware vendor
         */
        iaString getVendor();

        /*! \returns renderer type
         */
        iaString getRenderer();

        /*! \returns version of renderer
         */
        iaString getVersion();

        /*! \returns renderer extensions
         */
        iaString getExtensions();

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

        /*! sets the model matrix

        \param matrix matrix to set the model matrix
        */
        void setViewMatrix(const iaMatrixd &matrix);

        /*! sets view matrix from camera matrix

        \param camMatrix the camera position and orientation in world space
        */
        void setViewMatrixFromCam(const iaMatrixd &camMatrix);

        /*! sets projection matrix

        \param matrix the new projection matrix
        */
        void setProjectionMatrix(const iaMatrixd &matrix);

        /*! \returns camera matrix in world coordinates
         */
        const iaMatrixd &getCamMatrix() const;

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

        /*! sets a fallback texture if no other texture is available

        by default this is already set with a generated texture by the iTextureResourceFactory

        \param texture the texture to use
        */
        void setFallbackTexture(const iTexturePtr &texture);

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

        void drawTexturedRectangle(float32 x, float32 y, float32 width, float32 height, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2f &tiling = iaVector2f(1.0, 1.0));
        void drawTexturedRectangle(const iaRectanglef &rect, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2f &tiling = iaVector2f(1.0, 1.0));

        void drawQuad(const iaMatrixf &matrix, const iaColor4f &color = iaColor4f::white);
        void drawQuad(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iaColor4f &color = iaColor4f::white);
        void drawQuad(const iaVector3f &o, const iaVector3f &u, const iaVector3f &v, const iaColor4f &color = iaColor4f::white);

        void drawTexturedQuad(const iaMatrixf &matrix, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2f &tiling = iaVector2f(1.0, 1.0));
        void drawTexturedQuad(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2f &tiling = iaVector2f(1.0, 1.0));
        void drawTexturedQuad(const iaVector3f &o, const iaVector3f &u, const iaVector3f &v, iTexturePtr texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2f &tiling = iaVector2f(1.0, 1.0));

        /*! draw specified frame of given atlas
         */
        void drawFrame(const iaMatrixf &matrix, const iAtlasPtr &atlas, uint32 frameIndex, const iaColor4f &color = iaColor4f::white, bool blend = false);

        void drawString(float32 x, float32 y, const iaString &text, iHorizontalAlignment horz, iVerticalAlignment vert, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);
        void drawString(float32 x, float32 y, const iaString &text, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);

        /*! draw a circle.

        \param x horizontal center position
        \param y vertical center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        void drawCircle(float32 x, float32 y, float32 radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a filled circle.

        \param x horizontal center position
        \param y vertical center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        void drawFilledCircle(float32 x, float32 y, float32 radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        ///////////////////// 3D ////////////////////////////
        void drawBox(const iAACubed &box, const iaColor4f &color = iaColor4f::white);
        void drawBox(const iAACubef &box, const iaColor4f &color = iaColor4f::white);

        void drawBox(const iAABoxd &box, const iaColor4f &color = iaColor4f::white);
        void drawBox(const iAABoxf &box, const iaColor4f &color = iaColor4f::white);

        void drawMesh(iMeshPtr mesh, iTargetMaterialPtr targetMaterial);

        /*! draws buffer with given target material and primitive type

        \param vertexArray the buffer to draw
        \param primitiveType the given primitive type
        \param targetMaterial the target material (optional)
        */
        void drawBuffer(iVertexArrayPtr vertexArray, iRenderPrimitive primitiveType, iTargetMaterialPtr targetMaterial = nullptr);

        void drawBuffer(iInstancingBufferPtr instancingBuffer, iTargetMaterialPtr targetMaterial = nullptr);

        /////////////// LIGHT ///////////
        void setLightPosition(int32 lightnum, const iaVector3d &pos);
        void setLightAmbient(int32 lightnum, iaColor3f &ambient);
        void setLightDiffuse(int32 lightnum, iaColor3f &diffuse);
        void setLightSpecular(int32 lightnum, iaColor3f &specular);

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

        If render order is ignored the amount of draw calls will be reduced but this could cause massive artefacts.
        Only use this if you know what xou are doing.

        By default the renderer will always flush when the next render call changes the type of thing we are rendering.

        For example: Points to Lines or Buffers to Quads

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

        /*! \returns current viewport dimensions
         */
        const iaRectanglei &getViewport() const;

        /*! sets viewport dimensions

        \param viewport the new viewport
        */
        void setViewport(const iaRectanglei &viewport);

        /*! sets viewport dimensions

        \param x horizontal position
        \param y vertical position
        \param width width
        \param height height
        */
        void setViewport(int32 x, int32 y, int32 width, int32 height);

        /*! clears swtencil buffer with clear depth
         */
        void clearStencilBuffer(int32 index = 0);

        /*! clears color buffer with given color

        \param color the given clear color
         */
        void clearColorBuffer(const iaColor4f &color = iaColor4f::black);

        /*! clears depth buffer with given depth

        \param depth the given depth
         */
        void clearDepthBuffer(float32 depth = 1.0);

        /*! projects a screen position in to object space position

        \param screenpos the screen position (vertical origin is at top of screen)
        \param modelview the model view matrix
        \param projection the projection matrix
        \param viewport the viewport
        \returns world position of projection screen position
        */
        const iaVector3d unProject(const iaVector3d &screenpos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const;

        /*! projects a object space position in to screen space

        \param objectSpacePos the object space position to project
        \param modelview the model view matrix
        \param projection the projection matrix
        \param viewport the viewport
        \returns the screen position
        */
        const iaVector3d project(const iaVector3d &objectSpacePos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const;

        /*! enables/disables stencil test

        \param enable if true stencil test will be enabled
        */
        void setStencilTestActive(bool enable);

        bool isStencilTestActive() const;

        /*! stencil function types
         */
        enum class iStencilFunction
        {
            Never,
            Less,
            Equal,
            LessOrEqual,
            Greater,
            NotEqual,
            GreaterOrEqual,
            Always
        };

        /*! sets the stencil function

        \param function the stencil function (legal values are Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, and Always)
        \param ref the reference value
        \param mask the mask value to gate the result of the test
        */
        void setStencilFunction(iStencilFunction function, int32 ref, uint32 mask);

        /*! stencil operation types
         */
        enum class iStencilOperation
        {
            Keep,
            Zero,
            Replace,
            Increment,
            Decrement,
            Invert
        };

        /*! sets the stencil test actions

        legal values are Keep, Zero, Replace, Increment, IncrementWrap, Decrement, DecrementWrap, and Invert

        \param fail action when stencil test fails
        \param zfail action when stencil test passes but depth test fails
        \param zpass action when stencil and depth test passed
        */
        void setStencilOperation(iStencilOperation fail, iStencilOperation zfail, iStencilOperation zpass);

        /*! sets the stencil mask value
         */
        void setStencilMask(uint8 mask);

        void setDepthTestActive(bool enable);
        bool isDepthTestActive() const;

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
        const iRendererStats &getStats() const;

        /*! // TODO remove
        \todo make more private again maybe we need a class to handle buffers
        */
        void destroyBuffer(uint32 bufferID);

        // TODO
        /*! sets color Id to render with

        \param colorID next color ID to render with
        */
        void setColorID(uint64 colorID);

        /*! creates a render target

        \param width the width of the render target in pixel/texel
        \param height the height of the render target in pixel/texel
        \param format the color format of the render target
        \param renderTargetType the render target type
        \param useDepthBuffer if true render target is having a depth buffer
        \returns id of render target
        */
        iRenderTargetID createRenderTarget(uint32 width, uint32 height, iColorFormat format, iRenderTargetType renderTargetType, bool useDepthBuffer);

        /*! destroyes render target by id

        \param id the given render target id
        */
        void destroyRenderTarget(iRenderTargetID id);

        /*! sets the current render target by id

        \param id the render target id to be set current
        */
        void setRenderTarget(iRenderTargetID id = DEFAULT_RENDER_TARGET);

        /*! \returns id of current active render target
         */
        iRenderTargetID getRenderTarget() const;

        /*! reads rectangular area from screen buffer

        \param x horizontal position in pixel
        \parma y vertical position in pixel
        \param width width in pixel
        \param height height in pixel
        \param format color format
        \param data destination buffer to store the data in
        */
        void readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, uint8 *data);

        /*! sets globally forced wireframe mode

        \param active if true from now on everything is rendered in wireframe
        */
        void setWireframeEnabled(bool active);

        /*! \returns true if wireframe mode is on
         */
        bool isWireframeEnabled() const;

    private:
        /*! internal render data
         */
        std::unique_ptr<iRendererData> _data;

        /*! init
         */
        iRenderer();

        /*! deinit
         */
        ~iRenderer();

        /*! flushes textured quads queue
         */
        void flushTexQuads();

        /*! flushes triangles queue
         */
        void flushTriangles();

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

        /*! draws everything that is still in the queue
         */
        void flush();

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

        /*! begin adding triangles to the triangle queue
         */
        void beginTriangles();

        /*! end adding triangles to the triangle queue
         */
        void endTriangles();

        /*! called to start new frame
         */
        void beginFrame();

        /*! ends current frame and flushes all queues
         */
        void endFrame();

        /*! clears stats
         */
        void clearStats();

        /*! write igor specific shader parameters to current material

        \param targetMaterial the target specifig material values. Ignored if nullptr
        */
        void writeShaderParameters(iTargetMaterialPtr targetMaterial);

        /*! binds current material
         */
        void bindCurrentMaterial();
    };

}

#endif // __IGOR_RENDERER_2D__
