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

#ifndef __iRENDERER__
#define __iRENDERER__

#include <iRectangle.h>
#include <iTexture.h>
#include <iaGradient.h>

#include <iaSingleton.h>
#include <iaString.h>
#include <iaMatrix.h>
#include <iaColor4.h>
#include <iaColor3.h>
#include <iaEvent.h>
using namespace IgorAux;

#include <mutex>
#include <list>
#include <vector>
#include <map>
#include <memory>
#include <queue>
using namespace std;

namespace Igor
{
    class iTextureFont;
    class iMesh;
    class iParticle;
    class iParticle2D;
    class iSprite;
    class iVBOInstances;
    class iInstancer;
    class iMaterial;
    class iTargetMaterial;
    class iMeshBuffers;

    /*! shader object types
    */
    enum class iShaderObjectType
    {
        Vertex,
        Fragment,
        Geometry,
        Effect,
        Undefined
    };

    /*! texture placeholder class
    */
	class Igor_API iRendererTexture
	{
	public:

		uint32 _id;
	};

    /*! triggered when renderer is initialized. 
    usually after a window was opened.
    */
    iaEVENT(iRendererInitializedEvent, iRendererInitializedDelegate, void, (), ());

    /*! triggered when renderer is going to be deinitialized.
    usually after a window was closed.
    */
    iaEVENT(iRendererPreDeinitializeEvent, iRendererPreDeinitializeDelegate, void, (), ());

    /*! abstraction class for the actuall renderer. curently only OpenGL

    \todo light handling is not done by far

    Examples:
    \ref OpenGL3D/src/OpenGL3DExample.cpp "OpenGL3D usage example"
    \ref OpenGL2D/src/OpenGL2DExample.cpp "OpenGL2D usage example"

    */
    class Igor_API iRenderer : public iaSingleton<iRenderer>
    {

        friend class iaSingleton<iRenderer>;
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
            iaVector4f _position;

            /*! ambient color
            */
            iaColor4f _ambient = { 0.5f, 0.5f, 0.5f, 1.0f };

            /*! diffuse color
            */
            iaColor4f _diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };

            /*! specular color
            */
            iaColor4f _specular = { 0.8f, 0.8f, 0.8f, 1.0f };
        };

    public:

        /*! renderer definition of an invalid id
        */
        static const uint32 INVALID_ID = 0xffffffff;

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

        /*! projects a screen position in to world position

        \param screenpos the screen position (vertical origin is at top of screen)
        \param modelview the model view matrix
        \param projection the projection matrix
        \param viewport the viewport
        \returns world position of projection screen position
        */
        iaVector3d unProject(const iaVector3d& screenpos, const iaMatrixd& modelview, const iaMatrixd& projection, const iRectanglei& viewport);

        /*! creates vertex buffer objects for specified mesh

        \param mesh the mesh specified
        */
        shared_ptr<iMeshBuffers> createBuffers(shared_ptr<iMesh> mesh);

        /*! \returns true if the renderer is ready to use
        */
        virtual bool isReady();

        /*!
        \todo make more private again maybe we need a class to handle buffers
        */
        void destroyBuffer(uint32 bufferID);

        /*!
        \todo make more private again maybe we need a class to handle vertex arrays
        */
        void destroyVertexArray(uint32 bufferID);

        /*! set current material

        \param material the material to be set as current material
        */
        virtual void setMaterial(iMaterial* material);

        /*! set the viewport

        \param x horizontal position of viewport in pixel
        \param x vertical position of viewport in pixel
        \param with with of viewport in pixel
        \param height height of viewport in pixel
        */
        virtual void setViewport(int32 x, int32 y, int32 width, int32 height);

        /*! \returns viewport rectangle (sizes in pixel)
        */
        virtual void getViewport(iRectanglei& rect);

        /*! returns projection matrix

        \param[out] matrix destination to write projection matrix in
        */
        virtual void getProjectionMatrix(iaMatrixd& matrix);

        /*! sets projection matrix

        \param matrix the new projection matrix
        */
        virtual void setProjectionMatrix(const iaMatrixd& matrix);

        /*! set projection matrix with perspective projection

        \param fov field of view in degrees
        \param aspect aspect ratio of screen
        \param nearplane near plane distance
        \param farplane far plane distance
        */
        virtual void setPerspective(float32 fov, float32 aspect, float32 nearplane, float32 farplane);

        /*! set projection matrix with orthogonal projection

        \param left left bounding
        \param right right bounding
        \param bottom bottom bounding
        \param top top bounding
        \param nearplane near plane distance
        \param farplane far plane distance
        */
        virtual void setOrtho(float32 left, float32 right, float32 bottom, float32 top, float32 nearplane, float32 farplane);

        /*! returns model matrix

        \param[out] matrix destination to write model matrix in
        */
        virtual void getModelMatrix(iaMatrixd& matrix);

        /*! sets the model matrix

        \param matrix matrix to set the model matrix
        */
        virtual void setModelMatrix(iaMatrixd& matrix);

        /*! returns view matrix

        \param[out] matrix destination to write view wmatrix in
        */
        virtual void getViewMatrix(iaMatrixd& matrix);

        /*! sets the view matrix

        \param matrix matrix to set the view matrix
        */
        virtual void setViewMatrix(iaMatrixd& matrix);

        /*! returns carma world matrix

        \param[out] matrix destination to write camera world wmatrix in
        */
        virtual void getCamWorldMatrix(iaMatrixd& matrix);

        /*! sets the camera world matrix

        \param matrix matrix to set the camera world matrix
        */
        virtual void setCamWorldMatrix(iaMatrixd& matrix);

        /*! returns model view projection matrix

        \param[out] matrix destination to write model view projection wmatrix in
        */
        virtual void getModelViewProjectionMatrix(iaMatrixd& matrix);

        /*! sets the model view projection matrix

        \param matrix matrix to set the model view projection matrix
        */
        virtual void getModelViewMatrix(iaMatrixd& matrix);

        /*! sets clear color

        \param color the clear color
        */
        virtual void setClearColor(iaColor4f& color);

        /*! sets clear depth

        \param depth the clear depth
        */
        virtual void setClearDepth(float32 depth);

        /*! clears color buffer with clear color
        */
        virtual void clearColorBuffer();

        /*! clears depth buffer with clear depth
        */
        virtual void clearDepthBuffer();

        /*! reads rectangular area from screen buffer

        \param x horizontal position in pixel
        \parma y vertical position in pixel
        \param width width in pixel
        \param height height in pixel
        \param format color format
        \param data destination buffer to store the data in
        */
        virtual void readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, uint8* data);

        /*! reads rectangular area from screen buffer (float data)

        \param x horizontal position in pixel
        \parma y vertical position in pixel
        \param width width in pixel
        \param height height in pixel
        \param format color format
        \param data destination buffer to store the data in
        */
        virtual void readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, float32* data);

        /*! binds texture to texture unit

        \param texture the texture to bind
        \param textureunit the texture unit to bind the texture with
        */
        virtual void bindTexture(shared_ptr<iTexture> texture, uint32 textureunit);

        /*! sets current target emissive color

        \param emissive the emissive color
        */
        void setTargetEmissive(iaColor3f& emissive);

        /*! sets current target ambient color

        \param ambient the ambient color
        */
        void setTargetAmbient(iaColor3f& ambient);

        /*! sets current target diffuse color

        \param diffuse the diffuse color
        */
        void setTargetDiffuse(iaColor3f& diffuse);

        /*! sets current target specular color

        \param specular the specular color
        */
        void setTargetSpecular(iaColor3f& specular);

        /*! sets current target shininess factor

        \param shininess the shininess factor
        */
        void setTargetShininess(float32 shininess);

        /*! sets target material

        \param targetMaterial the target material
        */
        void setTargetMaterial(iTargetMaterial* targetMaterial);

        /*! draws a point

        \param x horizontal position
        \param y vertical position
        */
        virtual void drawPoint(float32 x, float32 y);

        /*! draw a line between two points

        \param x1 horizontal position of first point
        \param y1 vertical position of first point
        \param x2 horizontal position of second point
        \param y2 vertical position of second point
        */
        virtual void drawLine(float32 x1, float32 y1, float32 x2, float32 y2);

        /*! draw a filled rectangle.

        \param x horizontal position
        \param y vertical position
        \param width width of the rectangle
        \param height height of the rectangle
        */
        virtual void drawRectangle(float32 x, float32 y, float32 width, float32 height);

        /*! draw a filled rectangle with texture.

        Texture coordinates fill the rectangle once from 0-1 and 0-1.

        \param x horizontal position
        \param y vertical position
        \param width width of the rectangle
        \param height height of the rectangle
        \param texture the texture to render the rectangle with
        */
        virtual void drawTexture(float32 x, float32 y, float32 width, float32 height, shared_ptr<iTexture> texture);

        /*! draw a tiled texture

        as drawTexture but with repeating UV coordinates based on texture resolution

        \param x horizontal position
        \param y vertical position
        \param width width of the rectangle
        \param height height of the rectangle
        \param texture the texture to render the rectangle with
        */
        virtual void drawTextureTiled(float32 x, float32 y, float32 width, float32 height, shared_ptr<iTexture> texture);

        /*! draw a filled rectangle with texture.

        Texture coordinates fill the rectangle once from 0-1 and 0-1.
        Values for Width and Height are taken from the textures size.

        \param x horizontal position
        \param y vertical position
        \param texture the texture to render the rectangle with
        */
        virtual void drawTexture(float32 x, float32 y, shared_ptr<iTexture> texture);

        /*! draws a sprite at given position, orientation and scale

        \param sprite the sprite to render
        \param x horizontal position
        \param y vertical position
        \param angle orientation in rad
        \param scalex horizontal scale
        \param scaley vertical scale
        */
        virtual void drawSprite(iSprite* sprite, float32 x, float32 y, float32 angle = 0.0f, float32 scalex = 1.0f, float32 scaley = 1.0f);

        /*! draws a point

        \param pos 3d position to draw the point at
        */
        virtual void drawPoint(iaVector3f &pos);

        /*! draws a line between two 3d points

        \param posA first 3d point
        \param posB second 3d point
        */
        virtual void drawLine(iaVector3f &posA, iaVector3f &posB);

        /*! draws a line strip

        \param line list of line segments
        */
        virtual void drawLineStrip(vector<iaVector3f>& line);

        /*! draws an axis aligned box between two points

        \param posA first 3d point
        \param posB second 3d point
        */
        virtual void drawBox(iaVector3f &posA, iaVector3f &posB);

        /*! draws a billboard

        \param pos center position of billboard
        \param u first of two axis defining a plane
        \param v second of two axis defining a plane
        \param texture texture to draw with
        */
        virtual void drawBillboard(iaVector3f& pos, iaVector3f& u, iaVector3f& v, shared_ptr<iTexture> texture);

        /*! draws a billboard

        \param pos center position of billboard
        \param u first of two axis defining a plane
        \param v second of two axis defining a plane
        \param texture texture to draw with
        \param texScaleU scale of texture coordinates along the u axis
        \param texScaleV scale of texture coordinates along the v axis
        */
        virtual void drawBillboard(iaVector3f& pos, iaVector3f& u, iaVector3f& v, shared_ptr<iTexture> texture, float32 texScaleU, float32 texScaleV);

        /*! draws a mesh

        \param mesh the mesh to be drawn
        */
        virtual void drawMesh(shared_ptr<iMeshBuffers> mesh);

        /*! draws a mesh instanced

        \param mesh the mesh to be drawn
        \param instanceCount count of instances to be drawn
        */
        virtual void drawMesh(shared_ptr<iMeshBuffers> mesh, iInstancer* instancer);

        /*! set current color

        \param color the color to be set
        */
        virtual void setColor(iaColor4f color);

        /*! set current color

        \param r red channel
        \param g green channel
        \param b blue channel
        \param a alpha channel
        */
        virtual void setColor(float32 r, float32 g, float32 b, float32 a);

        // Lights
        virtual void setLightPosition(int32 lightnum, iaVector4d &pos);
        virtual void setLightAmbient(int32 lightnum, iaColor4f &ambient);
        virtual void setLightDiffuse(int32 lightnum, iaColor4f &diffuse);
        virtual void setLightSpecular(int32 lightnum, iaColor4f &specular);

        // 2D Bitmap Text
        /*! sets texture font to use next

        \param font the font to use
        */
        virtual void setFont(iTextureFont *font);

        /*! sets the size of fonts to render with

        \param fontSize the font size
        */
        virtual void setFontSize(float32 fontSize);

        /*! sets the font line height

        \param lineheight the text line height to render with
        */
        virtual void setFontLineHeight(float32 lineheight);

        /*! draws a string on screen using current font settings

        \param x horizontal position
        \param y vertical position
        \param text the text to draw
        \param angle orientation in degrees
        \param maxWidth the max width of the text. If text ist wider it will be rendered in multiple lines
        */
        virtual void drawString(float32 x, float32 y, iaString text, float32 angle = 0.0f, float32 maxWidth = 0.0f);

        /*! draws a string on screen using current font settings

        \param x horizontal position
        \param y vertical position
        \param text the text to draw
        \param horizontalAlignment horizontal alignment relative to horizontal position
        \param verticalAlignment vertical alignment relative to vertical position
        \param angle orientation in degrees
        \param maxWidth the max width of the text. If text ist wider it will be rendered in multiple lines
        */
        virtual void drawString(float32 x, float32 y, iaString text, iHorizontalAlignment horizontalAlignment, iVerticalAlignment verticalAlignment = iVerticalAlignment::Top, float32 angle = 0.0f, float32 maxWidth = 0.0f);

        // Particles
        virtual void drawParticles(const deque<iParticle> &particles, const iaGradientColor4f& rainbow);
        virtual void drawVelocityOrientedParticles(const deque<iParticle> &particles, const iaGradientColor4f& rainbow);
        virtual void drawParticles(float32 x, float32 y, float32 angle, iParticle2D* particles, int32 particleCount, iaGradientColor4f *rainbow = 0);

        // attributes
        /*! sets line render width

        \param lineWidth line render width
        */
        virtual void setLineWidth(float32 lineWidth);

        /*! sets point size

        \param pointSize point size
        */
        virtual void setPointSize(float32 pointSize);

        // shader
        int32 createShaderProgram();
        void destroyShaderProgram(int32 id);
        void useShaderProgram(int32 id);

        int32 createShaderObject(iShaderObjectType type);
        void destroyShaderObject(int32 id);

        void linkShaderProgram(int32 id, vector<int32> objects);
        bool compileShaderObject(int32 id, const char* source);

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

        void resetCounters();
        void getCounters(uint32& vertices, uint32& triangles, uint32& indices);

    private:

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
        map<int32, iRendererLight> _lights;

        /*! just to save the last set viewport values
        */
        iRectanglei _viewport;

        /*! current render color.

        default is white.
        */
        iaColor4f _color = { 1.0f, 1.0f, 1.0f, 1.0f };

        /*! current rainbow palette in use
        */
        iaGradientColor4f* _rainbow = nullptr;

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

        /*! statistic counter for vertices
        */
        uint32 _renderedVertices = 0;

        /*! statistic counter for triangles
        */
        uint32 _renderedTriangles = 0;

        /*! statistic counter for indicies
        */
        uint32 _renderedIndexes = 0;

        /*! current material in use
        */
        iMaterial* _currentMaterial = nullptr;

        /*! creates a Texture

        \param width width of the texture
        \parma height height of the texture
        \param bytepp bytes per pixel
        \param format color format of texture
        \param data pointer to the actual data used for the texture
        \param buildMode generation mode of texture like mimapping or not
        \param wrapMode wrap mode of texture
        \returns pointer to renderer texture
        */
        virtual iRendererTexture* createTexture(int32 width, int32 height, int32 bytepp, iColorFormat format, uint8* data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode);

        /*! releases texture

        \param texture pointer to texture to be released
        */
        virtual void destroyTexture(iRendererTexture* texture);

        /*! creates an instancing buffer for this instancer

        \param instancer instancer to create a instance buffer for
        */
        void createBuffers(iInstancer* instancer);

        /*! set's the dummy texture id

        \param id texture id
        */
        void setDummyTextureID(uint32 id);

        /*!
        */
        virtual void deinit();

        /*!
        */
        virtual void init();

        /*! updates the model view projection matrix if the corresponding dirty flag is true
        */
        void updateModelViewProjectionMatrix();

        /*! does nothing
        */
        iRenderer() = default;

        /*! does nothing
        */
        virtual ~iRenderer();

    };

    /*! \example OpenGL3D/src/OpenGL3DExample.cpp
    OpenGL3D usage example.

    \include OpenGL3D/src/OpenGL3DExample.h
    */

    /*! \example OpenGL2D/src/OpenGL2DExample.cpp
    OpenGL2D usage example.

    \include OpenGL2D/src/OpenGL2DExample.h
    */

};

#endif
