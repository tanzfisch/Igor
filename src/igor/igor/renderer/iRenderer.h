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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_RENDERER_H
#define IGOR_RENDERER_H

#include <igor/data/iAABox.h>
#include <igor/data/iAACube.h>
#include <igor/renderer/buffers/iInstancingBuffer.h>
#include <igor/resources/shader/iShader.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/sprite/iSprite.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/material/iMaterial.h>

#include <iaux/data/iaColor4.h>
#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaCircle.h>
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
        friend class iEntityScene;

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
        void setShader(const iShaderPtr &material);

        /*! \returns currently active material
         */
        const iShaderPtr &getMaterial() const;

        /*! set projection matrix with perspective projection

        \param fov field of view in degrees
        \param aspect aspect ratio of screen
        \param nearPlain near plane distance
        \param farPlain far plane distance
        */
        void setPerspective(float64 fov, float64 aspect, float64 nearPlain, float64 farPlain);

        /*! set projection matrix with orthogonal projection

        \param left left bounding
        \param right right bounding
        \param bottom bottom bounding
        \param top top bounding
        \param nearPlain near plane distance
        \param farPlain far plane distance
        */
        void setOrtho(float64 left, float64 right, float64 bottom, float64 top, float64 nearPlain, float64 farPlain);

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

        /*! draw point at given position (z = 0)

        \param x horizontal position
        \param y vertical position
        \param color the color to draw with
        */
        template <typename T>
        void drawPoint(T x, T y, const iaColor4f &color = iaColor4f::white);

        /*! draw point at given position (z = 0)

        \param v position
        \param color the color to draw with
        */
        template <typename T>
        void drawPoint(const iaVector2<T> &v, const iaColor4f &color = iaColor4f::white);

        /*! draw point at given position

        \param v position
        \param color the color to draw with
        */
        template <typename T>
        void drawPoint(const iaVector3<T> &v, const iaColor4f &color = iaColor4f::white);

        /*! draw line (z = 0)

        \param x1 horizontal position of line start
        \param y1 vertical position of line start
        \param x2 horizontal position of line end
        \param y2 vertical position of line end
        \param color the color to draw with
        */
        template <typename T>
        void drawLine(T x1, T y1, T x2, T y2, const iaColor4f &color = iaColor4f::white);

        /*! draw line (z = 0)

        \param v1 position of line start
        \param v2 position of line end
        \param color the color to draw with
        */
        template <typename T>
        void drawLine(const iaVector2<T> &v1, const iaVector2<T> &v2, const iaColor4f &color = iaColor4f::white);

        /*! draw line

        \param v1 position of line start
        \param v2 position of line end
        \param color the color to draw with
        */
        template <typename T>
        void drawLine(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaColor4f &color = iaColor4f::white);

        /*! draw line strip (z = 0)

        \param points points to draw the line strip with
        \param color the color to draw with
        */
        template <typename T>
        void drawLineStrip(const std::vector<iaVector2<T>> &points, const iaColor4f &color = iaColor4f::white);

        /*! draw line strip

        \param points points to draw the line strip with
        \param color the color to draw with
        */
        template <typename T>
        void drawLineStrip(const std::vector<iaVector3<T>> &points, const iaColor4f &color = iaColor4f::white);

        /*! draw line loop (z = 0)

        last point will be connected to the first point to close the loop

        \param points points to draw the line strip with
        \param color the color to draw with
        */
        template <typename T>
        void drawLineLoop(const std::vector<iaVector2<T>> &points, const iaColor4f &color = iaColor4f::white);

        /*! draw line loop

        last point will be connected to the first point to close the loop

        \param points points to draw the line strip with
        \param color the color to draw with
        */
        template <typename T>
        void drawLineLoop(const std::vector<iaVector3<T>> &points, const iaColor4f &color = iaColor4f::white);

        /*! draw rectangle (z = 0)

        \param x horizontal position
        \param y vertical position
        \param width horizontal size
        \param height vertical size
        \param color the color to draw with
        */
        template <typename T>
        void drawRectangle(T x, T y, T width, T height, const iaColor4f &color = iaColor4f::white);

        /*! draw rectangle (z = 0)

        \param rect the rectangle to draw
        \param color the color to draw with
        */
        template <typename T>
        void drawRectangle(const iaRectangle<T> &rect, const iaColor4f &color = iaColor4f::white);

        /*! draw filled rectangle (z = 0)

        \param rect the rectangle to draw
        \param color the color to draw with
        */
        template <typename T>
        void drawFilledRectangle(const iaRectangle<T> &rect, const iaColor4f &color = iaColor4f::white);

        /*! draw filled rectangle (z = 0)

        \param x horizontal position
        \param y vertical position
        \param width horizontal size
        \param height vertical size

        \param color the color to draw with
        */
        template <typename T>
        void drawFilledRectangle(T x, T y, T width, T height, const iaColor4f &color = iaColor4f::white);

        /*! draw filled rectangle (z = 0)

        \param rect the rectangle to draw
        \param color1 the color top left
        \param color2 the color bottom left
        \param color3 the color bottom right
        \param color4 the color top right
        */
        template <typename T>
        void drawFilledRectangleColored(const iaRectangle<T> &rect, const iaColor4f &color1, const iaColor4f &color2, const iaColor4f &color3, const iaColor4f &color4);

        /*! draw quad

        \param o center/origin position of quad
        \param u u/x/horizontal extension of quad
        \param v v/y/vertical extension of quad
        \param color the color to draw with
        */
        template <typename T>
        void drawQuad(const iaVector3<T> &o, const iaVector3<T> &u, const iaVector3<T> &v, const iaColor4f &color = iaColor4f::white);

        /*! draws a unit sized quad multiplied with given matrix

        \param matrix the given matrix
        \param color the color to draw with
        */
        template <typename T>
        void drawQuad(const iaMatrix<T> &matrix, const iaColor4f &color = iaColor4f::white);

        /*! draws a quad with 4 given points

        \param v1 first given point
        \param v2 second given point
        \param v3 third given point
        \param v4 fourth given point
        \param color the color to draw with
        */
        template <typename T>
        void drawQuad(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaVector3<T> &v3, const iaVector3<T> &v4, const iaColor4f &color = iaColor4f::white);

        /*! draw textured rectangle (z = 0)

        \param x horizontal position
        \param y vertical position
        \param width horizontal size
        \param height vertical size
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        template <typename T>
        void drawTexturedRectangle(T x, T y, T width, T height, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2<T> &tiling = iaVector2<T>(static_cast<T>(1), static_cast<T>(1)));

        /*! draw textured rectangle (z = 0)

        \param rect the rectangle to draw
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        template <typename T>
        void drawTexturedRectangle(const iaRectangle<T> &rect, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2<T> &tiling = iaVector2<T>(static_cast<T>(1), static_cast<T>(1)));

        /*! draw textured unit quad multiplied by a matrix

        \param matrix the given matrix
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        template <typename T>
        void drawTexturedQuad(const iaMatrix<T> &matrix, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2<T> &tiling = iaVector2<T>(static_cast<T>(1), static_cast<T>(1)));

        /*! draw textured quad

        \param o center/origin position of quad
        \param u u/x/horizontal extension of quad
        \param v v/y/vertical extension of quad
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        template <typename T>
        void drawTexturedQuad(const iaVector3<T> &o, const iaVector3<T> &u, const iaVector3<T> &v, iTexturePtr texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2<T> &tiling = iaVector2<T>(static_cast<T>(1), static_cast<T>(1)));

        /*! draw textured quad with four given points

        \param v1 first given point
        \param v2 second given point
        \param v3 third given point
        \param v4 fourth given point
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        template <typename T>
        void drawTexturedQuad(const iaVector3<T> &v1, const iaVector3<T> &v2, const iaVector3<T> &v3, const iaVector3<T> &v4, const iTexturePtr &texture, const iaColor4f &color = iaColor4f::white, bool blend = false, const iaVector2<T> &tiling = iaVector2<T>(static_cast<T>(1), static_cast<T>(1)));

        /*! draw specified frame of given sprite

        \param matrix the matrix to position and scale the frame
        \param sprite the sprite texture to use
        \param frameIndex the frame index in the sprite
        \param size the size of the sprite (on top of matrix scale)
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
         */
        template <typename T>
        void drawSprite(const iaMatrix<T> &matrix, const iSpritePtr &sprite, uint32 frameIndex = 0, const iaVector2<T> &size = iaVector2<T>(1.0f, 1.0f), const iaColor4f &color = iaColor4f::white, bool blend = false);

        /*! draw string

        \param x horizontal position
        \param y vertical position
        \param text the text to draw
        \param horz the horizontal alignment
        \param vert the vertical alignment
        \param color the color to draw with
        \param maxWidth the maximum width to render or else there will be line breaks
        */
        void drawString(float32 x, float32 y, const iaString &text, iHorizontalAlignment horz, iVerticalAlignment vert, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);

        /*! draw string

        \param x horizontal position
        \param y vertical position
        \param text the text to draw
        \param color the color to draw with
        \param maxWidth the maximum width to render or else there will be line breaks
        */
        void drawString(float32 x, float32 y, const iaString &text, const iaColor4f &color = iaColor4f::white, float32 maxWidth = 0.0f);

        /*! draw a circle.

        \param x horizontal center position
        \param y vertical center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawCircle(T x, T y, T radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a circle.

        \param pos center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawCircle(const iaVector2<T> &pos, T radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a circle.

        \param circle circle definition to draw
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawCircle(const iaCircle<T> &circle, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a filled circle.

        \param x horizontal center position
        \param y vertical center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawFilledCircle(T x, T y, T radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a filled circle.

        \param pos center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawFilledCircle(const iaVector2<T> &pos, T radius, int segments = 16, const iaColor4f &color = iaColor4f::white);

        /*! draw a filled circle.

        \param circle circle definition to draw
        \param segments segments count
        \param color the color to draw with
        */
        template <typename T>
        void drawFilledCircle(const iaCircle<T> &circle, int segments = 16, const iaColor4f &color = iaColor4f::white);

        ///////////////////// 3D ////////////////////////////
        /*! draw box based on given axis aligned cube

        positioned based on current model view and projection matrices

        \param cube the given cube
        \param color the color to draw with
        */
        template <typename T>
        void drawBox(const iAACube<T> &cube, const iaColor4f &color = iaColor4f::white);

        /*! draw box based on given axis aligned box

        \param box the given box
        \param color the color to draw with
        */
        template <typename T>
        void drawBox(const iAABox<T> &box, const iaColor4f &color = iaColor4f::white);

        /*! draw mesh

        positioned based on current model view and projection matrices

        \param mesh the given mesh to draw
        \param material the material to use
        */
        void drawMesh(iMeshPtr mesh, iMaterialPtr material);

        /*! draw mesh instanced

        positioned based on current model view and projection matrices

        \param mesh the given mesh to draw
        \param instancingBuffer the instancing buffer
        \param material the material to use
        */
        void drawMeshInstanced(iMeshPtr mesh, iInstancingBufferPtr instancingBuffer, iMaterialPtr material = nullptr);

        /*! draws buffer with given material and primitive type

        \param vertexArray the buffer to draw
        \param primitiveType the given primitive type
        \param material the material (optional)
        */
        void drawBuffer(iVertexArrayPtr vertexArray, iRenderPrimitive primitiveType, iMaterialPtr material = nullptr);

        /////////////// LIGHT TODO this might change a lot ///////////
        void setLightPosition(int32 lightnum, const iaVector3d &pos);
        void setLightAmbient(int32 lightnum, const iaColor3f &ambient);
        void setLightDiffuse(int32 lightnum, const iaColor3f &diffuse);
        void setLightSpecular(int32 lightnum, const iaColor3f &specular);

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

        /*! \returns true if stencil test is active
         */
        bool isStencilTestActive() const;

        /*! sets the stencil function

        \param function the stencil function (legal values are Never, Less, LessOrEqual, Greater, GreaterOrEqual, Equal, NotEqual, and Always)
        \param ref the reference value
        \param mask the mask value to gate the result of the test
        */
        void setStencilFunction(iStencilFunction function, int32 ref, uint32 mask);

        /*! sets the stencil test actions

        legal values are Keep, Zero, Replace, Increment, IncrementWrap, Decrement, DecrementWrap, and Invert

        \param fail action when stencil test fails
        \param zfail action when stencil test passes but depth test fails
        \param zpass action when stencil and depth test passed
        */
        void setStencilOperation(iStencilOperation fail, iStencilOperation zfail, iStencilOperation zpass);

        /*! sets the stencil mask value

        \param mask the stencil mas
         */
        void setStencilMask(uint8 mask);

        /*! sets the depth test active/inactive

        \param enable if true the depth test is enabled
        */
        void setDepthTestActive(bool enable);

        /*! \returns true if depth test is enabled
         */
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

        /*! sets color Id to render with

        is used by materials which use UNIFORM_SOLIDCOLOR to encode a color id

        \param colorID next color ID to render with
        */
        void setColorID(uint64 colorID);

        /*! sets the solid color

        only used by specialized shaders. see UNIFORM_SOLIDCOLOR

        \param color the color to set
        */
        void setColor(const iaColor4f &color);

        /*! creates a render target

        \param width the width of the render target in pixel/texel
        \param height the height of the render target in pixel/texel
        \param format the color format of the render target
        \param renderTargetType the render target type
        \param useDepthBuffer if true render target is having a depth buffer
        \returns id of render target
        */
        iRenderTargetID createRenderTarget(uint32 width, uint32 height, iColorFormat format, iRenderTargetType renderTargetType, bool useDepthBuffer);

        /*! destroys render target by id

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
        \param y vertical position in pixel
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

        /*! \returns default material
         */
        const iShaderPtr &getDefaultShader() const;

        /*! \returns colorID material
         */
        const iShaderPtr &getColorIDMaterial() const;

        /*! draws everything that is still in the queue
         */
        void flush();

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

        /*! updates internal matrices
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

        \param material the target specific material values. Ignored if nullptr
        */
        void writeShaderParameters(iMaterialPtr material);

        /*! binds current material
         */
        void bindCurrentMaterial();

        /*! draw point at given position

        \param v position
        \param color the color to draw with
        */
        void drawPointInternal(const iaVector3f &v, const iaColor4f &color);

        /*! draw line

        \param v1 position of line start
        \param v2 position of line end
        \param color the color to draw with
        */
        void drawLineInternal(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color);

        /*! draws a quad with 4 given points

        \param v1 first given point
        \param v2 second given point
        \param v3 third given point
        \param v4 fourth given point
        \param color1 the color for the first vertex
        \param color2 the color for the second vertex
        \param color3 the color for the third vertex
        \param color4 the color for the forth vertex
        */
        void drawQuadInternal(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iaColor4f &color1, const iaColor4f &color2, const iaColor4f &color3, const iaColor4f &color4);

        /*! draw textured quad with four given points

        \param v1 first given point
        \param v2 second given point
        \param v3 third given point
        \param v4 fourth given point
        \param texture the texture to use. if invalid fallback texture is used
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
        \param tiling tiling of the texture in two dimensions
        */
        void drawTexturedQuadInternal(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2f &tiling);

        /*! draw specified frame of given sprite

        \param matrix the matrix to position and scale the frame
        \param sprite the sprite texture to use
        \param frameIndex the frame index in the sprite
        \param size the size of the sprite to render
        \param color the color to draw with
        \param blend if true blending is used to draw the rectangle
         */
        void drawSpriteInternal(const iaMatrixf &matrix, const iSpritePtr &sprite, uint32 frameIndex, const iaVector2f &size, const iaColor4f &color, bool blend);

        /*! draw a filled circle.

        \param x horizontal center position
        \param y vertical center position
        \param radius radius of the circle
        \param segments segments count
        \param color the color to draw with
        */
        void drawFilledCircleInternal(float32 x, float32 y, float32 radius, int segments, const iaColor4f &color);

        /*! draw box based on given axis aligned box

        \param box the given box
        \param color the color to draw with
        */
        void drawBoxInternal(const iAABoxf &box, const iaColor4f &color);
    };

#include <igor/renderer/iRenderer.inl>

}

#endif // IGOR_RENDERER_H
