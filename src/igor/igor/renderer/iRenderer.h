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

#ifndef __IGOR_RENDERER_H__
#define __IGOR_RENDERER_H__

#include <igor/renderer/iRenderStateSet.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/module/iModule.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/data/iAABox.h>
#include <igor/data/iAACube.h>

#include <iaux/data/iaRectangle.h>
#include <iaux/data/iaGradient.h>
#include <iaux/data/iaString.h>
#include <iaux/data/iaColor4.h>
#include <iaux/data/iaColor3.h>
#include <iaux/math/iaMatrix.h>
#include <iaux/system/iaEvent.h>
#include <iaux/system/iaMutex.h>
using namespace iaux;

#include <list>
#include <vector>
#include <map>
#include <memory>
#include <queue>
#include <deque>

namespace igor
{
    class iTextureFont;
    class iParticle;
    class iParticle2D;
    class iInstancer;

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

    /*! triggered when renderer is initialized.
    usually after a window was opened.
    */
    iaEVENT(iRendererInitializedEvent, iRendererInitializedDelegate, (), ());

    /*! triggered when renderer is going to be deinitialized.
    usually after a window was closed.
    */
    iaEVENT(iRendererPreDeinitializeEvent, iRendererPreDeinitializeDelegate, (), ());

    /*! renderer statistics
     */
    struct iRendererStats
    {
        uint32 _vertices;
        uint32 _triangles;
        uint32 _indices;

        uint32 _verticesInstanced;
        uint32 _trianglesInstanced;
        uint32 _indicesInstanced;
    };

    /*! abstraction class for the actuall renderer. curently only OpenGL

    \todo light handling is not done at all
    */
    class IGOR_API iRenderer : public iModule<iRenderer>
    {

        friend class iModule<iRenderer>;
        friend class iTextureResourceFactory;
        friend class iRenderEngine;
        friend class iWindow;

        /*! internal structure for handling lights
         */
        struct iRendererLight
        {
        public:
            /*! the light world position
             */
            iaVector3f _position;

            /*! ambient color
             */
            iaColor3f _ambient = {0.5f, 0.5f, 0.5f};

            /*! diffuse color
             */
            iaColor3f _diffuse = {0.7f, 0.7f, 0.7f};

            /*! specular color
             */
            iaColor3f _specular = {0.8f, 0.8f, 0.8f};
        };

        /*! internal structure for handling a render target
         */
        struct iRendererTarget
        {
            /*! the render target type
             */
            iRenderTargetType _renderTargetType;

            /*! frame buffer object id
             */
            uint32 _frameBufferObject = 0;

            /*! color render buffer id
             */
            uint32 _colorBuffer = 0;

            /*! if true render target has depth buffer
             */
            bool _hasDepth = false;

            /*! depth render buffer id
             */
            uint32 _depthBuffer = 0;
        };

    public:
        /*! init
         */
        iRenderer();

        /*! deinit
         */
        ~iRenderer();

        /*! renderer definition of an invalid id
         */
        static const uint32 INVALID_ID = 0xffffffff;

        /*! the default render target ID
         */
        static const iRenderTargetID DEFAULT_RENDER_TARGET = 0;

        /*! register delegate to renderer initialized event

        \param initializedDelegate delegate to register
        */
        void registerInitializedDelegate(iRendererInitializedDelegate initializedDelegate);

        /*! unregister delegate from renderer initialized event

        \param initializedDelegate delegate to unregister
        */
        void unregisterInitializedDelegate(iRendererInitializedDelegate initializedDelegate);

        /*! register delegate to pre renderer deinitialize event

        \param preDeinitializeDelegate delegate to register
        */
        void registerPreDeinitializeDelegate(iRendererPreDeinitializeDelegate preDeinitializeDelegate);

        /*! unregister delegate from pre renderer deinitialize event

        \param preDeinitializeDelegate delegate to unregister
        */
        void unregisterPreDeinitializeDelegate(iRendererPreDeinitializeDelegate preDeinitializeDelegate);

        /*! creates vertex buffer objects for specified mesh

        \param mesh the mesh specified
        \todo this is weired stuff we should do that differently
        */
        iMeshBuffersPtr createBuffers(iMeshPtr mesh);

        /*!
        \todo this is weired stuff we should do that differently
        */
        void initBuffers(iMeshPtr mesh, iMeshBuffersPtr meshBuffers);

        /*!
        \todo this is weired stuff we should do that differently
        */
        iMeshBuffersPtr createBuffersAsync(iMeshPtr mesh);

        /*!
        \todo this is weired stuff we should do that differently
        */
        void createBuffers(float64 timeLimit = 10.0);

        /*! \returns true if the renderer is ready to use
         */
        bool isReady();

        /*!
        \todo make more private again maybe we need a class to handle buffers
        */
        void destroyBuffer(uint32 bufferID);

        /*!
        \todo make more private again maybe we need a class to handle vertex arrays
        */
        void destroyVertexArray(uint32 bufferID);

        /*! sets color Id to render with

        \param colorID next color ID to render with
        */
        void setColorID(uint64 colorID);

        int32 getShaderPropertyID(uint32 programID, const char *name);

        void writeShaderParameters();

        /*! set the viewport

        \param x horizontal position of viewport in pixel
        \param x vertical position of viewport in pixel
        \param with with of viewport in pixel
        \param height height of viewport in pixel
        */
        void setViewport(int32 x, int32 y, int32 width, int32 height);

        /*! \returns viewport rectangle (sizes in pixel)
         */
        void getViewport(iaRectanglei &rect);

        /*! returns projection matrix

        \param[out] matrix destination to write projection matrix in
        */
        void getProjectionMatrix(iaMatrixd &matrix);

        /*! sets projection matrix

        \param matrix the new projection matrix
        */
        void setProjectionMatrix(const iaMatrixd &matrix);

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

        /*! returns current model matrix

        \param[out] matrix returning model matrix
        */
        void getModelMatrix(iaMatrixd &matrix) const;

        void getViewMatrix(iaMatrixd &viewMatrix) const;

        const iaMatrixd &getModelViewProjectionMatrix() const
        {
            return _modelViewProjectionMatrix;
        }

        /*! sets the view matrix and camera world matrix

        \param viewMatrix matrix to set the view matrix
        \param camMatrix camera world matrix
        */
        void setViewMatrix(const iaMatrixd &viewMatrix, const iaMatrixd &camMatrix);

        /*! sets the view matrix and sets the camera world matrix to identity

        \param matrix matrix to set the view matrix
        */
        void setViewMatrix(const iaMatrixd &viewMatrix);

        /*! sets world grid resolution

        This one is kind of a workaround. In order to handle huge worlds beyond float precision we internally mess around with world positions.
        in consequence the world positions that end up in the shader are not valid. They are relative to the camera position. To compensate we can
        set the world grid resolution here. It basically makes a modulo on the world coordinates so they never exceed float precision.

        The default grid size is 10000.0
        Set it to zero if you don't want a grid at all

        \param gridSize sets the world grid size (only values with )
        */
        void setWorldGridResolution(float32 gridSize);

        /*! \returns world grid size
         */
        float32 getWorldGridResolution() const;

        /*! returns carma world matrix

        \param[out] matrix destination to write camera world wmatrix in
        */
        void getCamWorldMatrix(iaMatrixd &matrix) const;

        /*! sets clear color

        \param color the clear color
        */
        void setClearColor(const iaColor4f &color);

        /*! sets clear depth

        \param depth the clear depth
        */
        void setClearDepth(float32 depth);

        /*! clears color buffer with clear color
         */
        void clearColorBuffer();

        /*! clears depth buffer with clear depth
         */
        void clearDepthBuffer();

        /*! reads rectangular area from screen buffer

        \param x horizontal position in pixel
        \parma y vertical position in pixel
        \param width width in pixel
        \param height height in pixel
        \param format color format
        \param data destination buffer to store the data in
        */
        void readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, uint8 *data);

        /*! binds texture to texture unit

        \param texture the texture to bind
        \param textureunit the texture unit to bind the texture with
        */
        void bindTexture(iTexturePtr texture, uint32 textureunit);

        /*! sets target material

        \param targetMaterial the target material
        */
        void setTargetMaterial(iTargetMaterialPtr targetMaterial);

        /*! draw a line between two points

        \param x1 horizontal position of first point
        \param y1 vertical position of first point
        \param x2 horizontal position of second point
        \param y2 vertical position of second point
        */
        void drawLine(float32 x1, float32 y1, float32 x2, float32 y2);

        /*! draw a filled rectangle.

        \param x horizontal position
        \param y vertical position
        \param width width of the rectangle
        \param height height of the rectangle
        */
        void drawFilledRectangle(float32 x, float32 y, float32 width, float32 height);

        /*! draws a line between two 3d points

        \param posA first 3d point
        \param posB second 3d point
        */
        void drawLine(const iaVector3f &posA, const iaVector3f &posB);

        /*! draws a billboard

        \param pos center position of billboard
        \param u first of two axis defining a plane
        \param v second of two axis defining a plane
        \param texture texture to draw with
        */
        void drawBillboard(const iaVector3f &pos, const iaVector3f &u, const iaVector3f &v, iTexturePtr texture);

        /*! draws a billboard

        \param pos center position of billboard
        \param u first of two axis defining a plane
        \param v second of two axis defining a plane
        \param texture texture to draw with
        \param texScaleU scale of texture coordinates along the u axis
        \param texScaleV scale of texture coordinates along the v axis
        */
        void drawBillboard(const iaVector3f &pos, const iaVector3f &u, const iaVector3f &v, iTexturePtr texture, float32 texScaleU, float32 texScaleV);

        /*! draws a mesh

        \param mesh the mesh to be drawn
        */
        void drawMesh(iMeshBuffersPtr mesh);

        /*! draws a mesh instanced

        \param mesh the mesh to be drawn
        \param instanceCount count of instances to be drawn
        */
        void drawMesh(iMeshBuffersPtr mesh, iInstancer *instancer);

        /*! set current color

        \param color the color to be set
        */
        void setColor(const iaColor4f &color);
        void setColorExt(const iaColor4f &color); // replace the other set color once the fixed function pipeline is gone

        /*! set current color

        \param r red channel
        \param g green channel
        \param b blue channel
        \param a alpha channel
        */
        void setColor(float32 r, float32 g, float32 b, float32 a);

        // Lights
        void setLightPosition(int32 lightnum, const iaVector3d &pos);
        void setLightAmbient(int32 lightnum, iaColor3f &ambient);
        void setLightDiffuse(int32 lightnum, iaColor3f &diffuse);
        void setLightSpecular(int32 lightnum, iaColor3f &specular);

        // Particles
        void drawParticles(const std::deque<iParticle> &particles, const iaGradientColor4f &rainbow);
        void drawVelocityOrientedParticles(const std::deque<iParticle> &particles, const iaGradientColor4f &rainbow);

        // shader
        uint32 createShaderProgram();
        void destroyShaderProgram(uint32 id);
        void useShaderProgram(uint32 id);

        uint32 createShaderObject(iShaderObjectType type);
        void destroyShaderObject(uint32 id);

        void linkShaderProgram(uint32 id, std::vector<uint32> objects);

        /*! compiled given source for given shader

        \param id given shader id
        \param source zero terminate shader source
        */
        bool compileShaderObject(uint32 id, const char *source);

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

        // infos
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

        /*! triggerd at beginning of frame
         */
        void onStartFrame();

        /*! triggered at end of frame
         */
        void onStopFrame();

        /*! \returns stats of renderer
         */
        const iRendererStats &getStats() const;

    private:
        /*! current render target
         */
        uint32 _currentRenderTarget = DEFAULT_RENDER_TARGET;

        iaMutex _requestedBuffersMutex;
        std::deque<std::pair<iMeshPtr, iMeshBuffersPtr>> _requestedBuffers;

        /*! world grid resolution
         */
        float32 _gridSize = 10000.0;

        /*! the pre renderer deinitialize event
         */
        iRendererPreDeinitializeEvent _rendererPreDeinitializeEvent;

        /*! the renderer initialized event
         */
        iRendererInitializedEvent _rendererInitializedEvent;

        /*!
        \todo not sure if we need this
        */
        bool _initialized = false;

        /*! current font in use to render strings
         */
        iTextureFont *_font = nullptr;

        /*! current font size
         */
        float32 _fontSize = 10.0f;

        /*! current cont line height for rendering iaString with multiple lines
         */
        float32 _fontLineHeight = 1.15f;

        /*! OpenGL ID for dummy texture
         */
        uint32 _dummyTextureID = 0;

        /*! projection matrix
         */
        iaMatrixd _projectionMatrix;

        /*! model matrix
         */
        iaMatrixd _modelMatrix;

        /*! world offset matrix
         */
        iaVector3d _worldOffset;

        /*! view matrix
         */
        iaMatrixd _viewMatrix;

        /*! model and view matrix combined
         */
        iaMatrixd _modelViewMatrix;

        /*! model view and projection matrix combined
         */
        iaMatrixd _modelViewProjectionMatrix;

        /*! dirty flag for model view projection matrix.
        if true getModelViewProjectionMatrix will recalculate the matrix.
        */
        bool _dirtyModelViewProjectionMatrix = true;

        /*! camera world matrix
         */
        iaMatrixd _camWorldMatrix;

        /*! list of lights
         */
        std::map<int32, iRendererLight> _lights;

        /*! just to save the last set viewport values
         */
        iaRectanglei _viewport;

        /*! current render color.

        default is white.
        */
        iaColor4f _color = {1.0f, 1.0f, 1.0f, 1.0f};

        /*! current rainbow palette in use
         */
        iaGradientColor4f *_rainbow = nullptr;

        /*! vendor iaString
         */
        iaString _vendorOGL;

        /*! name of rendering device
         */
        iaString _rendererOGL;

        /*! version of OpenGL implemented
         */
        iaString _versionOGL;

        /*! OpenGL extensions implemented
         */
        iaString _extensionsOGL;

        /*! statistic counters
         */
        iRendererStats _stats;

        /*! current material in use
         */
        iMaterialPtr _currentMaterial;

        /*! map of render targets
         */
        std::map<uint32, iRendererTarget> _renderTargets;

        /*! creates a Texture

        \param width width of the texture
        \param height height of the texture
        \param bytepp bytes per pixel
        \param format color format of texture
        \param data pointer to the actual data used for the texture
        \param buildMode generation mode of texture like mimapping or not
        \param wrapMode wrap mode of texture
        \returns render texture id
        */
        uint32 createTexture(int32 width, int32 height, int32 bytepp, iColorFormat format, uint8 *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);

        /*! releases texture

        \param textureID texture ID to destroy
        */
        void destroyTexture(uint32 textureID);

        /*! creates an instancing buffer for this instancer

        \param instancer instancer to create a instance buffer for
        */
        void createBuffers(iInstancer *instancer);

        /*! set's the dummy texture id

        \param id texture id
        */
        void setDummyTextureID(uint32 id);

        /*! \returns current dummy texture ID
         */
        uint32 getDummyTextureID();

        /*!
         */
        void deinit();

        /*!
         */
        void init();

        /*! updates the model view projection matrix if the corresponding dirty flag is true
         */
        void updateModelViewProjectionMatrix();
    };

    /*! \example OpenGL3D/src/OpenGL3DExample.cpp
    OpenGL3D usage example.

    \include OpenGL3D/src/OpenGL3DExample.h
    */

    /*! \example OpenGL2D/src/OpenGL2DExample.cpp
    OpenGL2D usage example.

    \include OpenGL2D/src/OpenGL2DExample.h
    */

}; // namespace igor

#endif // __IGOR_RENDERER_H__
