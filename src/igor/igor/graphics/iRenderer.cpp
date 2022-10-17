// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/graphics/iRenderer.h>

// #include <igor/system/iWindow.h>
#include <igor/system/iTimer.h>
#include <igor/simulation/iParticleSystem2D.h>
#include <igor/graphics/iInstancer.h>
#include <igor/simulation/iParticleSystem3D.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iShader.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/resources/mesh/iMeshBuffers.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/material/iMaterialResourceFactory.h>

#define GL_GLEXT_PROTOTYPES
#include <GLee.h>
#include <GL/glu.h>

#include <sstream>
#include <cmath>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

// comes from X.h. don't know how this get's included here
#undef Always

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace igor
{
    // print out openGL debug messages
    static void onOGLDebugOutput(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            con_crit(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            con_err(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            con_warn(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            con_debug(message);
            return;
        }

        con_crit("Unknown Error");
    }

    static GLenum convertGLColorFormat(iColorFormat format)
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

    iRenderer::iRenderer()
    {
#ifdef __IGOR_DEBUG__
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(onOGLDebugOutput, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
    }

    iRenderer::~iRenderer()
    {
        if (_initialized)
        {
            deinit();
        }
    }

    void iRenderer::onStopFrame()
    {
    }

    void iRenderer::onStartFrame()
    {
        _stats._vertices = 0;
        _stats._triangles = 0;
        _stats._indices = 0;
        _stats._verticesInstanced = 0;
        _stats._trianglesInstanced = 0;
        _stats._indicesInstanced = 0;
    }

    const iRendererStats &iRenderer::getStats() const
    {
        return _stats;
    }

    bool iRenderer::compileShaderObject(uint32 id, const char *source)
    {
        GLsizei result = 0;
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        int len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

        if (len != 0)
        {
            char *buffer = new char[len];
            glGetInfoLogARB(id, len, &result, buffer);

            if (0 != result)
            {
                con_err("shader compiler error\n"
                        << buffer);
                con_endl("source:\n"
                         << iaString(source));
            }
            delete[] buffer;
        }

        return (0 != result) ? false : true;
    }

    uint32 iRenderer::createShaderObject(iShaderObjectType type)
    {
        uint32 shaderObject = INVALID_ID;

        switch (type)
        {
        case iShaderObjectType::Fragment:
            shaderObject = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
            break;

        case iShaderObjectType::Vertex:
            shaderObject = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
            break;

        case iShaderObjectType::Geometry:
            shaderObject = glCreateShaderObjectARB(GL_GEOMETRY_SHADER_ARB);
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
        glDeleteObjectARB(id);
    }

    void iRenderer::linkShaderProgram(uint32 id, std::vector<uint32> objects)
    {
        auto object = objects.begin();
        while (objects.end() != object)
        {
            glAttachObjectARB(id, (*object));
            object++;
        }

        glLinkProgramARB(id);
    }

    uint32 iRenderer::createShaderProgram()
    {
        uint32 result = glCreateProgramObjectARB();

        return result;
    }

    void iRenderer::destroyShaderProgram(uint32 id)
    {
        con_assert(INVALID_ID != id, "invalid id");
        glDeleteProgram(id);
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
            glEnable(GL_LINE_SMOOTH);

            glEnable(GL_POINT_SMOOTH);

            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

            glDepthFunc(GL_LESS);

            glEnable(GL_DEPTH_TEST);

            glDepthMask(true);

            _vendorOGL = (const char *)glGetString(GL_VENDOR);
            _rendererOGL = (const char *)glGetString(GL_RENDERER);
            GLint major = 0;
            GLint minor = 0;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            _versionOGL = iaString::toString((int32)major) + "." + iaString::toString((int32)minor);
            _extensionsOGL = (const char *)glGetString(GL_EXTENSIONS);

            con_info("OpenGL Version : " << _versionOGL << endlTab
                                         << "OpenGL Vendor  : " << _vendorOGL << endlTab
                                         << "OpenGL Renderer: " << _rendererOGL);

            setClearColor(iaColor4f(0.0f, 0.0f, 0.0f, 0.0f));
            setClearDepth(1.0f);
            setStencilMask(0xff);

            clearColorBuffer();
            clearDepthBuffer();
            clearStencilBuffer();

            enableStencilTest(false);

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

        glMatrixMode(GL_PROJECTION);

        glLoadMatrixd(_projectionMatrix.getData());

        glMatrixMode(GL_MODELVIEW);

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::drawPoint(iaVector3f &a)
    {
        glBegin(GL_POINTS);
        glVertex3f(a._x, a._y, a._z);
        glEnd();
    }

    void iRenderer::drawBBox(const iAACubed &bbox)
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

        glEnd();
    }

    void iRenderer::drawBBox(const iAABoxd &bbox)
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

        glEnd();
    }

    void iRenderer::drawFilledBox(const iaVector3f &a, const iaVector3f &b)
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
        glEnd();
    }

    void iRenderer::drawLineStrip(const std::vector<iaVector3f> &line)
    {
        glBegin(GL_LINE_STRIP);

        for (int i = 0; i < line.size(); ++i)
        {
            glVertex3f(line[i]._x, line[i]._y, line[i]._z);
        }

        glEnd();
    }

    void iRenderer::drawLine(const iaVector3f &a, const iaVector3f &b)
    {
        glBegin(GL_LINES);
        glVertex3f(a._x, a._y, a._z);
        glVertex3f(b._x, b._y, b._z);
        glEnd();
    }

    void iRenderer::drawBillboard(const iaVector3f &o, const iaVector3f &u, const iaVector3f &v, iTexturePtr texture, float32 texScaleU, float32 texScaleV)
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
        glEnd();
    }

    void iRenderer::drawBillboard(const iaVector3f &o, const iaVector3f &u, const iaVector3f &v, iTexturePtr texture)
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
        glEnd();
    }

    void iRenderer::setOrtho(float32 left, float32 right, float32 bottom, float32 top, float32 nearplain, float32 farplain)
    {
        _projectionMatrix.ortho(left, right, bottom, top, nearplain, farplain);

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glMultMatrixd(_projectionMatrix.getData());

        glMatrixMode(GL_MODELVIEW);

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::setClearColor(const iaColor4f &color)
    {
        glClearColor(color._r, color._g, color._b, color._a);
    }

    void iRenderer::setClearDepth(float32 depth)
    {
        glClearDepth(depth);
    }

    GLenum getOGLEnum(iRenderStateValue value)
    {
        switch (value)
        {
        case iRenderStateValue::One:
            return GL_ONE;
        case iRenderStateValue::Zero:
            return GL_ZERO;
        case iRenderStateValue::DestinationColor:
            return GL_DST_COLOR;
        case iRenderStateValue::OneMinusDestinationColor:
            return GL_ONE_MINUS_DST_COLOR;
        case iRenderStateValue::SourceAlpha:
            return GL_SRC_ALPHA;
        case iRenderStateValue::OneMinusSourceAlpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case iRenderStateValue::DestinationAlpha:
            return GL_DST_ALPHA;
        case iRenderStateValue::OneMinusDestinationAlpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case iRenderStateValue::SourceColor:
            return GL_SRC_COLOR;
        case iRenderStateValue::OneMinusSourceColor:
            return GL_ONE_MINUS_SRC_COLOR;
        case iRenderStateValue::Front:
            return GL_FRONT;
        case iRenderStateValue::Back:
            return GL_BACK;
        case iRenderStateValue::Never:
            return GL_NEVER;
        case iRenderStateValue::Less:
            return GL_LESS;
        case iRenderStateValue::LessOrEqual:
            return GL_LEQUAL;
        case iRenderStateValue::Greater:
            return GL_GREATER;
        case iRenderStateValue::GreaterOrEqual:
            return GL_GEQUAL;
        case iRenderStateValue::Equal:
            return GL_EQUAL;
        case iRenderStateValue::NotEqual:
            return GL_NOTEQUAL;
        case iRenderStateValue::Always:
            return GL_ALWAYS;
        case iRenderStateValue::Keep:
            return GL_KEEP;
        case iRenderStateValue::Replace:
            return GL_REPLACE;
        case iRenderStateValue::Increment:
            return GL_INCR;
        case iRenderStateValue::IncrementWrap:
            return GL_INCR_WRAP;
        case iRenderStateValue::Decrement:
            return GL_DECR;
        case iRenderStateValue::DecrementWrap:
            return GL_DECR_WRAP;
        case iRenderStateValue::Invert:
            return GL_INVERT;
        }

        con_err("invalid value");
        return GL_ZERO;
    }

    void iRenderer::setStencilFunction(iRenderStateValue function, int32 ref, uint32 mask)
    {
        glStencilFunc(getOGLEnum(function), ref, mask);
    }

    void iRenderer::setStencilOperation(iRenderStateValue fail, iRenderStateValue zfail, iRenderStateValue zpass)
    {
        glStencilOp(getOGLEnum(fail), getOGLEnum(zfail), getOGLEnum(zpass));
    }

    void iRenderer::enableStencilTest(bool enable)
    {
        if (enable)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
    }

    void iRenderer::setStencilMask(uint8 mask)
    {
        glStencilMask(mask);
    }

    void iRenderer::clearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void iRenderer::clearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void iRenderer::clearStencilBuffer()
    {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    void iRenderer::getProjectionMatrix(iaMatrixd &matrix)
    {
        matrix = _projectionMatrix;
    }

    void iRenderer::setProjectionMatrix(const iaMatrixd &matrix)
    {
        _projectionMatrix = matrix;

        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

        glMultMatrixd(_projectionMatrix.getData());

        glMatrixMode(GL_MODELVIEW);

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::getViewport(iaRectanglei &rect)
    {
        rect = _viewport;
    }

    void iRenderer::setViewport(int32 x, int32 y, int32 width, int32 height)
    {
        _viewport.setX(x);
        _viewport.setY(y);
        _viewport.setWidth(width);
        _viewport.setHeight(height);

        glViewport(x, y, width, height);
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
        glReadPixels(x, y, width, height, glformat, GL_UNSIGNED_BYTE, data);
    }

    iRendererTexture *iRenderer::createTexture(int32 width, int32 height, int32 bytepp, iColorFormat format, unsigned char *data, iTextureBuildMode buildMode, iTextureWrapMode wrapMode)
    {
        int32 glformat = convertGLColorFormat(format);
        if (glformat == iRenderer::INVALID_ID)
        {
            return nullptr;
        }

        iRendererTexture *result = nullptr;

        result = new iRendererTexture();
        glGenTextures(1, (GLuint *)&(result->_id));

        glBindTexture(GL_TEXTURE_2D, result->_id);

        switch (wrapMode)
        {
        case iTextureWrapMode::Repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            break;

        case iTextureWrapMode::Clamp:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            break;

        case iTextureWrapMode::MirrorRepeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

            break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (buildMode == iTextureBuildMode::Normal)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, bytepp, width, height, 0, glformat, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            gluBuild2DMipmaps(GL_TEXTURE_2D, bytepp, width, height, glformat, GL_UNSIGNED_BYTE, data);
        }
        glFinish();

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

    void iRenderer::destroyTexture(iRendererTexture *texture)
    {
        con_assert(texture != nullptr, "zero pointer");

        if (texture != nullptr)
        {
            GLuint texId = (GLuint)((texture)->_id);

            if (glIsTexture(texId))
            {
                glDeleteTextures(1, &texId);
            }

            delete texture;
        }
    }

    void iRenderer::bindTexture(iTexturePtr texture, uint32 textureunit)
    {
        glActiveTexture(GL_TEXTURE0 + textureunit);

        if (texture != nullptr)
        {
            if (!texture->isDummy())
            {
                glBindTexture(GL_TEXTURE_2D, (texture->_rendererTexture)->_id);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, _dummyTextureID);
            }
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    void iRenderer::getCamWorldMatrix(iaMatrixd &matrix) const
    {
        matrix = _camWorldMatrix;
        matrix._pos += _worldOffset;
    }

    void iRenderer::getModelMatrix(iaMatrixd &matrix) const
    {
        matrix = _modelMatrix;
        matrix._pos += _worldOffset;
    }

    void iRenderer::setModelMatrix(const iaMatrixd &matrix)
    {
        _modelMatrix = matrix;
        _modelMatrix._pos -= _worldOffset;

        _modelViewMatrix = _viewMatrix;
        _modelViewMatrix *= _modelMatrix;

        glLoadMatrixd(_modelViewMatrix.getData());

        _dirtyModelViewProjectionMatrix = true;
    }

    void iRenderer::setViewMatrix(const iaMatrixd &viewMatrix)
    {
        _viewMatrix = viewMatrix;

        _worldOffset.set(0, 0, 0);
        _camWorldMatrix.identity();

        _modelViewMatrix = _viewMatrix;
        _modelViewMatrix *= _modelMatrix;

        glLoadMatrixd(_modelViewMatrix.getData());

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

    void iRenderer::setViewMatrix(const iaMatrixd &viewMatrix, const iaMatrixd &camMatrix)
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

        glLoadMatrixd(_modelViewMatrix.getData());

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

            glUniform4fv(_currentMaterial->_matSolidColor, 1, static_cast<GLfloat *>(color));
        }
    }

    void iRenderer::setTargetMaterial(iTargetMaterial *targetMaterial)
    {
        if (_currentMaterial->getShader() != nullptr)
        {
            if (_currentMaterial->_hasTargetMaterial)
            {
                glUniform3fv(_currentMaterial->_matAmbient, 1, static_cast<GLfloat *>(targetMaterial->getAmbient().getData()));

                glUniform3fv(_currentMaterial->_matDiffuse, 1, static_cast<GLfloat *>(targetMaterial->getDiffuse().getData()));

                glUniform3fv(_currentMaterial->_matSpecular, 1, static_cast<GLfloat *>(targetMaterial->getSpecular().getData()));

                glUniform3fv(_currentMaterial->_matSpecular, 1, static_cast<GLfloat *>(targetMaterial->getEmissive().getData()));

                glUniform1f(_currentMaterial->_matShininess, targetMaterial->getShininess());

                glUniform1f(_currentMaterial->_matAlpha, targetMaterial->getAlpha());
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

    void iRenderer::setTargetEmissive(iaColor3f &emissive)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive.getData());
    }

    void iRenderer::setTargetAmbient(iaColor3f &ambient)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient.getData());
    }

    void iRenderer::setTargetDiffuse(iaColor3f &diffuse)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.getData());
    }

    void iRenderer::setTargetSpecular(iaColor3f &specular)
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

            iRenderStateSet &stateset = _currentMaterial->getRenderStateSet();
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthTest)] == iRenderStateValue::On) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

            (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthMask)] == iRenderStateValue::On) ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);

            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Blend)] == iRenderStateValue::On) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

            (stateset._renderStates[static_cast<unsigned int>(iRenderState::CullFace)] == iRenderStateValue::On) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

            glActiveTexture(GL_TEXTURE0);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D0)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE1);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D1)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE2);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D2)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE3);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D3)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE4);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D4)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE5);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D5)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE6);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D6)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE7);
            (stateset._renderStates[static_cast<unsigned int>(iRenderState::Texture2D7)] == iRenderStateValue::On) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

            switch (stateset._renderStates[static_cast<unsigned int>(iRenderState::DepthFunc)])
            {
            case iRenderStateValue::Less:
                glDepthFunc(GL_LESS);

                break;

            case iRenderStateValue::LessOrEqual:
                glDepthFunc(GL_LEQUAL);

                break;

            case iRenderStateValue::Never:
                glDepthFunc(GL_NEVER);

                break;

            case iRenderStateValue::Equal:
                glDepthFunc(GL_EQUAL);

                break;

            case iRenderStateValue::Greater:
                glDepthFunc(GL_GREATER);

                break;

            case iRenderStateValue::NotEqual:
                glDepthFunc(GL_NOTEQUAL);

                break;

            case iRenderStateValue::GreaterOrEqual:
                glDepthFunc(GL_GEQUAL);

                break;

            case iRenderStateValue::Always:
                glDepthFunc(GL_ALWAYS);

                break;
            }

            switch (stateset._renderStates[static_cast<unsigned int>(iRenderState::CullFaceFunc)])
            {
            case iRenderStateValue::Front:
                glCullFace(GL_FRONT);

                break;

            case iRenderStateValue::Back:
                glCullFace(GL_BACK);

                break;
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            if (stateset._renderStates[static_cast<unsigned int>(iRenderState::Wireframe)] == iRenderStateValue::On ||
                forceWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
    }

    void iRenderer::drawCircle(float32 x, float32 y, float32 radius, int segments)
    {
        const float32 step = 2 * M_PI / static_cast<float32>(segments);
        float32 angle = 0;

        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; ++i)
        {
            glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
            angle += step;
        }
        glEnd();
    }

    void iRenderer::drawFilledCircle(float32 x, float32 y, float32 radius, int segments)
    {
        const float32 step = 2 * M_PI / static_cast<float32>(segments);
        float32 angle = 0;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for (int i = 0; i < segments; ++i)
        {
            glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
            angle -= step;
        }
        glVertex2f(x + radius, y);

        glEnd();
    }

    void iRenderer::drawRectangle(float32 x, float32 y, float32 width, float32 height)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x, y + height);
        glVertex2f(x + width, y + height);
        glVertex2f(x + width, y);
        glEnd();
    }

    void iRenderer::drawFilledRectangle(float32 x, float32 y, float32 width, float32 height)
    {
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x, y + height);
        glVertex2f(x + width, y + height);
        glVertex2f(x + width, y);
        glEnd();
    }

    void iRenderer::drawTextureTiled(float32 x, float32 y, float32 width, float32 height, iTexturePtr texture)
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
        glEnd();
    }

    void iRenderer::drawTexture(float32 x, float32 y, float32 width, float32 height, iTexturePtr texture)
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
        glEnd();
    }

    void iRenderer::drawTexture(float32 x, float32 y, iTexturePtr texture)
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
        glEnd();
    }

    void iRenderer::drawSprite(const iAtlas *sprite, uint32 frameIndex, const iaVector2f &pos)
    {
        iTexturePtr texture = sprite->getTexture();
        const iAtlas::Frame &frame = sprite->getFrame(frameIndex);

        iaVector2f position = pos;
        position -= frame._origin;
        iaVector2f size(frame._size._x * texture->getWidth(), frame._size._y * texture->getHeight());

        bindTexture(texture, 0);

        glBegin(GL_QUADS);
        glTexCoord2f(frame._pos._x, frame._pos._y + frame._size._y);
        glVertex2f(position._x, position._y + size._y);

        glTexCoord2f(frame._pos._x + frame._size._x, frame._pos._y + frame._size._y);
        glVertex2f(position._x + size._x, position._y + size._y);

        glTexCoord2f(frame._pos._x + frame._size._x, frame._pos._y);
        glVertex2f(position._x + size._x, position._y);

        glTexCoord2f(frame._pos._x, frame._pos._y);
        glVertex2f(position._x, position._y);
        glEnd();
    }

    iRenderTargetID iRenderer::createRenderTarget(uint32 width, uint32 height, iColorFormat format, iRenderTargetType renderTargetType, bool useDepthBuffer)
    {
        iRenderTargetID result = iRenderer::DEFAULT_RENDER_TARGET;
        GLenum glformat = convertGLColorFormat(format);
        con_assert(glformat != iRenderer::INVALID_ID, "invalid color format");

        if (glformat != iRenderer::INVALID_ID)
        {
            GLuint fbo;
            GLuint colorRenderBuffer;
            GLuint depthRenderBuffer;
            glGenFramebuffersEXT(1, &fbo);

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

            glGenRenderbuffersEXT(1, &colorRenderBuffer);

            glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorRenderBuffer);

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
                glDeleteRenderbuffersEXT(1, &colorRenderBuffer);

                if (useDepthBuffer)
                {
                    glDeleteRenderbuffersEXT(1, &depthRenderBuffer);
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

    void iRenderer::destroyRenderTarget(iRenderTargetID id)
    {
        auto iter = _renderTargets.find(id);
        if (iter != _renderTargets.end())
        {
            iRendererTarget renderTarget = (*iter).second;

            glDeleteRenderbuffersEXT(1, (GLuint *)&renderTarget._colorBuffer);

            if (renderTarget._hasDepth)
            {
                glDeleteRenderbuffersEXT(1, (GLuint *)&renderTarget._depthBuffer);
            }

            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glDeleteFramebuffersEXT(1, (GLuint *)&renderTarget._frameBufferObject);

            // restore current render target
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _currentRenderTarget);

            _renderTargets.erase(iter);
        }
        else
        {
            con_err("invalid render target id " << id);
        }
    }

    void iRenderer::setRenderTarget(iRenderTargetID id)
    {
        // the ID is also the frame buffer object ID
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);

        _currentRenderTarget = id;
    }

    iRenderTargetID iRenderer::getRenderTarget() const
    {
        return _currentRenderTarget;
    }

    void iRenderer::destroyBuffer(uint32 bufferID)
    {
        if (glIsBuffer(bufferID))
        {
            glDeleteBuffers(1, (GLuint *)&bufferID);
        }
    }

    void iRenderer::destroyVertexArray(uint32 bufferID)
    {
        if (glIsBuffer(bufferID))
        {
            glDeleteVertexArrays(1, (GLuint *)&bufferID);
        }
    }

    void iRenderer::createBuffers(float64 timeLimit)
    {
        iaTime endTime = iaTime::getNow();
        endTime += iaTime::fromMilliseconds(timeLimit);
        std::deque<std::pair<iMeshPtr, iMeshBuffersPtr>>::iterator entryIter;

        iMeshPtr mesh;
        iMeshBuffersPtr meshBuffers;
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

            if (iaTime::getNow() > endTime)
            {
                break;
            }
        }
    }

    iMeshBuffersPtr iRenderer::createBuffersAsync(iMeshPtr mesh)
    {
        iMeshBuffers *meshBuffer = new iMeshBuffers();

        iMeshBuffersPtr result = iMeshBuffersPtr(meshBuffer);

        _requestedBuffersMutex.lock();
        _requestedBuffers.push_back(std::pair<iMeshPtr, iMeshBuffersPtr>(mesh, result));
        _requestedBuffersMutex.unlock();

        return result;
    }

    void iRenderer::initBuffers(iMeshPtr mesh, iMeshBuffersPtr meshBuffers)
    {
        uint32 vao = 0;
        uint32 ibo = 0;
        uint32 vbo = 0;

        glGenVertexArrays(1, (GLuint *)&vao);

        meshBuffers->setVertexArrayObject(vao);
        glBindVertexArray(meshBuffers->getVertexArrayObject());

        if (mesh->getIndexData() != nullptr)
        {
            glGenBuffers(1, (GLuint *)&ibo);

            meshBuffers->setIndexBufferObject(ibo);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBuffers->getIndexBufferObject());

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexDataSize(), mesh->getIndexData(), GL_STATIC_DRAW);
        }

        glGenBuffers(1, (GLuint *)&vbo);

        meshBuffers->setVertexBufferObject(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, meshBuffers->getVertexBufferObject());

        glBufferData(GL_ARRAY_BUFFER, mesh->getVertexDataSize(), mesh->getVertexData(), GL_STATIC_DRAW);

        uint32 location = 0;
        uint32 offset = 0;

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), BUFFER_OFFSET(offset));

        offset += 3 * sizeof(float32);

        if (mesh->hasNormals())
        {
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), BUFFER_OFFSET(offset));

            offset += 3 * sizeof(float32);
        }

        for (int i = 0; i < 4; ++i)
        {
            if (mesh->hasTextureUnit(i))
            {
                glEnableVertexAttribArray(2 + i);

                glVertexAttribPointer(2 + i, 2, GL_FLOAT, GL_FALSE, mesh->getVertexSize(), BUFFER_OFFSET(offset));

                offset += 2 * sizeof(float32);
            }
        }

        meshBuffers->setIndexesCount(mesh->getIndexesCount());
        meshBuffers->setTrianglesCount(mesh->getTrianglesCount());
        meshBuffers->setVertexCount(mesh->getVertexCount());

        meshBuffers->setReady();
    }

    iMeshBuffersPtr iRenderer::createBuffers(iMeshPtr mesh)
    {
        iMeshBuffers *meshBuffer = new iMeshBuffers();
        iMeshBuffersPtr result = iMeshBuffersPtr(meshBuffer);

        initBuffers(mesh, result);

        return result;
    }

    void iRenderer::createBuffers(iInstancer *instancer)
    {
        if (instancer->getInstanceArrayObject() == 0)
        {
            uint32 instanceArrayObject = 0;

            glGenBuffers(1, (GLuint *)&instanceArrayObject);

            glBindBuffer(GL_ARRAY_BUFFER, instanceArrayObject);

            glBufferData(GL_ARRAY_BUFFER, instancer->getInstanceDataBufferSize(), nullptr, GL_DYNAMIC_DRAW);

            instancer->setInstanceArrayObject(instanceArrayObject);
        }
    }

    void iRenderer::drawMesh(iMeshBuffersPtr meshBuffers, iInstancer *instancer)
    {
        iaMatrixd idMatrix;
        setModelMatrix(idMatrix);

        createBuffers(instancer); // TODO that's not a good place to initialize the buffer

        writeShaderParameters();

        glBindVertexArray(meshBuffers->getVertexArrayObject());

        glBindBuffer(GL_ARRAY_BUFFER, instancer->getInstanceArrayObject());

        glBufferSubData(GL_ARRAY_BUFFER, 0, instancer->getInstanceSize() * instancer->getInstanceCount(), instancer->getInstanceDataBuffer());

        glEnableVertexAttribArray(3);

        glEnableVertexAttribArray(4);

        glEnableVertexAttribArray(5);

        glEnableVertexAttribArray(6);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void *)(0 * sizeof(float32)));

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void *)(4 * sizeof(float32)));

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void *)(8 * sizeof(float32)));

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, instancer->getInstanceSize(), (void *)(12 * sizeof(float32)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glDrawElementsInstancedARB(GL_TRIANGLES, meshBuffers->getIndexesCount(), GL_UNSIGNED_INT, 0, instancer->getInstanceCount());

        glBindVertexArray(0);

        _stats._indicesInstanced += meshBuffers->getIndexesCount() * instancer->getInstanceCount();
        _stats._trianglesInstanced += meshBuffers->getTrianglesCount() * instancer->getInstanceCount();
        _stats._verticesInstanced += meshBuffers->getVertexCount() * instancer->getInstanceCount();
    }

    int32 iRenderer::getShaderPropertyID(uint32 programID, const char *name)
    {
        int32 result = glGetUniformLocation(programID, name);

        return result;
    }

    void iRenderer::writeShaderParameters()
    {
        if (_currentMaterial->getShader() != nullptr)
        {
            if (_currentMaterial->_hasDirectionalLight)
            {
                glUniform3fv(_currentMaterial->_lightOrientation, 1, static_cast<GLfloat *>(_lights[0]._position.getData()));
                glUniform3fv(_currentMaterial->_lightAmbient, 1, static_cast<GLfloat *>(_lights[0]._ambient.getData()));
                glUniform3fv(_currentMaterial->_lightDiffuse, 1, static_cast<GLfloat *>(_lights[0]._diffuse.getData()));
                glUniform3fv(_currentMaterial->_lightSpecular, 1, static_cast<GLfloat *>(_lights[0]._specular.getData()));
            }

            if (_currentMaterial->_hasEyePosition)
            {
                iaVector3f eyePosition(_camWorldMatrix._pos._x, _camWorldMatrix._pos._y, _camWorldMatrix._pos._z);
                glUniform3fv(_currentMaterial->_eyePosition, 1, static_cast<GLfloat *>(eyePosition.getData()));
            }

            if (_currentMaterial->_hasModelViewProjectionMatrix)
            {
                updateModelViewProjectionMatrix();
                iaMatrixf modelViewProjection;
                for (int i = 0; i < 16; ++i)
                {
                    modelViewProjection[i] = _modelViewProjectionMatrix[i];
                }
                glUniformMatrix4fv(_currentMaterial->_mvp_matrix, 1, false, modelViewProjection.getData());
            }

            if (_currentMaterial->_hasModelMatrix)
            {
                iaMatrixf model;
                for (int i = 0; i < 16; ++i)
                {
                    model[i] = _modelMatrix[i];
                }
                glUniformMatrix4fv(_currentMaterial->_model_matrix, 1, false, model.getData());
            }
        }
    }

    void iRenderer::drawMesh(iMeshBuffersPtr meshBuffers)
    {
        writeShaderParameters();

        glBindVertexArray(meshBuffers->getVertexArrayObject());
        glDrawElements(GL_TRIANGLES, meshBuffers->getIndexesCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        _stats._vertices += meshBuffers->getVertexCount();
        _stats._indices += meshBuffers->getIndexesCount();
        _stats._triangles += meshBuffers->getTrianglesCount();
    }

    void iRenderer::drawPoint(float32 x, float32 y)
    {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }

    void iRenderer::drawLine(float32 x1, float32 y1, float32 x2, float32 y2)
    {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    }

    void iRenderer::setColorExt(iaColor4f color)
    {
        if (_currentMaterial->_hasSolidColor)
        {
            if (_currentMaterial->getShader() != nullptr)
            {
                uint32 program = _currentMaterial->getShader()->getProgram();
                glUniform4fv(_currentMaterial->_matSolidColor, 1, static_cast<GLfloat *>(color.getData()));
            }
        }
    }

    void iRenderer::setColor(iaColor4f color)
    {
        _color = color;
        glColor4f(_color._r, _color._g, _color._b, _color._a);
    }

    void iRenderer::setColor(float32 r, float32 g, float32 b, float32 a)
    {
        _color._r = r;
        _color._g = g;
        _color._b = b;
        _color._a = a;
        glColor4f(_color._r, _color._g, _color._b, _color._a);
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
            uint32 textLength = text.getLength();
            bool changecolor = false;
            bool donotdraw = false;
            std::vector<iCharacterDimensions> characters = _font->getCharacters();

            bindTexture(_font->getTexture(), 0);

            glPushMatrix();

            glTranslatef(x, y, 0);

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

                if (ascii_code == '\n')
                {
                    renderPos._x = 0;
                    renderPos._y += _fontSize * _fontLineHeight;
                    donotdraw = true;
                }
                else if (maxWidth != 0)
                {
                    if (ascii_code == ' ')
                    {
                        int j = 1;
                        while ((i + j < text.getLength()) && (text[i + j] != ' '))
                        {
                            temptext[j - 1] = text[i + j];
                            j++;
                        }
                        temptext[j] = 0;

                        if (renderPos._x + _font->measureWidth(temptext, _fontSize) >= maxWidth)
                        {
                            renderPos._x = 0;
                            renderPos._y += _fontSize * _fontLineHeight;
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

            glEnd();

            glPopMatrix();
        }
    }

    void iRenderer::setLightPosition(int32 lightnum, const iaVector4d &pos)
    {
        // TODO fix with world offset
        _lights[lightnum]._position.set(pos._x, pos._y, pos._z, pos._w);
        glLightfv(GL_LIGHT0 + lightnum, GL_POSITION, _lights[lightnum]._position.getData());
    }

    void iRenderer::setLightAmbient(int32 lightnum, iaColor4f &ambient)
    {
        _lights[lightnum]._ambient = ambient;
        glLightfv(GL_LIGHT0 + lightnum, GL_AMBIENT, (GLfloat *)ambient.getData());
    }

    void iRenderer::setLightDiffuse(int32 lightnum, iaColor4f &diffuse)
    {
        _lights[lightnum]._diffuse = diffuse;
        glLightfv(GL_LIGHT0 + lightnum, GL_DIFFUSE, (GLfloat *)diffuse.getData());
    }

    void iRenderer::setLightSpecular(int32 lightnum, iaColor4f &specular)
    {
        _lights[lightnum]._specular = specular;
        glLightfv(GL_LIGHT0 + lightnum, GL_SPECULAR, (GLfloat *)specular.getData());
    }

    /*!
    \bug in debug mode the rainbow colors are wrong. it's changing back and forth
    */
    void iRenderer::drawParticles(float32 x, float32 y, float32 angle, iParticle2D *particles, int32 particleCount, iaGradientColor4f *rainbow)
    {
        iaVector2f a, b, c, d, u, v;
        iaColor4f color;

        glPushMatrix();

        glTranslatef(x, y, 0);

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

        glEnd();

        glPopMatrix();

        // todo maybe we should count this during cull process
        _stats._vertices += particleCount * 4;
        _stats._indices += particleCount * 4;
        _stats._triangles += particleCount * 2;
    }

    void iRenderer::drawParticles(const std::deque<iParticle> &particles, const iaGradientColor4f &rainbow)
    {
        iaVector4f camright;
        camright.set(_camWorldMatrix._right._x, _camWorldMatrix._right._y, _camWorldMatrix._right._z, 0);

        iaVector4f camtop;
        camtop.set(_camWorldMatrix._top._x, _camWorldMatrix._top._y, _camWorldMatrix._top._z, 0);

        iaMatrixf inv = _modelMatrix.convert<float32>();
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

                right._x = rightPreComp._x;
                right._y = rightPreComp._y;
                right._z = rightPreComp._z;
                right *= size;
                top._x = topPreComp._x;
                top._y = topPreComp._y;
                top._z = topPreComp._z;
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

        glEnd();

        // todo maybe we should count this during cull process
        _stats._vertices += static_cast<uint32>(particles.size()) * 4;
        _stats._indices += static_cast<uint32>(particles.size()) * 4;
        _stats._triangles += static_cast<uint32>(particles.size()) * 2;
    }

    void iRenderer::drawVelocityOrientedParticles(const std::deque<iParticle> &particles, const iaGradientColor4f &rainbow)
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

        glEnd();

        // todo maybe we should count this during cull process
        _stats._vertices += static_cast<uint32>(particles.size()) * 4;
        _stats._indices += static_cast<uint32>(particles.size()) * 4;
        _stats._triangles += static_cast<uint32>(particles.size()) * 2;
    }

    iaVector3d iRenderer::project(const iaVector3d &objectSpacePos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport)
    {
        iaVector4d in(objectSpacePos._x, objectSpacePos._y, objectSpacePos._z, 1);
        iaVector4d out;
        iaVector3d result;

        iaMatrixd modelViewProjection = projection;
        modelViewProjection *= modelview;
        out = modelViewProjection * in;
        out[0] /= out._w;
        out[1] /= out._w;
        out[2] /= out._w;

        result._x = static_cast<float64>(viewport.getWidth()) * (out._x + 1.0) / 2.0;
        result._y = static_cast<float64>(viewport.getHeight()) * (1.0 - ((out._y + 1.0) / 2.0));
        result._z = out._z;

        return result;
    }

    iaVector3d iRenderer::unProject(const iaVector3d &screenpos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport)
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

}; // namespace igor
