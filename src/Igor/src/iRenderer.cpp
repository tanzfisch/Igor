// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iRenderer.h>

#include <iTextureFont.h>
#include <iSprite.h>
#include <iMesh.h>
#include <iRenderStateSet.h>
#include <iParticleSystem2D.h>
#include <iWindow.h>
#include <iTextureResourceFactory.h>
#include <iShader.h>
#include <iInstancer.h>
#include <iTargetMaterial.h>
#include <iMeshBuffers.h>
#include <iParticleSystem3D.h>
#include <iTimer.h>
#include <iMaterial.h>
#include <iMaterialResourceFactory.h>

#include <GLee.h>
#include <GL\glu.h>

#include <sstream>
#include <cmath>
using namespace std;

#include <IgorAux.h>
#include <iaConvert.h>
using namespace IgorAux;

namespace Igor
{

    // needs to be a mecro so we can retrive the line number of the actual problem
#ifdef __IGOR_DEBUG__
#define GL_CHECK_ERROR()                                                                    \
    do                                                                                      \
    {                                                                                       \
        GLenum error = glGetError();                                                        \
        if (error != GL_NO_ERROR)                                                           \
        {                                                                                   \
            iaString errorCode;                                                             \
            switch (error)                                                                  \
            {                                                                               \
                case GL_INVALID_ENUM: errorCode = "GL_INVALID_ENUM"; break;                 \
                case GL_INVALID_VALUE: errorCode = "GL_INVALID_VALUE"; break;               \
                case GL_INVALID_OPERATION: errorCode = "GL_INVALID_OPERATION"; break;       \
                case GL_STACK_OVERFLOW: errorCode = "GL_STACK_OVERFLOW"; break;             \
                case GL_STACK_UNDERFLOW: errorCode = "GL_STACK_UNDERFLOW"; break;           \
                case GL_OUT_OF_MEMORY: errorCode = "GL_OUT_OF_MEMORY"; break;               \
                default: errorCode = "UNKNOWN ERROR"; break;                                \
            };                                                                              \
            con_assert(error != GL_NO_ERROR, "GL_ERROR: " << errorCode);                    \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            break;                                                                          \
        }                                                                                   \
    }while (0)
#else
#define GL_CHECK_ERROR() 1
#endif

    GLenum convertGLColorFormat(iColorFormat format)
    {
        GLenum glformat = iRenderer::INVALID_ID;

        switch (format)
        {
        case iColorFormat::RGB:
            glformat = GL_RGB;
            break;

        case iColorFormat::RGBA:
            glformat = GL_RGBA;
            break;

        case iColorFormat::RED:
            glformat = GL_RED;
            break;

        case iColorFormat::GREEN:
            glformat = GL_GREEN;
            break;

        case iColorFormat::BLUE:
            glformat = GL_BLUE;
            break;

        case iColorFormat::ALPHA:
            glformat = GL_ALPHA;
            break;

        case iColorFormat::DEPTH:
            glformat = GL_DEPTH_COMPONENT;
            break;

        default:
            con_err("unknown color format");
        };

        return glformat;
    }

    iRenderer::~iRenderer()
    {
        if (_initialized)
        {
            deinit();
        }
    }

    void iRenderer::resetCounters()
    {
        _renderedVertices = 0;
        _renderedTriangles = 0;
        _renderedIndexes = 0;
    }

    void iRenderer::getCounters(uint32& vertices, uint32& triangles, uint32& indices)
    {
        vertices = _renderedVertices;
        triangles = _renderedTriangles;
        indices = _renderedIndexes;
    }

    bool iRenderer::compileShaderObject(uint32 id, const char* source)
    {
        int32 result = 0;
        glShaderSourceARB(id, 1, &source, nullptr); GL_CHECK_ERROR();
        glCompileShader(id); GL_CHECK_ERROR();

        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        char* buffer = new char[len];
        glGetInfoLogARB(id, len, &result, buffer);

        if (0 != result)
        {
            con_err("compiler error: \n" << buffer);
        }
        delete[] buffer;

        return (0 != result) ? false : true;
    }

    uint32 iRenderer::createShaderObject(iShaderObjectType type)
    {
        uint32 shaderObject = INVALID_ID;

        switch (type)
        {
        case iShaderObjectType::Fragment:
            shaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB); GL_CHECK_ERROR();
            break;

        case iShaderObjectType::Vertex:
            shaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); GL_CHECK_ERROR();
            break;

        case iShaderObjectType::Geometry:
            shaderObject = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB); GL_CHECK_ERROR();
            break;

        case iShaderObjectType::Effect:
        case iShaderObjectType::Undefined:
        default:
            con_err("unsupported shader type");
        };

        return shaderObject;
    }

    void iRenderer::destroyShaderObject(uint32 id)
    {
        con_assert(-1 != id, "invalid id");
        glDeleteObjectARB(id); GL_CHECK_ERROR();
    }

    void iRenderer::linkShaderProgram(uint32 id, vector<uint32> objects)
    {
        auto object = objects.begin();
        while (objects.end() != object)
        {
            glAttachObjectARB(id, (*object)); GL_CHECK_ERROR();
            object++;
        }

        glLinkProgramARB(id); GL_CHECK_ERROR();
    }

    uint32 iRenderer::createShaderProgram()
    {
        uint32 result = glCreateProgramObjectARB(); GL_CHECK_ERROR();
        return result;
    }

    void iRenderer::destroyShaderProgram(uint32 id)
    {
        con_assert(INVALID_ID != id, "invalid id");
        glDeleteProgram(id); GL_CHECK_ERROR();
    }

    void iRenderer::useShaderProgram(uint32 id)
    {
        con_assert(INVALID_ID != id, "invalid id");
        glUseProgramObjectARB(id);
    }

    bool iRenderer::isReady()
    {
        return _initialized;
    }

    void iRenderer::deinit()
    {
        if (_initialized)
        {
            _rendererPreDeinitializeEvent();

            _initialized = false;
        }
        else
        {
            con_warn("renderer not initialized");
        }
    }

    void iRenderer::init()
    {
        if (!_initialized)
        {
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); GL_CHECK_ERROR();
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	GL_CHECK_ERROR();
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	GL_CHECK_ERROR();
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	GL_CHECK_ERROR();

            glDepthFunc(GL_LESS);		GL_CHECK_ERROR();
            glEnable(GL_DEPTH_TEST);	GL_CHECK_ERROR();
            glDepthMask(true);			GL_CHECK_ERROR();

            _vendorOGL = (const char*)glGetString(GL_VENDOR);
            _rendererOGL = (const char*)glGetString(GL_RENDERER);
            _versionOGL = (const char*)glGetString(GL_VERSION);
            _extensionsOGL = (const char*)glGetString(GL_EXTENSIONS);

            con_endl("Initializing OpenGL" << endl << "Vendor    : " << _vendorOGL << endl << "Renderer  : " << _rendererOGL << endl << "Version   : " << _versionOGL << endl);

            setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));
            setClearDepth(1.0f);

            clearColorBuffer();
            clearDepthBuffer();

            _initialized = true;

            _rendererInitializedEvent();
        }
        else
        {
            con_warn("renderer already initialized");
        }
    }

    void iRenderer::registerInitializedDelegate(iRendererInitializedDelegate initializedDelegate)
    {
        _rendererInitializedEvent.append(initializedDelegate);
    }

    void iRenderer::unregisterInitializedDelegate(iRendererInitializedDelegate initializedDelegate)
    {
        _rendererInitializedEvent.remove(initializedDelegate);
    }

    void iRenderer::registerPreDeinitializeDelegate(iRendererPreDeinitializeDelegate preDeinitializeDelegate)
    {
        _rendererPreDeinitializeEvent.append(preDeinitializeDelegate);
    }

    void iRenderer::unregisterPreDeinitializeDelegate(iRendererPreDeinitializeDelegate preDeinitializeDelegate)
    {
        _rendererPreDeinitializeEvent.remove(preDeinitializeDelegate);
    }

    iaString iRenderer::getVendor()
    {
        return _vendorOGL;
    }

    iaString iRenderer::getRenderer()
    {
        return _rendererOGL;
    }

    iaString iRenderer::getVersion()
    {
        return _versionOGL;
    }

    iaString iRenderer::getExtensions()
    {
        return _extensionsOGL;
    }

    void iRenderer::setPerspective(float32 fov, float32 aspect, float32 nearplain, float32 farplain)
    {
        _projectionMatrix.perspective(fov, aspect, nearplain, farplain);

        glMatrixMode(GL_PROJECTION);				GL_CHECK_ERROR();
        glLoadMatrixd(_projectionMatrix.getData());	GL_CHECK_ERROR();
        glMatrixMode(GL_MODELVIEW);					GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::drawPoint(iaVector3f &a)
    {
        glBegin(GL_POINTS);
        glVertex3f(a._x, a._y, a._z);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawBBox(const iAACubed& bbox)
    {
        iaVector3d a = bbox._center;
        a._x -= bbox._halfEdgeLength;
        a._y -= bbox._halfEdgeLength;
        a._z -= bbox._halfEdgeLength;
        iaVector3d b = bbox._center;
        b._x += bbox._halfEdgeLength;
        b._y += bbox._halfEdgeLength;
        b._z += bbox._halfEdgeLength;

        glBegin(GL_LINES);
        glVertex3f(a._x, a._y, a._z);
        glVertex3f(b._x, a._y, a._z);

        glVertex3f(a._x, a._y, a._z);
        glVertex3f(a._x, b._y, a._z);

        glVertex3f(a._x, a._y, a._z);
        glVertex3f(a._x, a._y, b._z);

        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, a._y, b._z);

        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, b._y, a._z);

        glVertex3f(b._x, a._y, b._z);
        glVertex3f(b._x, b._y, b._z);

        glVertex3f(a._x, a._y, b._z);
        glVertex3f(b._x, a._y, b._z);

        glVertex3f(a._x, a._y, b._z);
        glVertex3f(a._x, b._y, b._z);

        glVertex3f(a._x, b._y, a._z);
        glVertex3f(a._x, b._y, b._z);

        glVertex3f(a._x, b._y, b._z);
        glVertex3f(b._x, b._y, b._z);

        glVertex3f(a._x, b._y, a._z);
        glVertex3f(b._x, b._y, a._z);

        glVertex3f(b._x, b._y, a._z);
        glVertex3f(b._x, b._y, b._z);

        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawBBox(const iAABoxd& bbox)
    {
        iaVector3d a = bbox._center;
        a -= bbox._halfWidths;
        iaVector3d b = bbox._center;
        b += bbox._halfWidths;

        glBegin(GL_LINES);
        glVertex3f(a._x, a._y, a._z);
        glVertex3f(b._x, a._y, a._z);

        glVertex3f(a._x, a._y, a._z);
        glVertex3f(a._x, b._y, a._z);

        glVertex3f(a._x, a._y, a._z);
        glVertex3f(a._x, a._y, b._z);

        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, a._y, b._z);

        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, b._y, a._z);

        glVertex3f(b._x, a._y, b._z);
        glVertex3f(b._x, b._y, b._z);

        glVertex3f(a._x, a._y, b._z);
        glVertex3f(b._x, a._y, b._z);

        glVertex3f(a._x, a._y, b._z);
        glVertex3f(a._x, b._y, b._z);

        glVertex3f(a._x, b._y, a._z);
        glVertex3f(a._x, b._y, b._z);

        glVertex3f(a._x, b._y, b._z);
        glVertex3f(b._x, b._y, b._z);

        glVertex3f(a._x, b._y, a._z);
        glVertex3f(b._x, b._y, a._z);

        glVertex3f(b._x, b._y, a._z);
        glVertex3f(b._x, b._y, b._z);

        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawFilledBox(iaVector3f &a, iaVector3f &b)
    {
        glBegin(GL_QUADS);
        // front
        glNormal3f(0, 0, -1);
        glVertex3f(a._x, b._y, a._z);
        glVertex3f(b._x, b._y, a._z);
        glVertex3f(b._x, a._y, a._z);
        glVertex3f(a._x, a._y, a._z);

        // back
        glNormal3f(0, 0, 1);
        glVertex3f(a._x, a._y, b._z);
        glVertex3f(b._x, a._y, b._z);
        glVertex3f(b._x, b._y, b._z);
        glVertex3f(a._x, b._y, b._z);

        // left
        glNormal3f(-1, 0, 0);
        glVertex3f(a._x, a._y, b._z);
        glVertex3f(a._x, b._y, b._z);
        glVertex3f(a._x, b._y, a._z);
        glVertex3f(a._x, a._y, a._z);

        // right
        glNormal3f(1, 0, 0);
        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, b._y, a._z);
        glVertex3f(b._x, b._y, b._z);
        glVertex3f(b._x, a._y, b._z);

        // bottom
        glNormal3f(0, -1, 0);
        glVertex3f(b._x, a._y, a._z);
        glVertex3f(b._x, a._y, b._z);
        glVertex3f(a._x, a._y, b._z);
        glVertex3f(a._x, a._y, a._z);

        // top
        glNormal3f(0, 1, 0);
        glVertex3f(a._x, b._y, a._z);
        glVertex3f(a._x, b._y, b._z);
        glVertex3f(b._x, b._y, b._z);
        glVertex3f(b._x, b._y, a._z);
        glEnd();		GL_CHECK_ERROR();
    }

    void iRenderer::drawLineStrip(vector<iaVector3f>& line)
    {
        glBegin(GL_LINE_STRIP);

        for (int i = 0; i < line.size(); ++i)
        {
            glVertex3f(line[i]._x, line[i]._y, line[i]._z);
        }

        glEnd();									GL_CHECK_ERROR();
    }

    void iRenderer::drawLine(iaVector3f &a, iaVector3f &b)
    {
        glBegin(GL_LINES);
        glVertex3f(a._x, a._y, a._z);
        glVertex3f(b._x, b._y, b._z);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawBillboard(iaVector3f &o, iaVector3f &u, iaVector3f &v, shared_ptr<iTexture> texture, float32 texScaleU, float32 texScaleV)
    {
        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(texScaleU, 0);
        glVertex3fv((o + v + u).getData());

        glTexCoord2f(texScaleU, texScaleV);
        glVertex3fv((o - v + u).getData());

        glTexCoord2f(0, texScaleV);
        glVertex3fv((o - v - u).getData());

        glTexCoord2f(0, 0);
        glVertex3fv((o + v - u).getData());
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawBillboard(iaVector3f &o, iaVector3f &u, iaVector3f &v, shared_ptr<iTexture> texture)
    {
        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(1, 0);
        glVertex3fv((o + v + u).getData());

        glTexCoord2f(1, 1);
        glVertex3fv((o - v + u).getData());

        glTexCoord2f(0, 1);
        glVertex3fv((o - v - u).getData());

        glTexCoord2f(0, 0);
        glVertex3fv((o + v - u).getData());
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::setOrtho(float32 left, float32 right, float32 bottom, float32 top, float32 nearplain, float32 farplain)
    {
        _projectionMatrix.ortho(left, right, bottom, top, nearplain, farplain);

        glMatrixMode(GL_PROJECTION);				GL_CHECK_ERROR();
        glLoadIdentity();							GL_CHECK_ERROR();
        glMultMatrixd(_projectionMatrix.getData());	GL_CHECK_ERROR();
        glMatrixMode(GL_MODELVIEW);					GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::setClearColor(iaColor4f& color)
    {
        glClearColor(color._r, color._g, color._b, color._a); GL_CHECK_ERROR();
    }

    void iRenderer::setClearDepth(float32 depth)
    {
        glClearDepth(depth);	GL_CHECK_ERROR();
    }

    void iRenderer::clearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT); GL_CHECK_ERROR();
    }

    void iRenderer::clearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);	GL_CHECK_ERROR();
    }

    void iRenderer::getProjectionMatrix(iaMatrixd& matrix)
    {
        matrix = _projectionMatrix;
    }

    void iRenderer::setProjectionMatrix(const iaMatrixd& matrix)
    {
        _projectionMatrix = matrix;

        glMatrixMode(GL_PROJECTION);				GL_CHECK_ERROR();
        glLoadIdentity();							GL_CHECK_ERROR();
        glMultMatrixd(_projectionMatrix.getData());	GL_CHECK_ERROR();
        glMatrixMode(GL_MODELVIEW);					GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::getViewport(iRectanglei& rect)
    {
        rect = _viewport;
    }

    void iRenderer::setViewport(int32 x, int32 y, int32 width, int32 height)
    {
        _viewport.setX(x);
        _viewport.setY(y);
        _viewport.setWidth(width);
        _viewport.setHeight(height);

        glViewport(x, y, width, height); GL_CHECK_ERROR();
    }

    void iRenderer::readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, uint8 *data)
    {
        GLenum glformat = convertGLColorFormat(format);
        con_assert(glformat != iRenderer::INVALID_ID, "invalid color format");

        if (_currentRenderTarget == iRenderer::DEFAULT_RENDER_TARGET)
        {
            glReadBuffer(GL_FRONT);
        }
        else
        {
            glReadBuffer(GL_COLOR_ATTACHMENT0);
        }
        glReadPixels(x, y, width, height, glformat, GL_UNSIGNED_BYTE, data); GL_CHECK_ERROR();
    }

    iRendererTexture* iRenderer::createTexture(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        int32 glformat = convertGLColorFormat(format);
        if (glformat == iRenderer::INVALID_ID)
        {
            return nullptr;
        }

        iRendererTexture *result = nullptr;

        result = new iRendererTexture();
        glGenTextures(1, (GLuint*)&(result->_id));													    GL_CHECK_ERROR();
        glBindTexture(GL_TEXTURE_2D, result->_id);													    GL_CHECK_ERROR();

        switch (wrapMode)
        {
        case  iTextureWrapMode::Repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                               GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                               GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::Clamp:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);                        GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);                        GL_CHECK_ERROR();
            break;

        case iTextureWrapMode::MirrorRepeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);                      GL_CHECK_ERROR();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);                      GL_CHECK_ERROR();
            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);							    GL_CHECK_ERROR();

        if (buildMode == iTextureBuildMode::Normal)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);				            GL_CHECK_ERROR();
            glTexImage2D(GL_TEXTURE_2D, 0, bytepp, width, height, 0, glformat, GL_UNSIGNED_BYTE, data); GL_CHECK_ERROR();
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);				GL_CHECK_ERROR();
            gluBuild2DMipmaps(GL_TEXTURE_2D, bytepp, width, height, glformat, GL_UNSIGNED_BYTE, data);	GL_CHECK_ERROR();
        }
        glFinish();																					    GL_CHECK_ERROR();

        return result;
    }

    void iRenderer::setDummyTextureID(uint32 id)
    {
        _dummyTextureID = id;
    }

    uint32 iRenderer::getDummyTextureID()
    {
        return _dummyTextureID;
    }

    void iRenderer::destroyTexture(iRendererTexture* texture)
    {
        con_assert(texture != nullptr, "zero pointer");

        if (texture != nullptr)
        {
            GLuint texId = (GLuint)((texture)->_id);

            bool isTexture = glIsTexture(texId);
            if (isTexture)
            {
                glDeleteTextures(1, &texId); GL_CHECK_ERROR();
            }

            delete texture;
        }
    }

    void iRenderer::bindTexture(shared_ptr<iTexture> texture, uint32 textureunit)
    {
        glActiveTexture(GL_TEXTURE0 + textureunit); GL_CHECK_ERROR();

        if (texture != nullptr)
        {
            if (!texture->isDummy())
            {
                glBindTexture(GL_TEXTURE_2D, (texture->_rendererTexture)->_id); GL_CHECK_ERROR();
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, _dummyTextureID); GL_CHECK_ERROR();
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0); GL_CHECK_ERROR();
        }
    }

    void iRenderer::getCamWorldMatrix(iaMatrixd& matrix)
    {
        matrix = _camWorldMatrix;
        matrix._pos += _worldOffset;
    }

    void iRenderer::getModelMatrix(iaMatrixd& matrix)
    {
        matrix = _modelMatrix;
        matrix._pos += _worldOffset;
    }

    void iRenderer::setModelMatrix(const iaMatrixd& matrix)
    {
        _modelMatrix = matrix;
        _modelMatrix._pos -= _worldOffset;

        _modelViewMatrix = _viewMatrix;
        _modelViewMatrix *= _modelMatrix;

        glLoadMatrixd(_modelViewMatrix.getData());	GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::setViewMatrix(const iaMatrixd& viewMatrix)
    {
        _viewMatrix = viewMatrix;

        _worldOffset.set(0, 0, 0);
        _camWorldMatrix.identity();

        _modelViewMatrix = _viewMatrix;
        _modelViewMatrix *= _modelMatrix;

        glLoadMatrixd(_modelViewMatrix.getData());	GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    float32 iRenderer::getWorldGridResolution() const
    {
        return _gridSize;
    }

    void iRenderer::setWorldGridResolution(float32 gridSize)
    {
        _gridSize = gridSize;
    }

    void iRenderer::setViewMatrix(const iaMatrixd& viewMatrix, const iaMatrixd& camMatrix)
    {
        _worldOffset = camMatrix._pos;

        if (_gridSize > 0.0)
        {
            _worldOffset /= static_cast<float64>(_gridSize);
            _worldOffset._x = trunc(_worldOffset._x);
            _worldOffset._y = trunc(_worldOffset._y);
            _worldOffset._z = trunc(_worldOffset._z);
            _worldOffset *= static_cast<float64>(_gridSize);
        }

        _camWorldMatrix = camMatrix;
        _camWorldMatrix._pos -= _worldOffset;

        iaMatrixd worldOffset;
        worldOffset._pos = _worldOffset;

        _viewMatrix = viewMatrix;
        _viewMatrix *= worldOffset;

        _modelViewMatrix = _viewMatrix;
        _modelViewMatrix *= _modelMatrix;

        glLoadMatrixd(_modelViewMatrix.getData());	GL_CHECK_ERROR();

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::updateModelViewProjectionMatrix()
    {
        if (_dirtyModelViewProjectionMatrix)
        {
            _modelViewProjectionMatrix = _projectionMatrix;
            _modelViewProjectionMatrix *= _modelViewMatrix;
            _dirtyModelViewProjectionMatrix = false;
        }
    }

    void iRenderer::setLineWidth(float32 width)
    {
        glLineWidth(width);
    }

    void iRenderer::setPointSize(float32 size)
    {
        glPointSize(size);
    }

    void iRenderer::setColorID(uint64 colorID)
    {
        if (_currentMaterial->_hasSolidColor &&
            _currentMaterial->getShader() != nullptr)
        {
            uint32 program = _currentMaterial->getShader()->getProgram();
            float32 color[4];
            color[0] = static_cast<float32>(static_cast<uint8>(colorID >> 16)) / 255.0;
            color[1] = static_cast<float32>(static_cast<uint8>(colorID >> 8)) / 255.0;
            color[2] = static_cast<float32>(static_cast<uint8>(colorID)) / 255.0;
            color[3] = 1.0f;

            glUniform4fv(_currentMaterial->_matSolidColor, 1, static_cast<GLfloat*>(color)); GL_CHECK_ERROR();
        }
    }

    void iRenderer::setTargetMaterial(iTargetMaterial* targetMaterial)
    {
        if (_currentMaterial->getShader() != nullptr)
        {
            if (_currentMaterial->_hasTargetMaterial)
            {
                glUniform3fv(_currentMaterial->_matAmbient, 1, static_cast<GLfloat*>(targetMaterial->getAmbient().getData())); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_matDiffuse, 1, static_cast<GLfloat*>(targetMaterial->getDiffuse().getData())); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_matSpecular, 1, static_cast<GLfloat*>(targetMaterial->getSpecular().getData())); GL_CHECK_ERROR();
                glUniform1f(_currentMaterial->_matShininess, targetMaterial->getShininess()); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_matSpecular, 1, static_cast<GLfloat*>(targetMaterial->getEmissive().getData())); GL_CHECK_ERROR();
            }

            if (targetMaterial->hasTextureUnit(0) &&
                _currentMaterial->_hasTexture[0])
            {
                glUniform1i(_currentMaterial->_matTexture[0], 0);
                bindTexture(targetMaterial->getTexture(0), 0);
            }

            if (targetMaterial->hasTextureUnit(1) &&
                _currentMaterial->_hasTexture[1])
            {
                glUniform1i(_currentMaterial->_matTexture[1], 1);
                bindTexture(targetMaterial->getTexture(1), 1);
            }

            if (targetMaterial->hasTextureUnit(2) &&
                _currentMaterial->_hasTexture[2])
            {
                glUniform1i(_currentMaterial->_matTexture[2], 2);
                bindTexture(targetMaterial->getTexture(2), 2);
            }

            if (targetMaterial->hasTextureUnit(3) &&
                _currentMaterial->_hasTexture[3])
            {
                glUniform1i(_currentMaterial->_matTexture[3], 3);
                bindTexture(targetMaterial->getTexture(3), 3);
            }
        }
    }

    void iRenderer::setTargetEmissive(iaColor3f& emissive)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive.getData());
    }

    void iRenderer::setTargetAmbient(iaColor3f& ambient)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.getData());
    }

    void iRenderer::setTargetDiffuse(iaColor3f& diffuse)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.getData());
    }

    void iRenderer::setTargetSpecular(iaColor3f& specular)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.getData());
    }

    void iRenderer::setTargetShininess(float32 shininess)
    {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }

    void iRenderer::setMaterial(uint64 materialID, bool forceWireframe)
    {
        auto material = iMaterialResourceFactory::getInstance().getMaterial(materialID);
        setMaterial(material, forceWireframe);
    }

    //! \todo this is just a first rudimentary version. we need a structure that only switches the deltas between materials
    void iRenderer::setMaterial(iMaterialPtr material, bool forceWireframe)
    {
        if (material == _currentMaterial)
        {
            return;
        }

        if (_currentMaterial)
        {
            _currentMaterial->deactivateShader();
        }

        _currentMaterial = material;

        if (_currentMaterial != nullptr)
        {
            _currentMaterial->activateShader();

            iRenderStateSet& stateset = _currentMaterial->getRenderStateSet();
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthTest)] == iRenderStateValue::On) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);		GL_CHECK_ERROR();
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthMask)] == iRenderStateValue::On) ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);	        GL_CHECK_ERROR();
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Blend)] == iRenderStateValue::On) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);	                GL_CHECK_ERROR();
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::CullFace)] == iRenderStateValue::On) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);	    GL_CHECK_ERROR();

            glActiveTexture(GL_TEXTURE0);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D0)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE1);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D1)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE2);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D2)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE3);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D3)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE4);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D4)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE5);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D5)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE6);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D6)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();
            glActiveTexture(GL_TEXTURE7);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D7)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);	GL_CHECK_ERROR();

            switch (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthFunc)])
            {
            case iRenderStateValue::Less:
                glDepthFunc(GL_LESS);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::LessOrEqual:
                glDepthFunc(GL_LEQUAL);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::Never:
                glDepthFunc(GL_NEVER);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::Equal:
                glDepthFunc(GL_EQUAL);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::Greater:
                glDepthFunc(GL_GREATER);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::NotEqual:
                glDepthFunc(GL_NOTEQUAL);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::GreaterOrEqual:
                glDepthFunc(GL_GEQUAL);   GL_CHECK_ERROR();
                break;

            case iRenderStateValue::Always:
                glDepthFunc(GL_ALWAYS);   GL_CHECK_ERROR();
                break;
            }

            switch (stateset._renderStates[static_cast<unsigned int>(iRenderState::CullFaceFunc)])
            {
            case iRenderStateValue::Front:
                glCullFace(GL_FRONT);    GL_CHECK_ERROR();
                break;

            case iRenderStateValue::Back:
                glCullFace(GL_BACK);    GL_CHECK_ERROR();
                break;
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); GL_CHECK_ERROR();

            if (stateset._renderStates[static_cast<unsigned int>(iRenderState::Wireframe)] == iRenderStateValue::On ||
                forceWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                GL_CHECK_ERROR();
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                GL_CHECK_ERROR();
            }
        }
    }

    void iRenderer::drawRectangle(float32 x, float32 y, float32 width, float32 height)
    {
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x, y + height);
        glVertex2f(x + width, y + height);
        glVertex2f(x + width, y);
        glEnd(); GL_CHECK_ERROR();

        glFlush();
    }

    void iRenderer::drawTextureTiled(float32 x, float32 y, float32 width, float32 height, shared_ptr<iTexture> texture)
    {
        float32 scaleX = width / texture->getWidth();
        float32 scaleY = height / texture->getHeight();

        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y);
        glTexCoord2f(0.0f, 1.0f * scaleY);
        glVertex2f(x, y + height);
        glTexCoord2f(1.0f * scaleX, 1.0f * scaleY);
        glVertex2f(x + width, y + height);
        glTexCoord2f(1.0f * scaleX, 0.0f);
        glVertex2f(x + width, y);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawTexture(float32 x, float32 y, float32 width, float32 height, shared_ptr<iTexture> texture)
    {
        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x, y + height);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + width, y + height);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + width, y);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawTexture(float32 x, float32 y, shared_ptr<iTexture> texture)
    {
        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(x, y);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(x + texture->getWidth(), y);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(x + texture->getWidth(), y + texture->getHeight());
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(x, y + texture->getHeight());
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawSprite(iSprite* sprite, float32 x, float32 y, float32 angle, float32 scalex, float32 scaley)
    {
        con_assert(sprite != nullptr, "zero pointer");

        if (sprite != nullptr)
        {
            iaVector2f o(x, y);
            iaVector2f u(1.0f, 0.0f);
            iaVector2f v;
            u.rotateXY(angle);
            v._x = -u._y;
            v._y = u._x;

            o = o - u * sprite->getOrigin()._x * scalex - v * sprite->getOrigin()._y * scaley;

            iaVector2f b = o + u * (sprite->getWidth() * scalex);
            iaVector2f c = b + v * (sprite->getHeight() * scaley);
            iaVector2f d = o + v * (sprite->getHeight() * scaley);

            bindTexture(sprite->getTexture(), 0);

            glBegin(GL_QUADS);
            glTexCoord2f(sprite->getTexCoord(1)._x, sprite->getTexCoord(1)._y);
            glVertex2f(d._x, d._y);

            glTexCoord2f(sprite->getTexCoord(2)._x, sprite->getTexCoord(2)._y);
            glVertex2f(c._x, c._y);

            glTexCoord2f(sprite->getTexCoord(3)._x, sprite->getTexCoord(3)._y);
            glVertex2f(b._x, b._y);

            glTexCoord2f(sprite->getTexCoord(0)._x, sprite->getTexCoord(0)._y);
            glVertex2f(o._x, o._y);

            glEnd(); GL_CHECK_ERROR();
        }
    }

    uint32 iRenderer::createRenderTarget(uint32 width, uint32 height, iColorFormat format, iRenderTargetType renderTargetType, bool useDepthBuffer)
    {
        uint32 result = iRenderer::DEFAULT_RENDER_TARGET;
        GLenum glformat = convertGLColorFormat(format);
        con_assert(glformat != iRenderer::INVALID_ID, "invalid color format");

        if (glformat != iRenderer::INVALID_ID)
        {
            GLuint fbo;
            GLuint colorRenderBuffer;
            GLuint depthRenderBuffer;
            glGenFramebuffersEXT(1, &fbo); GL_CHECK_ERROR();
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); GL_CHECK_ERROR();

            glGenRenderbuffersEXT(1, &colorRenderBuffer); GL_CHECK_ERROR();
            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorRenderBuffer); GL_CHECK_ERROR();
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, width, height);
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorRenderBuffer);

            if (useDepthBuffer)
            {
                glGenRenderbuffersEXT(1, &depthRenderBuffer);
                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBuffer);
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBuffer);
            }

            if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
            {
                con_err("unsupported frame buffer object configureation");

                // clean up again
                glDeleteRenderbuffersEXT(1, &colorRenderBuffer); GL_CHECK_ERROR();
                if (useDepthBuffer)
                {
                    glDeleteRenderbuffersEXT(1, &depthRenderBuffer); GL_CHECK_ERROR();
                }
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
                glDeleteFramebuffersEXT(1, &fbo);

                // restore current render target
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentRenderTarget);

                return iRenderer::DEFAULT_RENDER_TARGET;
            }

            // restore current render target
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentRenderTarget);

            iRendererTarget renderTarget;
            renderTarget._renderTargetType = renderTargetType;
            renderTarget._frameBufferObject = fbo;
            renderTarget._colorBuffer = colorRenderBuffer;
            renderTarget._hasDepth = useDepthBuffer;
            renderTarget._depthBuffer = depthRenderBuffer;

            // id is handled by ogl so we don't have to check it
            _renderTargets[fbo] = renderTarget;

            result = fbo;
        }
        else
        {
            con_err("invalid color format");
        }

        return result;
    }

    void iRenderer::destroyRenderTarget(uint32 id)
    {
        auto iter = _renderTargets.find(id);
        if (iter != _renderTargets.end())
        {
            iRendererTarget renderTarget = (*iter).second;

            glDeleteRenderbuffersEXT(1, &renderTarget._colorBuffer); GL_CHECK_ERROR();
            if (renderTarget._hasDepth)
            {
                glDeleteRenderbuffersEXT(1, &renderTarget._depthBuffer); GL_CHECK_ERROR();
            }

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glDeleteFramebuffersEXT(1, &renderTarget._frameBufferObject); GL_CHECK_ERROR();

            // restore current render target
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentRenderTarget);

            _renderTargets.erase(iter);
        }
        else
        {
            con_err("invalid render target id " << id);
        }
    }

    void iRenderer::setRenderTarget(uint32 id)
    {
        // the ID is also the frame buffer object ID
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id); GL_CHECK_ERROR();
        _currentRenderTarget = id;
    }

    uint32 iRenderer::getRenderTarget() const
    {
        return _currentRenderTarget;
    }

    void iRenderer::destroyBuffer(uint32 bufferID)
    {
        if (glIsBuffer(bufferID))
        {
            glDeleteBuffers(1, &bufferID); GL_CHECK_ERROR();
        }
    }

    void iRenderer::destroyVertexArray(uint32 bufferID)
    {
        if (glIsBuffer(bufferID))
        {
            glDeleteVertexArrays(1, &bufferID); GL_CHECK_ERROR();
        }
    }

    void iRenderer::createBuffers(float64 timeLimit)
    {
        float64 endTime = iTimer::getInstance().getApplicationTime() + timeLimit;
        deque<pair<shared_ptr<iMesh>, shared_ptr<iMeshBuffers>>>::iterator entryIter;

        shared_ptr<iMesh> mesh;
        shared_ptr<iMeshBuffers> meshBuffers;
        bool proceed = false;

        while (true)
        {
            _requestedBuffersMutex.lock();
            if (!_requestedBuffers.empty())
            {
                entryIter = _requestedBuffers.begin();
                mesh = (*entryIter).first;
                meshBuffers = (*entryIter).second;
                _requestedBuffers.pop_front();
                proceed = true;
            }
            _requestedBuffersMutex.unlock();

            if (proceed)
            {
                initBuffers(mesh, meshBuffers);
                proceed = false;
            }
            else
            {
                break;
            }

            if (iTimer::getInstance().getApplicationTime() > endTime)
            {
                break;
            }
        }
    }

    shared_ptr<iMeshBuffers> iRenderer::createBuffersAsync(shared_ptr<iMesh> mesh)
    {
        iMeshBuffers* meshBuffer = new iMeshBuffers();

        shared_ptr<iMeshBuffers> result = shared_ptr<iMeshBuffers>(meshBuffer);

        _requestedBuffersMutex.lock();
        _requestedBuffers.push_back(pair<shared_ptr<iMesh>, shared_ptr<iMeshBuffers>>(mesh, result));
        _requestedBuffersMutex.unlock();

        return result;
    }

    void iRenderer::initBuffers(shared_ptr<iMesh> mesh, shared_ptr<iMeshBuffers> meshBuffers)
    {
        uint32 vao = 0;
        uint32 ibo = 0;
        uint32 vbo = 0;

        glGenVertexArrays(1, &vao); GL_CHECK_ERROR();
        meshBuffers->setVertexArrayObject(vao);
        glBindVertexArray(meshBuffers->getVertexArrayObject()); GL_CHECK_ERROR();

        if (mesh->getIndexData() != nullptr)
        {
            glGenBuffers(1, &ibo); GL_CHECK_ERROR();
            meshBuffers->setIndexBufferObject(ibo);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBuffers->getIndexBufferObject()); GL_CHECK_ERROR();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexDataSize(), mesh->getIndexData(), GL_STATIC_DRAW); GL_CHECK_ERROR();
        }

        glGenBuffers(1, &vbo); GL_CHECK_ERROR();
        meshBuffers->setVertexBufferObject(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, meshBuffers->getVertexBufferObject()); GL_CHECK_ERROR();
        glBufferData(GL_ARRAY_BUFFER, mesh->getVertexDataSize(), mesh->getVertexData(), GL_STATIC_DRAW); GL_CHECK_ERROR();

        uint32 location = 0;
        uint32 offset = 0;

        glEnableVertexAttribArray(0); GL_CHECK_ERROR();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), (void*)offset); GL_CHECK_ERROR();
        offset += 3 * sizeof(float32);

        if (mesh->hasNormals())
        {
            glEnableVertexAttribArray(1); GL_CHECK_ERROR();
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), (void*)offset); GL_CHECK_ERROR();
            offset += 3 * sizeof(float32);
        }

        for (int i = 0; i < 4; ++i)
        {
            if (mesh->hasTextureUnit(i))
            {
                glEnableVertexAttribArray(2 + i); GL_CHECK_ERROR();
                glVertexAttribPointer(2 + i, 2, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), (void*)offset); GL_CHECK_ERROR();
                offset += 2 * sizeof(float32);
            }
        }

        meshBuffers->setIndexesCount(mesh->getIndexesCount());
        meshBuffers->setTrianglesCount(mesh->getTrianglesCount());
        meshBuffers->setVertexCount(mesh->getVertexCount());

        meshBuffers->setReady();
    }

    /*! http://in2gpu.com/2014/09/07/instanced-drawing-opengl/
    */
    shared_ptr<iMeshBuffers> iRenderer::createBuffers(shared_ptr<iMesh> mesh)
    {
        iMeshBuffers* meshBuffer = new iMeshBuffers();
        shared_ptr<iMeshBuffers> result = shared_ptr<iMeshBuffers>(meshBuffer);

        initBuffers(mesh, result);

        return result;
    }

    void iRenderer::createBuffers(iInstancer* instancer)
    {
        if (instancer->getInstanceArrayObject() == 0)
        {
            uint32 instanceArrayObject = 0;

            glGenBuffers(1, &instanceArrayObject); GL_CHECK_ERROR();
            glBindBuffer(GL_ARRAY_BUFFER, instanceArrayObject); GL_CHECK_ERROR();
            glBufferData(GL_ARRAY_BUFFER, instancer->getInstanceDataBufferSize(), nullptr, GL_DYNAMIC_DRAW); GL_CHECK_ERROR();

            instancer->setInstanceArrayObject(instanceArrayObject);
        }
    }

    void iRenderer::drawMesh(shared_ptr<iMeshBuffers> meshBuffers, iInstancer* instancer)
    {
        iaMatrixd idMatrix;
        setModelMatrix(idMatrix);

        createBuffers(instancer);

        writeShaderParameters();

        glBindBuffer(GL_ARRAY_BUFFER, instancer->getInstanceArrayObject()); GL_CHECK_ERROR();

        glBufferSubData(GL_ARRAY_BUFFER, 0, instancer->getInstanceSize() * instancer->getInstanceCount(), instancer->getInstanceDataBuffer()); GL_CHECK_ERROR();

        glEnableVertexAttribArray(3); GL_CHECK_ERROR();
        glEnableVertexAttribArray(4); GL_CHECK_ERROR();
        glEnableVertexAttribArray(5); GL_CHECK_ERROR();
        glEnableVertexAttribArray(6); GL_CHECK_ERROR();
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void*)(0 * sizeof(float32))); GL_CHECK_ERROR();
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void*)(4 * sizeof(float32))); GL_CHECK_ERROR();
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void*)(8 * sizeof(float32))); GL_CHECK_ERROR();
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void*)(12 * sizeof(float32))); GL_CHECK_ERROR();
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(meshBuffers->getVertexArrayObject()); GL_CHECK_ERROR();

        glDrawElementsInstancedARB(GL_TRIANGLES, meshBuffers->getIndexesCount(), GL_UNSIGNED_INT, 0, instancer->getInstanceCount()); GL_CHECK_ERROR();

        glBindVertexArray(0); GL_CHECK_ERROR();

        // todo maybe we should count this during cull process
        _renderedVertices += meshBuffers->getVertexCount() * instancer->getInstanceCount();
        _renderedIndexes += meshBuffers->getIndexesCount() * instancer->getInstanceCount();
        _renderedTriangles += meshBuffers->getTrianglesCount() * instancer->getInstanceCount();
    }

    int32 iRenderer::getShaderPropertyID(uint32 programID, const char* name)
    {
        int32 result = glGetUniformLocation(programID, name); GL_CHECK_ERROR();
        return result;
    }

    void iRenderer::writeShaderParameters()
    {
        if (_currentMaterial->getShader() != nullptr)
        {
            if (_currentMaterial->_hasDirectionalLight)
            {
                glUniform3fv(_currentMaterial->_lightOrientation, 1, static_cast<GLfloat*>(_lights[0]._position.getData())); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_lightAmbient, 1, static_cast<GLfloat*>(_lights[0]._ambient.getData())); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_lightDiffuse, 1, static_cast<GLfloat*>(_lights[0]._diffuse.getData())); GL_CHECK_ERROR();
                glUniform3fv(_currentMaterial->_lightSpecular, 1, static_cast<GLfloat*>(_lights[0]._specular.getData())); GL_CHECK_ERROR();
            }

            if (_currentMaterial->_hasEyePosition)
            {
                iaVector3f eyePosition(_camWorldMatrix._pos._x, _camWorldMatrix._pos._y, _camWorldMatrix._pos._z);
                glUniform3fv(_currentMaterial->_eyePosition, 1, static_cast<GLfloat*>(eyePosition.getData())); GL_CHECK_ERROR();
            }

            if (_currentMaterial->_hasModelViewProjectionMatrix)
            {
                updateModelViewProjectionMatrix();
                iaMatrixf modelViewProjection;
                for (int i = 0; i < 16; ++i)
                {
                    modelViewProjection[i] = _modelViewProjectionMatrix[i];
                }
                glUniformMatrix4fv(_currentMaterial->_mvp_matrix, 1, false, modelViewProjection.getData()); GL_CHECK_ERROR();
            }

            if (_currentMaterial->_hasModelMatrix)
            {
                iaMatrixf model;
                for (int i = 0; i < 16; ++i)
                {
                    model[i] = _modelMatrix[i];
                }
                glUniformMatrix4fv(_currentMaterial->_model_matrix, 1, false, model.getData()); GL_CHECK_ERROR();
            }
        }
    }

    void iRenderer::drawMesh(shared_ptr<iMeshBuffers> meshBuffers)
    {
        writeShaderParameters();

        glBindVertexArray(meshBuffers->getVertexArrayObject()); GL_CHECK_ERROR();

        glDrawElements(GL_TRIANGLES, meshBuffers->getIndexesCount(), GL_UNSIGNED_INT, 0); GL_CHECK_ERROR();

        glBindVertexArray(0); GL_CHECK_ERROR();

        _renderedVertices += meshBuffers->getVertexCount();
        _renderedIndexes += meshBuffers->getIndexesCount();
        _renderedTriangles += meshBuffers->getTrianglesCount();

    }

    void iRenderer::drawPoint(float32 x, float32 y)
    {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::drawLine(float32 x1, float32 y1, float32 x2, float32 y2)
    {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd(); GL_CHECK_ERROR();
    }

    void iRenderer::setColorExt(iaColor4f color)
    {
        if (_currentMaterial->_hasSolidColor)
        {
            if (_currentMaterial->getShader() != nullptr)
            {
                uint32 program = _currentMaterial->getShader()->getProgram();
                glUniform4fv(_currentMaterial->_matSolidColor, 1, static_cast<GLfloat*>(color.getData())); GL_CHECK_ERROR();
            }
        }
    }

    void iRenderer::setColor(iaColor4f color)
    {
        _color = color;
        glColor4f(_color._r, _color._g, _color._b, _color._a);	GL_CHECK_ERROR();
    }

    void iRenderer::setColor(float32 r, float32 g, float32 b, float32 a)
    {
        _color._r = r;
        _color._g = g;
        _color._b = b;
        _color._a = a;
        glColor4f(_color._r, _color._g, _color._b, _color._a);	GL_CHECK_ERROR();
    }

    void iRenderer::setFont(iTextureFont *font)
    {
        _font = font;
    }

    void iRenderer::setFontSize(float32 size)
    {
        _fontSize = size;
    }

    void iRenderer::setFontLineHeight(float32 lineheight)
    {
        _fontLineHeight = lineheight;
    }

    void iRenderer::drawString(float32 x, float32 y, iaString text, iHorizontalAlignment horz, iVerticalAlignment vert, float32 angle, float32 maxWidth)
    {
        con_assert(horz == iHorizontalAlignment::Left || horz == iHorizontalAlignment::Right || horz == iHorizontalAlignment::Center, "invalid parameters");
        con_assert(vert == iVerticalAlignment::Top || vert == iVerticalAlignment::Bottom || vert == iVerticalAlignment::Center, "invalid parameters");

        if (nullptr != _font)
        {
            float32 posx, posy;

            if (horz == iHorizontalAlignment::Left)
            {
                posx = x;
            }
            else if (horz == iHorizontalAlignment::Right)
            {
                if (maxWidth == 0.0f)
                {
                    posx = x - _font->measureWidth(text, _fontSize);
                }
                else
                {
                    posx = x - maxWidth;
                }
            }
            else if (horz == iHorizontalAlignment::Center)
            {
                if (maxWidth == 0.0f)
                {
                    posx = x - _font->measureWidth(text, _fontSize) * 0.5f;
                }
                else
                {
                    posx = x - maxWidth * 0.5f;
                }
            }

            if (vert == iVerticalAlignment::Top)
            {
                posy = y;
            }
            else if (vert == iVerticalAlignment::Bottom)
            {
                posy = y - _font->measureHeight(text, _fontSize, maxWidth, _fontLineHeight);
            }
            else if (vert == iVerticalAlignment::Center)
            {
                posy = y - _font->measureHeight(text, _fontSize, maxWidth, _fontLineHeight) * 0.5f;
            }

            drawString(posx, posy, text, angle, maxWidth);
        }
    }

    void iRenderer::drawString(float32 x, float32 y, iaString text, float32 angle, float32 maxWidth)
    {
        static wchar_t temptext[1024];

        if (nullptr != _font)
        {
            unsigned char ascii_code, font_code;
            iaVector2f texturePos;
            iaVector2f textureSize;
            iaVector2f renderSize(_fontSize, _fontSize);
            iaVector2f renderPos;
            uint32 textLength = text.getSize();
            bool changecolor = false;
            bool donotdraw = false;
            vector<iCharacterDimensions> characters = _font->getCharacters();

            bindTexture(_font->getTexture(), 0);

            glPushMatrix();					GL_CHECK_ERROR();
            glTranslatef(x, y, 0);			GL_CHECK_ERROR();
            glRotatef(-angle, 0, 0, 1);

            glBegin(GL_QUADS);

            for (uint32 i = 0; i < textLength; i++)
            {
                ascii_code = (unsigned char)text[i];
                font_code = ascii_code - 32;

                texturePos._x = characters[font_code].rect.getX();
                texturePos._y = characters[font_code].rect.getY();

                textureSize._x = characters[font_code].rect.getWidth();
                textureSize._y = characters[font_code].rect.getHeight();

                renderSize._x = _fontSize * characters[font_code].relRenderWidth;

                donotdraw = false;

                if (maxWidth != 0)
                {
                    if (ascii_code == ' ')
                    {
                        int j = 1;
                        while ((i + j < text.getSize()) && (text[i + j] != ' '))
                        {
                            temptext[j - 1] = text[i + j];
                            j++;
                        }
                        temptext[j] = 0;

                        if (renderPos._x + _font->measureWidth(temptext, _fontSize) > maxWidth)
                        {
                            renderPos._x = 0;
                            renderPos._y += _fontSize*_fontLineHeight;
                            donotdraw = true;
                        }
                    }
                }

                if (!donotdraw)
                {
                    glTexCoord2d(texturePos._x, texturePos._y + textureSize._y);
                    glVertex2f(renderPos._x, renderPos._y + renderSize._y);

                    glTexCoord2d(texturePos._x + textureSize._x, texturePos._y + textureSize._y);
                    glVertex2f(renderPos._x + renderSize._x, renderPos._y + renderSize._y);

                    glTexCoord2d(texturePos._x + textureSize._x, texturePos._y);
                    glVertex2f(renderPos._x + renderSize._x, renderPos._y);

                    glTexCoord2d(texturePos._x, texturePos._y);
                    glVertex2f(renderPos._x, renderPos._y);

                    renderPos._x += renderSize._x;
                }
            }

            glEnd();		GL_CHECK_ERROR();
            glPopMatrix();	GL_CHECK_ERROR();
        }
    }

    void iRenderer::setLightPosition(int32 lightnum, iaVector4d &pos)
    {
        // TODO fix with world offset
        _lights[lightnum]._position.set(pos._vec._x, pos._vec._y, pos._vec._z, pos._w);
        glLightfv(GL_LIGHT0 + lightnum, GL_POSITION, _lights[lightnum]._position.getData());		GL_CHECK_ERROR();
    }

    void iRenderer::setLightAmbient(int32 lightnum, iaColor4f &ambient)
    {
        _lights[lightnum]._ambient = ambient;
        glLightfv(GL_LIGHT0 + lightnum, GL_AMBIENT, ambient.getData());		GL_CHECK_ERROR();
    }

    void iRenderer::setLightDiffuse(int32 lightnum, iaColor4f &diffuse)
    {
        _lights[lightnum]._diffuse = diffuse;
        glLightfv(GL_LIGHT0 + lightnum, GL_DIFFUSE, diffuse.getData());		GL_CHECK_ERROR();
    }

    void iRenderer::setLightSpecular(int32 lightnum, iaColor4f &specular)
    {
        _lights[lightnum]._specular = specular;
        glLightfv(GL_LIGHT0 + lightnum, GL_SPECULAR, specular.getData());		GL_CHECK_ERROR();
    }

    /*!
    \bug in debug mode the rainbow colors are wrong. it's changing back and forth
    */
    void iRenderer::drawParticles(float32 x, float32 y, float32 angle, iParticle2D* particles, int32 particleCount, iaGradientColor4f *rainbow)
    {
        iaVector2f a, b, c, d, u, v;
        iaColor4f color;

        glPushMatrix();					GL_CHECK_ERROR();
        glTranslatef(x, y, 0);			GL_CHECK_ERROR();
        glRotatef(-angle, 0, 0, 1);

        glBegin(GL_QUADS);

        for (uint32 i = 0; i < particleCount; ++i)
        {
            if (particles[i]._life > 0)
            {
                if (rainbow != nullptr)
                {
                    rainbow->getValue(particles[i]._life, color);
                    glColor4fv(color.getData());
                }

                u.set(1.0f, 0.0f);
                u.rotateXY(particles[i]._angle);
                v._x = -u._y;
                v._y = u._x;

                u *= 0.5f * particles[i]._size;
                v *= 0.5f * particles[i]._size;

                a = particles[i]._position;
                a -= u;
                a -= v;

                b = particles[i]._position;
                b += u;
                b -= v;

                c = particles[i]._position;
                c += u;
                c += v;

                d = particles[i]._position;
                d -= u;
                d += v;

                glTexCoord2f(0.0f, 0.0f);
                glVertex2fv(d.getData());

                glTexCoord2f(1.0f, 0.0f);
                glVertex2fv(c.getData());

                glTexCoord2f(1.0f, 1.0f);
                glVertex2fv(b.getData());

                glTexCoord2f(0.0f, 1.0f);
                glVertex2fv(a.getData());
            }
        }

        glEnd();		GL_CHECK_ERROR();
        glPopMatrix();	GL_CHECK_ERROR();

        // todo maybe we should count this during cull process
        _renderedVertices += particleCount * 4;
        _renderedIndexes += particleCount * 4;
        _renderedTriangles += particleCount * 2;
    }

    void iRenderer::drawParticles(const deque<iParticle> &particles, const iaGradientColor4f& rainbow)
    {
        iaVector4f camright;
        camright._vec.set(_camWorldMatrix._right._x, _camWorldMatrix._right._y, _camWorldMatrix._right._z);
        camright._w = 0;

        iaVector4f camtop;
        camtop._vec.set(_camWorldMatrix._top._x, _camWorldMatrix._top._y, _camWorldMatrix._top._z);
        camtop._w = 0;

        iaMatrixf inv;
        iaConvert::convert(_modelMatrix, inv);
        inv.invert();

        iaVector4f rightPreComp = inv * camright;
        iaVector4f topPreComp = inv * camtop;
        iaVector3f right;
        iaVector3f top;
        iaColor4f color;
        float32 size;

        iaVector3f rightScale;
        iaVector3f topScale;

        iaVector2f x(1, 0);
        iaVector2f y;

        glBegin(GL_QUADS);

        for (auto particle : particles)
        {
            if (particle._visible)
            {
                size = particle._size * particle._sizeScale;

                right = rightPreComp._vec;
                right *= size;
                top = topPreComp._vec;
                top *= size;

                rainbow.getValue(particle._visibleTime, color);
                glColor4fv(color.getData());

                x.set(1, 0);
                x.rotateXY(particle._orientation);
                y._x = -x._y;
                y._y = x._x;

                rightScale = right * x._x + top * x._y;
                topScale = right * y._x + top * y._y;

                glMultiTexCoord2f(GL_TEXTURE0, particle._texturefrom._x, particle._texturefrom._y);
                glMultiTexCoord2f(GL_TEXTURE1, 0 + particle._phase0[0], 1 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 0 + particle._phase1[0], 1 + particle._phase1[1]);

                glVertex3fv((particle._position - rightScale + topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._texturefrom._x, particle._textureto._y);
                glMultiTexCoord2f(GL_TEXTURE1, 0 + particle._phase0[0], 0 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 0 + particle._phase1[0], 0 + particle._phase1[1]);

                glVertex3fv((particle._position - rightScale - topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._textureto._x, particle._textureto._y);
                glMultiTexCoord2f(GL_TEXTURE1, 1 + particle._phase0[0], 0 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 1 + particle._phase1[0], 0 + particle._phase1[1]);

                glVertex3fv((particle._position + rightScale - topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._textureto._x, particle._texturefrom._y);
                glMultiTexCoord2f(GL_TEXTURE1, 1 + particle._phase0[0], 1 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 1 + particle._phase1[0], 1 + particle._phase1[1]);

                glVertex3fv((particle._position + rightScale + topScale).getData());


            }
        }

        glEnd();		GL_CHECK_ERROR();

        // todo maybe we should count this during cull process
        _renderedVertices += static_cast<uint32>(particles.size()) * 4;
        _renderedIndexes += static_cast<uint32>(particles.size()) * 4;
        _renderedTriangles += static_cast<uint32>(particles.size()) * 2;
    }

    void iRenderer::drawVelocityOrientedParticles(const deque<iParticle> &particles, const iaGradientColor4f& rainbow)
    {
        // TODO implement also for local coordinates see drawParticles

        iaVector3f right;
        iaVector3f top(_camWorldMatrix._top._x, _camWorldMatrix._top._y, _camWorldMatrix._top._z);
        iaVector3f depth(_camWorldMatrix._depth._x, _camWorldMatrix._depth._y, _camWorldMatrix._depth._z);
        iaColor4f color;
        float32 size;

        iaVector3f rightScale;
        iaVector3f topScale;
        iaVector3f direction;

        iaVector2f x(1, 0);
        iaVector2f y;

        glBegin(GL_QUADS);

        for (auto particle : particles)
        {
            if (particle._visible)
            {
                size = particle._size * particle._sizeScale;

                rainbow.getValue(particle._visibleTime, color);
                glColor4fv(color.getData());

                direction = particle._velocity;
                direction.normalize();

                right = direction % depth;
                top = depth % right;

                right *= size;
                top *= size;

                x.set(1, 0);
                x.rotateXY(particle._orientation);
                y._x = -x._y;
                y._y = x._x;

                rightScale = right * x._x + top * x._y;
                topScale = right * y._x + top * y._y;

                glMultiTexCoord2f(GL_TEXTURE0, particle._texturefrom._x, particle._texturefrom._y);
                glMultiTexCoord2f(GL_TEXTURE1, 0 + particle._phase0[0], 1 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 0 + particle._phase1[0], 1 + particle._phase1[1]);

                glVertex3fv((particle._position - rightScale + topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._texturefrom._x, particle._textureto._y);
                glMultiTexCoord2f(GL_TEXTURE1, 0 + particle._phase0[0], 0 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 0 + particle._phase1[0], 0 + particle._phase1[1]);

                glVertex3fv((particle._position - rightScale - topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._textureto._x, particle._textureto._y);
                glMultiTexCoord2f(GL_TEXTURE1, 1 + particle._phase0[0], 0 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 1 + particle._phase1[0], 0 + particle._phase1[1]);

                glVertex3fv((particle._position + rightScale - topScale).getData());

                glMultiTexCoord2f(GL_TEXTURE0, particle._textureto._x, particle._texturefrom._y);
                glMultiTexCoord2f(GL_TEXTURE1, 1 + particle._phase0[0], 1 + particle._phase0[1]);
                glMultiTexCoord2f(GL_TEXTURE2, 1 + particle._phase1[0], 1 + particle._phase1[1]);

                glVertex3fv((particle._position + rightScale + topScale).getData());
            }
        }

        glEnd();		GL_CHECK_ERROR();

        // todo maybe we should count this during cull process
        _renderedVertices += static_cast<uint32>(particles.size()) * 4;
        _renderedIndexes += static_cast<uint32>(particles.size()) * 4;
        _renderedTriangles += static_cast<uint32>(particles.size()) * 2;
    }

    iaVector3d iRenderer::project(const iaVector3d& objectSpacePos, const iaMatrixd& modelview, const iaMatrixd& projection, const iRectanglei& viewport)
    {
        iaVector4d in(objectSpacePos._x, objectSpacePos._y, objectSpacePos._z, 1);
        iaVector4d out;
        iaVector3d result;

        iaMatrixd modelViewProjection = projection;
        modelViewProjection *= modelview;
        out = modelViewProjection * in;

        result._x = (float64)viewport.getWidth() * (out._vec._x + 1.0) / 2.0;
        result._y = (float64)viewport.getHeight() * (1.0 - ((out._vec._y + 1.0) / 2.0));
        result._z = out._vec._z;

        return result;
    }

    iaVector3d iRenderer::unProject(const iaVector3d& screenpos, const iaMatrixd& modelview, const iaMatrixd& projection, const iRectanglei& viewport)
    {
        iaVector4d in;
        iaVector4d out;
        iaVector3d result;

        in[0] = (screenpos[0] - (float32)viewport.getX()) / (float32)viewport.getWidth() * 2.0f - 1.0f;
        in[1] = (((float32)viewport.getHeight() - screenpos[1]) - (float32)viewport.getY()) / (float32)viewport.getHeight() * 2.0f - 1.0f;
        in[2] = screenpos[2] * 2.0f - 1.0f;
        in[3] = 1.0f;

        iaMatrixd modelViewProjection = projection;
        modelViewProjection *= modelview;
        modelViewProjection.invert();
        out = modelViewProjection * in;

        con_assert(out[3] != 0.0f, "out of range");

        if (out[3] != 0.0f)
        {
            result[0] = out[0] / out[3];
            result[1] = out[1] / out[3];
            result[2] = out[2] / out[3];
        }

        return result;
    }

};
