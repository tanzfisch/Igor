// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderer.h>

#include <igor/renderer/utils/iRendererUtils.h>

#include <igor/simulation/iParticleSystem.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/iResourceManager.h>

#include <deque>
#include <sstream>

namespace igor
{
    static const uint32 MAX_POINT_VERTICES = 10000;

    static const uint32 MAX_LINES = 10000;
    static const uint32 MAX_LINE_VERTICES = MAX_LINES * 2;

    static const uint32 MAX_QUADS = 10000;
    static const uint32 MAX_QUAD_VERTICES = MAX_QUADS * 4;
    static const uint32 MAX_QUAD_INDICES = MAX_QUADS * 6;

    static const uint32 MAX_TRIANGLES = 10000;
    static const uint32 MAX_TRIANGLE_VERTICES = MAX_TRIANGLES * 3;
    static const uint32 MAX_TRIANGLE_INDICES = MAX_TRIANGLES * 3;

    static const iaVector2f QUAD_TEXTURE_COORDS[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
    static const iaVector3f QUAD_VERTEX_POSITIONS[] = {{-0.5f, -0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}};

    // flat vertex definition
    struct iFlatVertex
    {
        iaVector3f _pos;
        iaColor4f _color;
    };

    // textured vertex definition
    struct iTexturedVertex
    {
        iaVector3f _pos;
        iaColor4f _color;
        iaVector2f _texCoord0;
        float32 _texIndex0;
    };

    /// data to render points
    struct iRenderDataPoints
    {
        /*! point vertex buffer
         */
        iVertexBufferPtr _vertexBuffer;

        /*! point vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! point vertex count
         */
        uint32 _vertexCount;

        /*! oing vertex data
         */
        iFlatVertex *_vertexData = nullptr;

        /*! pointer to current position in point vertex data
         */
        iFlatVertex *_vertexDataPtr = nullptr;
    };

    /// data to render lines
    struct iRenderDataLines
    {
        /*! line vertex buffer
         */
        iVertexBufferPtr _vertexBuffer;

        /*! line vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! line vertex count
         */
        uint32 _vertexCount;

        /*! line vertex data
         */
        iFlatVertex *_vertexData = nullptr;

        /*! pointer to current position in line vertex data
         */
        iFlatVertex *_vertexDataPtr = nullptr;
    };

    /// data to render quads
    struct iRendererDataQuads
    {
        /*! quad vertex buffer
         */
        iVertexBufferPtr _vertexBuffer;

        /*! quad vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! quad vertex count
         */
        uint32 _vertexCount;

        /*! quad vertex count
         */
        uint32 _indexCount;

        /*! quad vertex data
         */
        iFlatVertex *_vertexData = nullptr;

        /*! pointer to current position in quad vertex data
         */
        iFlatVertex *_vertexDataPtr = nullptr;
    };

    /// data to render triangles
    struct iRendererDataTriangles
    {
        /*! vertex buffer
         */
        iVertexBufferPtr _vertexBuffer;

        /*! vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! vertex count
         */
        uint32 _vertexCount;

        /*! vertex count
         */
        uint32 _indexCount;

        /*! vertex data
         */
        iFlatVertex *_vertexData = nullptr;

        /*! pointer to current position in quad vertex data
         */
        iFlatVertex *_vertexDataPtr = nullptr;

        /*! index buffer
         */
        iIndexBufferPtr _indexBuffer;

        /*! index data
         */
        uint32 *_indexData = nullptr;

        /*! pointer to current position in index data
         */
        uint32 *_indexDataPtr = nullptr;
    };

    /// data to render textured quads
    struct iRendererDataTexturedQuads
    {
        /*! textured quad vertex buffer
         */
        iVertexBufferPtr _vertexBuffer;

        /*! textured quad vertex array
         */
        iVertexArrayPtr _vertexArray;

        /*! textured quad vertex count
         */
        uint32 _vertexCount;

        /*! textured quad vertex count
         */
        uint32 _indexCount;

        /*! textured quad vertex data
         */
        iTexturedVertex *_vertexData = nullptr;

        /*! pointer to current position in textured quad vertex data
         */
        iTexturedVertex *_vertexDataPtr = nullptr;

        /*! quad textures
         */
        iTexturePtr _textures[MAX_TEXTURE_UNITS];

        /*! next available texture index for quads
         */
        uint32 _nextTextureIndex;
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

    /*! render data set names
     */
    enum class iRenderDataSet
    {
        NoDataSet,
        Points,
        Lines,
        Triangles,
        Quads,
        TexturedQuads,
        Buffer,
        Particles // 3d particles
    };

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

    /*! all the data needed
     */
    struct iRendererData
    {
        /*! points render data
         */
        iRenderDataPoints _points;

        /*! lines render data
         */
        iRenderDataLines _lines;

        /*! triangles render data
         */
        iRendererDataTriangles _triangles;

        /*! quads render data
         */
        iRendererDataQuads _quads;

        /*! textured quads render data
         */
        iRendererDataTexturedQuads _texQuads;

        ////////// MATERIALS ////////////
        /*! material for flat shaded rendering
         */
        iMaterialPtr _flatShader;

        /*! material for textured rendering
         */
        iMaterialPtr _textureShader;

        /*! material for flat shaded rendering
         */
        iMaterialPtr _flatShaderBlend;

        /*! material for textured rendering
         */
        iMaterialPtr _textureShaderBlend;

        /*! the current material in use
         */
        iMaterialPtr _currentMaterial;

        //////////// SHARED DATA ///////////
        /*! quad index buffer
         */
        iIndexBufferPtr _sharedQuadIndexBuffer;

        /*! stores which render dataset was used last
         */
        iRenderDataSet _lastRenderDataSetUsed;
        ///////// BUFFERS ///////
        iaMutex _requestedBuffersMutex;
        std::deque<std::pair<iMeshPtr, iMeshBuffersPtr>> _requestedBuffers;

        /////////// MATRICES /////////////
        iaMatrixd _modelMatrix;
        iaMatrixd _viewMatrix;
        iaMatrixd _projectionMatrix;
        iaMatrixd _modelViewMatrix;
        iaMatrixd _modelViewProjectionMatrix;

        iaMatrixd _camMatrix;

        ///////////// LIGHTS //////////////
        /*! list of lights
         */
        std::map<int32, iRendererLight> _lights;

        /////////// SETTINGS //////
        /*! if true render order will be kept by the cost of more draw calls beeing used
         */
        bool _keepRenderOrder = true;

        /*! if true everything will be rendered in wireframe mode
         */
        bool _wireframeEnabled = false;

        /*! line width
         */
        float32 _lineWidth = 1.0f;

        /*! point size
         */
        float32 _pointSize = 1.0;

        /*! font used
         */
        iTextureFontPtr _font;

        /*! font size
         */
        float32 _fontSize = 10.0f;

        /*! font line height
         */
        float32 _fontLineHeight = 1.15f;

        /*! current viewport dimensions
         */
        iaRectanglei _viewport;

        /*! fallback texture used when there is no texture available
         */
        iTexturePtr _fallbackTexture;

        /*! current render target
         */
        uint32 _currentRenderTarget = DEFAULT_RENDER_TARGET;

        /*! map of render targets
         */
        std::map<uint32, iRendererTarget> _renderTargets;

        /*! color id value
         */
        iaColor4f _solidColor;

        ////// stats ////
        iRenderer::iRendererStats _stats;

        //////// INFO ///////
        iaString _vendor;
        iaString _renderer;
        iaString _version;
        iaString _extensions;
    };

    iRenderer::iRenderer()
    {
        _data = std::make_unique<iRendererData>();
    }

    iRenderer::~iRenderer()
    {
        deinit();
    }

    void iRenderer::init()
    {
        /////////// CHECK VERSION ////////////
        _data->_vendor = (const char *)glGetString(GL_VENDOR);
        _data->_renderer = (const char *)glGetString(GL_RENDERER);
        GLint major = 0;
        GLint minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        _data->_version = iaString::toString((int32)major) + "." + iaString::toString((int32)minor);
        _data->_extensions = (const char *)glGetString(GL_EXTENSIONS);

        con_info("OpenGL Version : " << _data->_version << endlTab
                                     << "OpenGL Vendor  : " << _data->_vendor << endlTab
                                     << "OpenGL Renderer: " << _data->_renderer);

        con_assert_sticky(major >= 4 && minor >= 5, "minimum OpenGL version is 4.5");

        /////////// LINES //////////////
        auto &lines = _data->_lines;
        lines._vertexArray = iVertexArray::create();

        lines._vertexBuffer = iVertexBuffer::create(MAX_LINE_VERTICES * sizeof(iFlatVertex));
        lines._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        lines._vertexArray->addVertexBuffer(lines._vertexBuffer);

        lines._vertexData = new iFlatVertex[MAX_LINE_VERTICES];
        lines._vertexDataPtr = lines._vertexData;
        lines._vertexCount = 0;

        //////////// POINTS /////////////
        auto &points = _data->_points;
        points._vertexArray = iVertexArray::create();

        points._vertexBuffer = iVertexBuffer::create(MAX_POINT_VERTICES * sizeof(iFlatVertex));
        points._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        points._vertexArray->addVertexBuffer(points._vertexBuffer);

        points._vertexData = new iFlatVertex[MAX_POINT_VERTICES];
        points._vertexDataPtr = points._vertexData;
        points._vertexCount = 0;

        //////////// FLAT TRIANGLES /////////////
        auto &triangles = _data->_triangles;

        triangles._vertexArray = iVertexArray::create();

        triangles._vertexBuffer = iVertexBuffer::create(MAX_TRIANGLE_VERTICES * sizeof(iFlatVertex));
        triangles._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        triangles._vertexArray->addVertexBuffer(triangles._vertexBuffer);

        triangles._indexBuffer = iIndexBuffer::create(MAX_TRIANGLE_INDICES);
        triangles._vertexArray->setIndexBuffer(triangles._indexBuffer);

        triangles._vertexData = new iFlatVertex[MAX_TRIANGLE_VERTICES];
        triangles._vertexDataPtr = triangles._vertexData;

        triangles._indexData = new uint32[MAX_TRIANGLE_INDICES];
        triangles._indexDataPtr = triangles._indexData;

        triangles._vertexCount = 0;
        triangles._indexCount = 0;

        //////// QUADS comon data ///////////
        uint32 sharedQuadIndexData[MAX_QUAD_INDICES];

        for (uint32 i = 0; i < MAX_QUADS; ++i)
        {
            sharedQuadIndexData[i * 6 + 0] = i * 4 + 0;
            sharedQuadIndexData[i * 6 + 1] = i * 4 + 1;
            sharedQuadIndexData[i * 6 + 2] = i * 4 + 3;

            sharedQuadIndexData[i * 6 + 3] = i * 4 + 1;
            sharedQuadIndexData[i * 6 + 4] = i * 4 + 2;
            sharedQuadIndexData[i * 6 + 5] = i * 4 + 3;
        }

        _data->_sharedQuadIndexBuffer = iIndexBuffer::create(MAX_QUAD_INDICES, sharedQuadIndexData);

        //////////// FLAT QUADS /////////////
        auto &quads = _data->_quads;
        quads._vertexArray = iVertexArray::create();

        quads._vertexBuffer = iVertexBuffer::create(MAX_QUAD_VERTICES * sizeof(iFlatVertex));
        quads._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        quads._vertexArray->addVertexBuffer(quads._vertexBuffer);
        quads._vertexArray->setIndexBuffer(_data->_sharedQuadIndexBuffer);

        quads._vertexData = new iFlatVertex[MAX_QUAD_VERTICES];
        quads._vertexDataPtr = quads._vertexData;
        quads._vertexCount = 0;
        quads._indexCount = 0;

        //////////// TEXTURED QUADS /////////////
        auto &texQuads = _data->_texQuads;
        texQuads._vertexArray = iVertexArray::create();

        texQuads._vertexBuffer = iVertexBuffer::create(MAX_QUAD_VERTICES * sizeof(iTexturedVertex));
        texQuads._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}, {iShaderDataType::Float2}, {iShaderDataType::Float}});
        texQuads._vertexArray->addVertexBuffer(texQuads._vertexBuffer);
        texQuads._vertexArray->setIndexBuffer(_data->_sharedQuadIndexBuffer);

        texQuads._vertexData = new iTexturedVertex[MAX_QUAD_VERTICES];
        texQuads._vertexDataPtr = texQuads._vertexData;
        texQuads._vertexCount = 0;
        texQuads._indexCount = 0;
        texQuads._nextTextureIndex = 0;

        /////////// OGL //////////
#if defined(__IGOR_DEBUG__) && defined(GL_DEBUG_SEVERITY_HIGH) // TODO can we drop this now? GL_DEBUG_SEVERITY_HIGH
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(onOGLDebugOutput, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        setStencilMask(0xff);
        clearStencilBuffer();
        setStencilTestActive(false);

        ///////////// MATERIALS ////////////
        _data->_flatShader = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/flat_shaded.mat", false);
        _data->_flatShaderBlend = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/flat_shaded_blend.mat", false);
        _data->_textureShader = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/texture_shaded.mat", false);
        _data->_textureShaderBlend = iMaterialResourceFactory::getInstance().loadMaterial("igor/materials/texture_shaded_blend.mat", false);

        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;
        _data->_currentMaterial.reset();

        ////////////// generate textures //////////
        iParameters paramFallback({{"name", iaString("fallback_texture")},
                                   {"type", iaString("texture")},
                                   {"cacheMode", iResourceCacheMode::Keep},
                                   {"generate", true},
                                   {"pattern", iTexturePattern::CheckerBoard},
                                   {"primary", iaColor4f::black},
                                   {"secondary", iaColor4f::magenta},
                                   {"width", 128},
                                   {"height", 128}});

        _data->_fallbackTexture = std::dynamic_pointer_cast<iTexture>(iResourceManager::getInstance().loadResource(paramFallback));

        iParameters paramWhite({{"name", iaString("white_texture")},
                                {"type", iaString("texture")},
                                {"cacheMode", iResourceCacheMode::Keep},
                                {"generate", true},
                                {"pattern", iTexturePattern::SolidColor},
                                {"primary", iaColor4f::white},
                                {"width", 1},
                                {"height", 1}});

        std::dynamic_pointer_cast<iTexture>(iResourceManager::getInstance().loadResource(paramWhite));
    }

    void iRenderer::deinit()
    {
        /////////// MATERIALS ////////
        _data->_flatShader = nullptr;
        _data->_flatShaderBlend = nullptr;
        _data->_textureShader = nullptr;
        _data->_textureShaderBlend = nullptr;
        _data->_currentMaterial = nullptr;

        /////////// TEXTURES ////////
        _data->_fallbackTexture = nullptr;
        _data->_font = nullptr;

        /////////// LINES //////////
        auto &lines = _data->_lines;
        lines._vertexBuffer = nullptr;
        lines._vertexArray = nullptr;
        delete[] lines._vertexData;
        lines._vertexDataPtr = lines._vertexData = nullptr;

        /////////// POINTS //////////
        auto &points = _data->_points;
        points._vertexBuffer = nullptr;
        points._vertexArray = nullptr;
        delete[] points._vertexData;
        points._vertexDataPtr = points._vertexData = nullptr;

        /////////// FLAT SHADED TRIANGLES //////////
        auto &triangles = _data->_triangles;
        triangles._vertexBuffer = nullptr;
        triangles._vertexArray = nullptr;
        delete[] triangles._vertexData;
        triangles._vertexDataPtr = triangles._vertexData = nullptr;
        triangles._indexBuffer = nullptr;
        delete[] triangles._indexData;
        triangles._indexData = nullptr;

        /////////// FLAT SHADED QUADS //////////
        auto &quads = _data->_quads;
        quads._vertexBuffer = nullptr;
        quads._vertexArray = nullptr;
        delete[] quads._vertexData;
        quads._vertexDataPtr = quads._vertexData = nullptr;

        /////////// TEXTURED QUADS //////////
        auto &texQuads = _data->_texQuads;
        texQuads._vertexBuffer = nullptr;
        texQuads._vertexArray = nullptr;
        delete[] texQuads._vertexData;
        texQuads._vertexDataPtr = texQuads._vertexData = nullptr;
        for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
        {
            texQuads._textures[i] = nullptr;
        }

        /////////// SHARED QUAD INDICES //////////
        _data->_sharedQuadIndexBuffer = nullptr;
    }

    void iRenderer::beginFrame()
    {
    }

    void iRenderer::endFrame()
    {
        flush();
        setWireframeEnabled(false);
    }

    __IGOR_INLINE__ int32 iRenderer::beginTexturedQuad(const iTexturePtr &texture)
    {
        auto &texQuads = _data->_texQuads;

        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::TexturedQuads)
        {
            flushLastUsed();
        }

        if (texQuads._vertexCount >= MAX_QUAD_VERTICES)
        {
            flushTexQuads();
        }

        int32 textureIndex = -1;
        for (uint32_t i = 0; i < texQuads._nextTextureIndex; i++)
        {
            if (texQuads._textures[i] == texture)
            {
                textureIndex = (float32)i;
                break;
            }
        }

        if (textureIndex == -1)
        {
            if (texQuads._nextTextureIndex >= MAX_TEXTURE_UNITS)
            {
                flushTexQuads();
            }

            textureIndex = texQuads._nextTextureIndex;
            texQuads._textures[texQuads._nextTextureIndex] = texture;
            texQuads._nextTextureIndex++;
        }

        return textureIndex;
    }

    __IGOR_INLINE__ void iRenderer::endTexturedQuad()
    {
        auto &texQuads = _data->_texQuads;
        texQuads._vertexCount += 4;
        texQuads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::TexturedQuads;
    }

    void iRenderer::drawTexturedQuadInternal(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2f &tiling)
    {
        (color._a == 1.0 && !blend) ? setMaterial(_data->_textureShader) : setMaterial(_data->_textureShaderBlend);

        const int32 textureIndex = beginTexturedQuad(texture);

        auto &texQuads = _data->_texQuads;
        texQuads._vertexDataPtr->_pos = v1;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0 = QUAD_TEXTURE_COORDS[0];
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v2;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0._x = QUAD_TEXTURE_COORDS[1]._x;
        texQuads._vertexDataPtr->_texCoord0._y = QUAD_TEXTURE_COORDS[1]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v3;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0._x = QUAD_TEXTURE_COORDS[2]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord0._y = QUAD_TEXTURE_COORDS[2]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v4;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0._x = QUAD_TEXTURE_COORDS[3]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord0._y = QUAD_TEXTURE_COORDS[3]._y;
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        endTexturedQuad();
    }

    void iRenderer::drawFrameInternal(const iaMatrixf &matrix, const iAtlasPtr &atlas, uint32 frameIndex, const iaColor4f &color, bool blend)
    {
        (color._a == 1.0 && !blend) ? setMaterial(_data->_textureShader) : setMaterial(_data->_textureShaderBlend);

        const int32 textureIndex = beginTexturedQuad(atlas->getTexture());

        const iAtlas::iFrame &frame = atlas->getFrame(frameIndex);

        // TODO offset origin
        const iaVector3f offset; // (frame._origin._x, frame._origin._y, 0.0);

        auto &texQuads = _data->_texQuads;
        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[0];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0 = frame._rect.getTopLeft();
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[1];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0 = frame._rect.getBottomLeft();
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[2];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0 = frame._rect.getBottomRight();
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[3];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord0 = frame._rect.getTopRight();
        texQuads._vertexDataPtr->_texIndex0 = textureIndex;
        texQuads._vertexDataPtr++;

        endTexturedQuad();
    }

    void iRenderer::drawPointInternal(const iaVector3f &v, const iaColor4f &color)
    {
        auto &points = _data->_points;

        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Points)
        {
            flushLastUsed();
        }

        if (points._vertexCount >= MAX_POINT_VERTICES)
        {
            flushPoints();
        }

        (color._a == 1.0) ? setMaterial(_data->_flatShader) : setMaterial(_data->_flatShaderBlend);

        points._vertexDataPtr->_pos = v;
        points._vertexDataPtr->_color = color;
        points._vertexDataPtr++;

        points._vertexCount++;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Points;
    }

    void iRenderer::drawQuadInternal(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iaColor4f &color)
    {
        auto &quads = _data->_quads;

        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Quads)
        {
            flushLastUsed();
        }

        if (quads._vertexCount >= MAX_QUAD_VERTICES)
        {
            flushQuads();
        }

        (color._a == 1.0) ? setMaterial(_data->_flatShader) : setMaterial(_data->_flatShaderBlend);

        quads._vertexDataPtr->_pos = v1;
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = v2;
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = v3;
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = v4;
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexCount += 4;
        quads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Quads;
    }

    template <>
    void iRenderer::drawFrame(const iaMatrix<float32> &matrix, const iAtlasPtr &atlas, uint32 frameIndex, const iaColor4f &color, bool blend)
    {
        drawFrameInternal(matrix, atlas, frameIndex, color, blend);
    }

    template <>
    void iRenderer::drawTexturedQuad(const iaVector3<float32> &v1, const iaVector3<float32> &v2, const iaVector3<float32> &v3, const iaVector3<float32> &v4, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<float32> &tiling)
    {
        drawTexturedQuadInternal(v1, v2, v3, v4, texture, color, blend, tiling);
    }

    template <>
    void iRenderer::drawTexturedQuad(const iaMatrix<float32> &matrix, const iTexturePtr &texture, const iaColor4f &color, bool blend, const iaVector2<float32> &tiling)
    {
        drawTexturedQuadInternal(matrix * QUAD_VERTEX_POSITIONS[0],
                                 matrix * QUAD_VERTEX_POSITIONS[1],
                                 matrix * QUAD_VERTEX_POSITIONS[2],
                                 matrix * QUAD_VERTEX_POSITIONS[3],
                                 texture, color, blend, tiling);
    }

    template <>
    void iRenderer::drawTexturedQuad(const iaVector3<float32> &o, const iaVector3<float32> &u, const iaVector3<float32> &v, iTexturePtr texture, const iaColor4f &color, bool blend, const iaVector2<float32> &tiling)
    {
        drawTexturedQuadInternal(o + v - u,
                                 o - v - u,
                                 o - v + u,
                                 o + v + u,
                                 texture, color, blend, tiling);
    }

    template <>
    void iRenderer::drawQuad(const iaMatrix<float32> &matrix, const iaColor4f &color)
    {
        drawQuadInternal(matrix * QUAD_VERTEX_POSITIONS[0],
                         matrix * QUAD_VERTEX_POSITIONS[1],
                         matrix * QUAD_VERTEX_POSITIONS[2],
                         matrix * QUAD_VERTEX_POSITIONS[3],
                         color);
    }

    template <>
    void iRenderer::drawQuad(const iaVector3<float32> &o, const iaVector3<float32> &u, const iaVector3<float32> &v, const iaColor4f &color)
    {
        drawQuadInternal(o + v - u,
                         o - v - u,
                         o - v + u,
                         o + v + u,
                         color);
    }

    template <>
    void iRenderer::drawLineStrip(const std::vector<iaVector3<float32>> &points, const iaColor4f &color)
    {
        con_assert(points.size() > 1, "too few points");

        for (int i = 1; i < points.size(); ++i)
        {
            const auto &start = points[i - 1];
            const auto &stop = points[i];

            drawLineInternal(start, stop, color);
        }
    }

    template <>
    void iRenderer::drawLine(const iaVector3<float32> &v1, const iaVector3<float32> &v2, const iaColor4f &color)
    {
        drawLineInternal(v1, v2, color);
    }

    template <>
    void iRenderer::drawPoint(const iaVector3<float32> &v, const iaColor4f &color)
    {
        drawPointInternal(v, color);
    }

    template <>
    void iRenderer::drawBox(const iAABox<float32> &box, const iaColor4f &color)
    {
        drawBoxInternal(box, color);
    }

    void iRenderer::drawLineInternal(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color)
    {
        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Lines)
        {
            flushLastUsed();
        }

        auto &lines = _data->_lines;

        if (lines._vertexCount >= MAX_LINE_VERTICES)
        {
            flushLines();
        }

        (color._a == 1.0) ? setMaterial(_data->_flatShader) : setMaterial(_data->_flatShaderBlend);

        lines._vertexDataPtr->_pos = v1;
        lines._vertexDataPtr->_color = color;
        lines._vertexDataPtr++;

        lines._vertexDataPtr->_pos = v2;
        lines._vertexDataPtr->_color = color;
        lines._vertexDataPtr++;

        lines._vertexCount += 2;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Lines;
    }

    void iRenderer::flushTexQuads()
    {
        auto &texQuads = _data->_texQuads;

        if (texQuads._vertexCount != 0)
        {
            uint32 dataSize = (uint32)((uint8 *)texQuads._vertexDataPtr - (uint8 *)texQuads._vertexData);
            texQuads._vertexBuffer->setData(dataSize, texQuads._vertexData);
            texQuads._vertexArray->bind();

            for (int32 i = 0; i < texQuads._nextTextureIndex; ++i)
            {
                if (texQuads._textures[i] != nullptr &&
                    texQuads._textures[i]->isValid())
                {
                    texQuads._textures[i]->bind(i);
                }
                else
                {
                    _data->_fallbackTexture->bind(i);
                }

                texQuads._textures[i] = nullptr;
            }

            bindCurrentMaterial();
            _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());

            glDrawElements(GL_TRIANGLES, texQuads._indexCount, GL_UNSIGNED_INT, nullptr);
            GL_CHECK_ERROR();

            // save stats
            _data->_stats._drawCalls++;
            _data->_stats._vertices += texQuads._vertexCount;
            _data->_stats._indices += texQuads._indexCount;
            _data->_stats._triangles += texQuads._vertexCount / 2;
        }

        // reset queue
        texQuads._vertexCount = 0;
        texQuads._indexCount = 0;
        texQuads._vertexDataPtr = texQuads._vertexData;
        texQuads._nextTextureIndex = 0;
    }

    void iRenderer::flushTriangles()
    {
        auto &triangles = _data->_triangles;

        if (triangles._vertexCount == 0)
        {
            return;
        }

        bindCurrentMaterial();
        _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());

        uint32 vertexDataSize = (uint32)((uint8 *)triangles._vertexDataPtr - (uint8 *)triangles._vertexData);
        triangles._vertexBuffer->setData(vertexDataSize, triangles._vertexData);
        uint32 indexDataSize = (uint32)((uint8 *)triangles._vertexDataPtr - (uint8 *)triangles._vertexData);
        triangles._indexBuffer->setData(indexDataSize, triangles._indexData);

        triangles._vertexArray->bind();
        glDrawElements(GL_TRIANGLES, triangles._indexCount, GL_UNSIGNED_INT, nullptr);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += triangles._vertexCount;
        _data->_stats._indices += triangles._indexCount;
        _data->_stats._triangles += triangles._vertexCount / 3;

        // reset queue
        triangles._vertexCount = 0;
        triangles._indexCount = 0;
        triangles._vertexDataPtr = triangles._vertexData;
        triangles._indexDataPtr = triangles._indexData;
    }

    void iRenderer::flushQuads()
    {
        auto &quads = _data->_quads;

        if (quads._vertexCount == 0)
        {
            return;
        }

        bindCurrentMaterial();
        _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());

        uint32 dataSize = (uint32)((uint8 *)quads._vertexDataPtr - (uint8 *)quads._vertexData);
        quads._vertexBuffer->setData(dataSize, quads._vertexData);

        quads._vertexArray->bind();
        glDrawElements(GL_TRIANGLES, quads._indexCount, GL_UNSIGNED_INT, nullptr);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += quads._vertexCount;
        _data->_stats._indices += quads._indexCount;
        _data->_stats._triangles += quads._vertexCount / 2;

        // reset queue
        quads._vertexCount = 0;
        quads._indexCount = 0;
        quads._vertexDataPtr = quads._vertexData;
    }

    void iRenderer::flushLines()
    {
        auto &lines = _data->_lines;

        if (lines._vertexCount == 0)
        {
            return;
        }

        bindCurrentMaterial();
        _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());

        uint32 dataSize = (uint32)((uint8 *)lines._vertexDataPtr - (uint8 *)lines._vertexData);
        lines._vertexBuffer->setData(dataSize, lines._vertexData);

        lines._vertexArray->bind();
        glDrawArrays(GL_LINES, 0, lines._vertexCount);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += lines._vertexCount;

        // reset queue
        lines._vertexCount = 0;
        lines._vertexDataPtr = lines._vertexData;
    }

    void iRenderer::flushPoints()
    {
        auto &points = _data->_points;

        if (points._vertexCount == 0)
        {
            return;
        }

        bindCurrentMaterial();
        _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());

        uint32 dataSize = (uint32)((uint8 *)points._vertexDataPtr - (uint8 *)points._vertexData);
        points._vertexBuffer->setData(dataSize, points._vertexData);

        points._vertexArray->bind();
        glDrawArrays(GL_POINTS, 0, points._vertexCount);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += points._vertexCount;

        // reset queue
        points._vertexCount = 0;
        points._vertexDataPtr = points._vertexData;
    }

    void iRenderer::flush()
    {
        // nothing to flush
        if (_data->_lastRenderDataSetUsed == iRenderDataSet::NoDataSet)
        {
            return;
        }

        flushTexQuads();
        flushQuads();
        flushTriangles();
        flushLines();
        flushPoints();

        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;
    }

    void iRenderer::flushLastUsed()
    {
        switch (_data->_lastRenderDataSetUsed)
        {
        case iRenderDataSet::Points:
            flushPoints();
            break;

        case iRenderDataSet::Lines:
            flushLines();
            break;

        case iRenderDataSet::Quads:
            flushQuads();
            break;

        case iRenderDataSet::Triangles:
            flushTriangles();
            break;

        case iRenderDataSet::TexturedQuads:
            flushTexQuads();
            break;
        }

        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;
    }

    void iRenderer::setIgnoreRenderOrder(bool ignoreRenderOrder)
    {
        _data->_keepRenderOrder = !ignoreRenderOrder;
        flush();
    }

    bool iRenderer::isIgnoringRenderOrder()
    {
        return !_data->_keepRenderOrder;
    }

    void iRenderer::setLineWidth(float32 lineWidth)
    {
        if (_data->_lineWidth == lineWidth)
        {
            return;
        }

        flush();

        _data->_lineWidth = lineWidth;
        glLineWidth(_data->_lineWidth);
    }

    float32 iRenderer::getLineWidth() const
    {
        return _data->_lineWidth;
    }

    void iRenderer::setPointSize(float32 pointSize)
    {
        if (_data->_pointSize == pointSize)
        {
            return;
        }

        flush();

        _data->_pointSize = pointSize;
        glPointSize(_data->_pointSize);
    }

    float32 iRenderer::getPointSize() const
    {
        return _data->_pointSize;
    }

    void iRenderer::updateMatrices()
    {
        _data->_modelViewMatrix = _data->_viewMatrix * _data->_modelMatrix;
        _data->_modelViewProjectionMatrix = _data->_projectionMatrix * _data->_modelViewMatrix;
    }

    void iRenderer::setOrtho(float64 left, float64 right, float64 bottom, float64 top, float64 nearPlain, float64 farPlain)
    {
        iaMatrixd matrix;
        matrix.ortho(left, right, bottom, top, nearPlain, farPlain);
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer::setPerspective(float64 fov, float64 aspect, float64 nearPlain, float64 farPlain)
    {
        iaMatrixd matrix;
        matrix.perspective(fov, aspect, nearPlain, farPlain);
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer::setProjectionMatrix(const iaMatrixd &matrix)
    {
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer::setModelMatrix(const iaMatrixd &matrix)
    {
        if (_data->_modelMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_modelMatrix = matrix;
        updateMatrices();
    }

    void iRenderer::setViewMatrix(const iaMatrixd &matrix)
    {
        if (_data->_viewMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_viewMatrix = matrix;
        updateMatrices();
    }

    void iRenderer::setViewMatrixFromCam(const iaMatrixd &camMatrix)
    {
        _data->_camMatrix = camMatrix;

        iaMatrixd camViewMatrix;
        camViewMatrix.lookAt(camMatrix._pos, camMatrix._pos - camMatrix._depth, camMatrix._top);

        setViewMatrix(camViewMatrix);
    }

    const iaMatrixd &iRenderer::getModelMatrix() const
    {
        return _data->_modelMatrix;
    }

    const iaMatrixd &iRenderer::getCamMatrix() const
    {
        return _data->_camMatrix;
    }

    const iaMatrixd &iRenderer::getViewMatrix() const
    {
        return _data->_viewMatrix;
    }

    const iaMatrixd &iRenderer::getProjectionMatrix() const
    {
        return _data->_projectionMatrix;
    }

    const iaMatrixd &iRenderer::getModelViewMatrix() const
    {
        return _data->_modelViewMatrix;
    }

    const iaMatrixd &iRenderer::getModelViewProjectionMatrix() const
    {
        return _data->_modelViewProjectionMatrix;
    }

    const iaMatrixf iRenderer::getMVP() const
    {
        iaMatrixf matrix;
        for (int i = 0; i < 16; ++i)
        {
            matrix[i] = _data->_modelViewProjectionMatrix[i];
        }

        return matrix;
    }

    void iRenderer::drawString(float32 x, float32 y, const iaString &text, iHorizontalAlignment horz, iVerticalAlignment vert, const iaColor4f &color, float32 maxWidth)
    {
        con_assert(horz == iHorizontalAlignment::Left || horz == iHorizontalAlignment::Right || horz == iHorizontalAlignment::Center, "invalid parameters");
        con_assert(vert == iVerticalAlignment::Top || vert == iVerticalAlignment::Bottom || vert == iVerticalAlignment::Center, "invalid parameters");

        const iTextureFontPtr &font = _data->_font;
        const float32 &fontSize = _data->_fontSize;
        const float32 &fontLineHeight = _data->_fontLineHeight;

        float32 posx, posy;

        if (horz == iHorizontalAlignment::Left)
        {
            posx = x;
        }
        else if (horz == iHorizontalAlignment::Right)
        {
            if (maxWidth == 0.0f)
            {
                posx = x - font->measureWidth(text, fontSize);
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
                posx = x - font->measureWidth(text, fontSize) * 0.5f;
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
            posy = y - font->measureHeight(text, fontSize, maxWidth, fontLineHeight);
        }
        else if (vert == iVerticalAlignment::Center)
        {
            posy = y - font->measureHeight(text, fontSize, maxWidth, fontLineHeight) * 0.5f;
        }

        drawString(posx, posy, text, color, maxWidth);
    }

    void iRenderer::drawString(float32 x, float32 y, const iaString &text, const iaColor4f &color, float32 maxWidth)
    {
        if (text.isEmpty())
        {
            return;
        }

        setMaterial(_data->_textureShaderBlend);

        static wchar_t temptext[1024]; // TODO

        const float32 &fontSize = _data->_fontSize;
        const float32 &fontLineHeight = _data->_fontLineHeight;
        const iTextureFontPtr &font = _data->_font;
        const iTexturePtr &texture = font->getTexture();

        iaVector2f renderPos(x, y);
        iaVector2f renderSize(fontSize, fontSize);

        bool donotdraw = false;
        std::vector<iCharacterDimensions> characters = font->getCharacters();

        iaVector3f a, b, c, d;

        const float32 spaceWidth = font->measureWidth(" ", fontSize);

        for (uint32 i = 0; i < text.getLength(); i++)
        {
            const wchar_t &character = text[i];

            donotdraw = false;

            if (character == L'\n')
            {
                renderPos._x = x;
                renderPos._y += fontSize * fontLineHeight;
                donotdraw = true;
            }
            else if (character == L'\t')
            {
                renderPos._x += spaceWidth * 4;
                donotdraw = true;
            }
            else if (maxWidth != 0)
            {
                if (character == L' ')
                {
                    int j = 1;
                    while ((i + j < text.getLength()) && (text[i + j] != ' '))
                    {
                        temptext[j - 1] = text[i + j];
                        j++;
                    }
                    temptext[j] = 0;

                    if (renderPos._x - x + font->measureWidth(temptext, fontSize) >= maxWidth)
                    {
                        renderPos._x = x;
                        renderPos._y += fontSize * fontLineHeight;
                        donotdraw = true;
                    }
                }
            }

            if (donotdraw)
            {
                continue;
            }

            const uint32 fontIndex = character - 32;
            const float32 &texX = characters[fontIndex]._characterRect.getX();
            const float32 &texY = characters[fontIndex]._characterRect.getY();

            const float32 &width = characters[fontIndex]._characterRect.getWidth();
            const float32 &height = characters[fontIndex]._characterRect.getHeight();

            renderSize._x = fontSize * characters[fontIndex]._characterOffset;

            const int32 textureIndex = beginTexturedQuad(texture);

            auto &texQuads = _data->_texQuads;
            texQuads._vertexDataPtr->_pos.set(renderPos._x, renderPos._y + renderSize._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord0.set(texX, texY + height);
            texQuads._vertexDataPtr->_texIndex0 = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x + renderSize._x, renderPos._y + renderSize._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord0.set(texX + width, texY + height);
            texQuads._vertexDataPtr->_texIndex0 = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x + renderSize._x, renderPos._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord0.set(texX + width, texY);
            texQuads._vertexDataPtr->_texIndex0 = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x, renderPos._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord0.set(texX, texY);
            texQuads._vertexDataPtr->_texIndex0 = textureIndex;
            texQuads._vertexDataPtr++;

            endTexturedQuad();

            renderPos._x += renderSize._x;
        }
    }

    void iRenderer::setFont(const iTextureFontPtr &font)
    {
        con_assert(font->isValid(), "invalid font used");

        _data->_font = font;
    }

    const iTextureFontPtr &iRenderer::getFont() const
    {
        return _data->_font;
    }

    void iRenderer::setFontSize(float32 fontSize)
    {
        _data->_fontSize = fontSize;
    }

    float32 iRenderer::getFontSize() const
    {
        return _data->_fontSize;
    }

    void iRenderer::setFontLineHeight(float32 lineheight)
    {
        _data->_fontLineHeight = lineheight;
    }

    float32 iRenderer::getFontLineHeight() const
    {
        return _data->_fontLineHeight;
    }

    const iRenderer::iRendererStats &iRenderer::getStats() const
    {
        return _data->_stats;
    }

    void iRenderer::clearStats()
    {
        // con_endl("_data->_stats._drawCalls " << _data->_stats._drawCalls);

        _data->_stats._drawCalls = 0;
        _data->_stats._vertices = 0;
        _data->_stats._indices = 0;
        _data->_stats._triangles = 0;
    }

    const iaRectanglei &iRenderer::getViewport() const
    {
        return _data->_viewport;
    }

    void iRenderer::setViewport(const iaRectanglei &viewport)
    {
        if (_data->_viewport == viewport)
        {
            return;
        }

        flush();

        _data->_viewport = viewport;
        glViewport(viewport._x, viewport._y, viewport._width, viewport._height);
    }

    void iRenderer::setViewport(int32 x, int32 y, int32 width, int32 height)
    {
        setViewport(iaRectanglei(x, y, width, height));
    }

    void iRenderer::clearColorBuffer(const iaColor4f &color)
    {
        glClearColor(color._r, color._g, color._b, color._a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void iRenderer::clearDepthBuffer(float32 depth)
    {
        glDepthMask(true);
        glClearDepth(depth);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void iRenderer::clearStencilBuffer(int32 index)
    {
        glClearStencil(index);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    const iaVector3d iRenderer::project(const iaVector3d &objectSpacePos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const
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

    const iaVector3d iRenderer::unProject(const iaVector3d &screenpos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const
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

    void iRenderer::setStencilFunction(iStencilFunction function, int32 ref, uint32 mask)
    {
        glStencilFunc(iRendererUtils::convertType(function), ref, mask);
    }

    void iRenderer::setStencilOperation(iStencilOperation fail, iStencilOperation zfail, iStencilOperation zpass)
    {
        glStencilOp(iRendererUtils::convertType(fail), iRendererUtils::convertType(zfail), iRendererUtils::convertType(zpass));
    }

    void iRenderer::setStencilTestActive(bool enable)
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

    void iRenderer::setMaterial(const iMaterialPtr &material)
    {
        con_assert(material != nullptr, "invalid material");

        if (_data->_currentMaterial == material)
        {
            return;
        }

        flush();
        _data->_currentMaterial = material;
    }

    const iMaterialPtr &iRenderer::getMaterial() const
    {
        return _data->_currentMaterial;
    }

    void iRenderer::drawBoxInternal(const iAABoxf &box, const iaColor4f &color)
    {
        iaVector3f a = box._center;
        a -= box._halfWidths;
        iaVector3f b = box._center;
        b += box._halfWidths;

        drawLineInternal(iaVector3f(a._x, a._y, a._z),
                         iaVector3f(b._x, a._y, a._z), color);

        drawLineInternal(iaVector3f(a._x, a._y, a._z),
                         iaVector3f(a._x, b._y, a._z), color);

        drawLineInternal(iaVector3f(a._x, a._y, a._z),
                         iaVector3f(a._x, a._y, b._z), color);

        drawLineInternal(iaVector3f(b._x, a._y, a._z),
                         iaVector3f(b._x, a._y, b._z), color);

        drawLineInternal(iaVector3f(b._x, a._y, a._z),
                         iaVector3f(b._x, b._y, a._z), color);

        drawLineInternal(iaVector3f(b._x, a._y, b._z),
                         iaVector3f(b._x, b._y, b._z), color);

        drawLineInternal(iaVector3f(a._x, a._y, b._z),
                         iaVector3f(b._x, a._y, b._z), color);

        drawLineInternal(iaVector3f(a._x, a._y, b._z),
                         iaVector3f(a._x, b._y, b._z), color);

        drawLineInternal(iaVector3f(a._x, b._y, a._z),
                         iaVector3f(a._x, b._y, b._z), color);

        drawLineInternal(iaVector3f(a._x, b._y, b._z),
                         iaVector3f(b._x, b._y, b._z), color);

        drawLineInternal(iaVector3f(a._x, b._y, a._z),
                         iaVector3f(b._x, b._y, a._z), color);

        drawLineInternal(iaVector3f(b._x, b._y, a._z),
                         iaVector3f(b._x, b._y, b._z), color);
    }

    void iRenderer::drawFilledCircleInternal(float32 x, float32 y, float32 radius, int segments, const iaColor4f &color)
    {
        beginTriangles();

        (color._a == 1.0) ? setMaterial(_data->_flatShader) : setMaterial(_data->_flatShaderBlend);

        auto &triangles = _data->_triangles;

        const float32 step = 2 * M_PI / static_cast<float32>(segments);
        float32 angleA = 0;
        float32 angleB = step;

        uint32 centerIndex = triangles._vertexCount;
        uint32 nextIndex = centerIndex + 1;

        triangles._vertexDataPtr->_pos.set(x, y, 0.0f);
        triangles._vertexDataPtr->_color = color;
        triangles._vertexDataPtr++;

        for (int i = 0; i < segments; ++i)
        {
            const iaVector3f b(x + radius * cosf(angleA), y + radius * sinf(angleA), 0.0f);
            const iaVector3f c(x + radius * cosf(angleB), y + radius * sinf(angleB), 0.0f);
            angleA += step;
            angleB += step;

            *triangles._indexDataPtr = centerIndex;
            triangles._indexDataPtr++;
            triangles._indexCount++;

            triangles._vertexDataPtr->_pos = c;
            triangles._vertexDataPtr->_color = color;
            triangles._vertexDataPtr++;

            *triangles._indexDataPtr = nextIndex++;
            triangles._indexDataPtr++;
            triangles._indexCount++;

            triangles._vertexDataPtr->_pos = b;
            triangles._vertexDataPtr->_color = color;
            triangles._vertexDataPtr++;

            *triangles._indexDataPtr = nextIndex++;
            triangles._indexDataPtr++;
            triangles._indexCount++;
        }

        triangles._vertexCount += segments * 2 + 1;

        endTriangles();
    }

    __IGOR_INLINE__ void iRenderer::beginTriangles()
    {
        auto &triangles = _data->_triangles;

        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Triangles)
        {
            flushLastUsed();
        }

        if (triangles._vertexCount >= MAX_TRIANGLE_VERTICES)
        {
            flushTriangles();
        }
    }

    __IGOR_INLINE__ void iRenderer::endTriangles()
    {
        _data->_lastRenderDataSetUsed = iRenderDataSet::Triangles;
    }

    void iRenderer::setFallbackTexture(const iTexturePtr &texture)
    {
        _data->_fallbackTexture = texture;
    }

    // TODO only bind if it changed
    void iRenderer::bindCurrentMaterial()
    {
        con_assert_sticky(_data->_currentMaterial != nullptr, "no material set");

        _data->_currentMaterial->bind();

        if (_data->_wireframeEnabled)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }

    void iRenderer::drawMesh(iMeshPtr mesh, iTargetMaterialPtr targetMaterial)
    {
        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Buffer)
        {
            flushLastUsed();
        }

        bindCurrentMaterial();
        writeShaderParameters(targetMaterial);

        mesh->bind();

        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, nullptr);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += mesh->getVertexCount();
        _data->_stats._indices += mesh->getIndexCount();
        _data->_stats._triangles += mesh->getTrianglesCount();

        _data->_lastRenderDataSetUsed = iRenderDataSet::Buffer;
    }

    void iRenderer::drawBuffer(iVertexArrayPtr vertexArray, iRenderPrimitive primitiveType, iTargetMaterialPtr targetMaterial)
    {
        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Buffer)
        {
            flushLastUsed();
        }

        bindCurrentMaterial();
        writeShaderParameters(targetMaterial);

        vertexArray->bind();

        const uint32 indexCount = vertexArray->getIndexCount();

        if (indexCount != 0)
        {
            glDrawElements(iRendererUtils::convertType(primitiveType), indexCount, GL_UNSIGNED_INT, nullptr);
            GL_CHECK_ERROR();
        }
        else
        {
            glDrawArrays(iRendererUtils::convertType(primitiveType), 0, vertexArray->getVertexCount());
            GL_CHECK_ERROR();
        }

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += vertexArray->getVertexCount();
        _data->_stats._indices += indexCount;
        _data->_stats._triangles += vertexArray->getVertexCount() / 3; // TODO this is just an estimate

        _data->_lastRenderDataSetUsed = iRenderDataSet::Buffer;
    }

    void iRenderer::drawMeshInstanced(iMeshPtr mesh, iInstancingBufferPtr instancingBuffer, iTargetMaterialPtr targetMaterial)
    {
        if (!mesh->isValid())
        {
            return;
        }

        if (_data->_keepRenderOrder && _data->_lastRenderDataSetUsed != iRenderDataSet::Buffer)
        {
            flushLastUsed();
        }

        instancingBuffer->finalizeData();

        iaMatrixd idMatrix;
        setModelMatrix(idMatrix);

        bindCurrentMaterial();
        writeShaderParameters(targetMaterial);

        // re combine mesh data wich instancing data
        iVertexArrayPtr vertexArray = iVertexArray::create();

        for (iVertexBufferPtr vertexBuffer : mesh->getVertexArray()->getVertexBuffers())
        {
            vertexArray->addVertexBuffer(vertexBuffer);
        }
        vertexArray->setIndexBuffer(mesh->getVertexArray()->getIndexBuffer());
        vertexArray->addVertexBuffer(instancingBuffer->getVertexBuffer());
        vertexArray->bind();

        const uint32 instanceCount = instancingBuffer->getInstanceCount();

        glDrawElementsInstanced(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
        GL_CHECK_ERROR();

        // save stats
        _data->_stats._drawCalls++;
        _data->_stats._vertices += mesh->getVertexCount() * instanceCount;
        _data->_stats._indices += mesh->getIndexCount() * instanceCount;
        _data->_stats._triangles += mesh->getTrianglesCount() * instanceCount;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Buffer;
    }

    void iRenderer::writeShaderParameters(iTargetMaterialPtr targetMaterial)
    {
        if (targetMaterial != nullptr)
        {
            if (_data->_currentMaterial->hasTargetMaterial())
            {
                _data->_currentMaterial->setFloat3(UNIFORM_MATERIAL_AMBIENT, targetMaterial->getAmbient());
                _data->_currentMaterial->setFloat3(UNIFORM_MATERIAL_DIFFUSE, targetMaterial->getDiffuse());
                _data->_currentMaterial->setFloat3(UNIFORM_MATERIAL_SPECULAR, targetMaterial->getSpecular());
                _data->_currentMaterial->setFloat3(UNIFORM_MATERIAL_EMISSIVE, targetMaterial->getEmissive());
                _data->_currentMaterial->setFloat(UNIFORM_MATERIAL_SHININESS, targetMaterial->getShininess());
                _data->_currentMaterial->setFloat(UNIFORM_MATERIAL_ALPHA, targetMaterial->getAlpha());
            }

            if (_data->_currentMaterial->hasTilingConfig())
            {
                _data->_currentMaterial->setFloat2(UNIFORM_CONFIG_TILING, targetMaterial->getTilingConfig());
            }

            if (_data->_currentMaterial->hasVelocityOrientedConfig())
            {
                _data->_currentMaterial->setFloat(UNIFORM_CONFIG_VELOCITY_ORIENTED, targetMaterial->isVelocityOriented() ? 1.0 : 0.0);
            }

            for (const auto &pair : targetMaterial->getTextures())
            {
                const auto &texture = pair.second;
                const auto &texUnit = pair.first;

                if (_data->_currentMaterial->hasTextureUnit(texUnit))
                {
                    std::stringstream shaderProperty;
                    shaderProperty << SAMPLER_TEXTURE << texUnit;
                    _data->_currentMaterial->setInt(shaderProperty.str().c_str(), texUnit);

                    if (texture == nullptr ||
                        texture->useFallback())
                    {
                        _data->_fallbackTexture->bind(texUnit);
                    }
                    else
                    {
                        texture->bind(texUnit);
                    }
                }
            }
        }

        if (_data->_currentMaterial->hasDirectionalLight())
        {
            _data->_currentMaterial->setFloat3(UNIFORM_LIGHT_ORIENTATION, _data->_lights[0]._position);
            _data->_currentMaterial->setFloat3(UNIFORM_LIGHT_AMBIENT, _data->_lights[0]._ambient);
            _data->_currentMaterial->setFloat3(UNIFORM_LIGHT_DIFFUSE, _data->_lights[0]._diffuse);
            _data->_currentMaterial->setFloat3(UNIFORM_LIGHT_SPECULAR, _data->_lights[0]._specular);
        }

        if (_data->_currentMaterial->hasEyePosition())
        {
            iaVector3f eyePosition(_data->_camMatrix._pos._x, _data->_camMatrix._pos._y, _data->_camMatrix._pos._z);
            _data->_currentMaterial->setFloat3(UNIFORM_EYE_POSITION, eyePosition);
        }

        if (_data->_currentMaterial->hasModelViewProjectionMatrix())
        {
            _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW_PROJECTION, getMVP());
        }

        if (_data->_currentMaterial->hasModelViewMatrix())
        {
            iaMatrixf modelView;
            for (int i = 0; i < 16; ++i)
            {
                modelView[i] = _data->_modelViewMatrix[i];
            }
            _data->_currentMaterial->setMatrix(UNIFORM_MODEL_VIEW, modelView);
        }

        if (_data->_currentMaterial->hasViewProjectionMatrix())
        {
            iaMatrixd vpd = _data->_projectionMatrix * _data->_viewMatrix;
            iaMatrixf viewProjection;

            for (int i = 0; i < 16; ++i)
            {
                viewProjection[i] = vpd[i];
            }
            _data->_currentMaterial->setMatrix(UNIFORM_VIEW_PROJECTION, viewProjection);
        }

        if (_data->_currentMaterial->hasModelMatrix())
        {
            iaMatrixf model;
            for (int i = 0; i < 16; ++i)
            {
                model[i] = _data->_modelMatrix[i];
            }
            _data->_currentMaterial->setMatrix(UNIFORM_MODEL, model);
        }

        if (_data->_currentMaterial->hasSolidColor())
        {
            _data->_currentMaterial->setFloat4(UNIFORM_SOLIDCOLOR, _data->_solidColor);
        }
    }

    void iRenderer::setLightPosition(int32 lightnum, const iaVector3d &pos)
    {
        _data->_lights[lightnum]._position.set(pos._x, pos._y, pos._z);
    }

    void iRenderer::setLightAmbient(int32 lightnum, iaColor3f &ambient)
    {
        _data->_lights[lightnum]._ambient = ambient;
    }

    void iRenderer::setLightDiffuse(int32 lightnum, iaColor3f &diffuse)
    {
        _data->_lights[lightnum]._diffuse = diffuse;
    }

    void iRenderer::setLightSpecular(int32 lightnum, iaColor3f &specular)
    {
        _data->_lights[lightnum]._specular = specular;
    }

    void iRenderer::setColorID(uint64 colorID)
    {
        _data->_solidColor.set(static_cast<float32>(static_cast<uint8>(colorID >> 16)) / 255.0,
                               static_cast<float32>(static_cast<uint8>(colorID >> 8)) / 255.0,
                               static_cast<float32>(static_cast<uint8>(colorID)) / 255.0,
                               1.0f);
    }

    void iRenderer::setColor(const iaColor4f &color)
    {
        _data->_solidColor = color;
    }

    iRenderTargetID iRenderer::createRenderTarget(uint32 width, uint32 height, iColorFormat format, iRenderTargetType renderTargetType, bool useDepthBuffer)
    {
        iRenderTargetID result = DEFAULT_RENDER_TARGET;
        GLenum glformat = iRendererUtils::convertType(format);

        GLuint fbo;
        GLuint colorRenderBuffer;
        GLuint depthRenderBuffer;
        glGenFramebuffers(1, &fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenRenderbuffers(1, &colorRenderBuffer);

        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);

        if (useDepthBuffer)
        {
            glGenRenderbuffers(1, &depthRenderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            con_err("unsupported frame buffer object configureation");

            // clean up again
            glDeleteRenderbuffers(1, &colorRenderBuffer);

            if (useDepthBuffer)
            {
                glDeleteRenderbuffers(1, &depthRenderBuffer);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, &fbo);

            // restore current render target
            glBindFramebuffer(GL_FRAMEBUFFER, _data->_currentRenderTarget);

            return DEFAULT_RENDER_TARGET;
        }

        // restore current render target
        glBindFramebuffer(GL_FRAMEBUFFER, _data->_currentRenderTarget);

        iRendererTarget renderTarget;
        renderTarget._renderTargetType = renderTargetType;
        renderTarget._frameBufferObject = fbo;
        renderTarget._colorBuffer = colorRenderBuffer;
        renderTarget._hasDepth = useDepthBuffer;
        renderTarget._depthBuffer = depthRenderBuffer;

        // id is handled by ogl so we don't have to check it
        _data->_renderTargets[fbo] = renderTarget;

        result = fbo;

        return result;
    }

    void iRenderer::destroyRenderTarget(iRenderTargetID id)
    {
        auto iter = _data->_renderTargets.find(id);
        if (iter != _data->_renderTargets.end())
        {
            iRendererTarget renderTarget = (*iter).second;

            glDeleteRenderbuffers(1, (GLuint *)&renderTarget._colorBuffer);

            if (renderTarget._hasDepth)
            {
                glDeleteRenderbuffers(1, (GLuint *)&renderTarget._depthBuffer);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDeleteFramebuffers(1, (GLuint *)&renderTarget._frameBufferObject);

            // restore current render target
            glBindFramebuffer(GL_FRAMEBUFFER, _data->_currentRenderTarget);

            _data->_renderTargets.erase(iter);
        }
        else
        {
            con_err("invalid render target id " << id);
        }
    }

    void iRenderer::setRenderTarget(iRenderTargetID id)
    {
        // the ID is also the frame buffer object ID
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        _data->_currentRenderTarget = id;
    }

    iRenderTargetID iRenderer::getRenderTarget() const
    {
        return _data->_currentRenderTarget;
    }

    void iRenderer::readPixels(int32 x, int32 y, int32 width, int32 height, iColorFormat format, uint8 *data)
    {
        GLenum glformat = iRendererUtils::convertType(format);

        if (_data->_currentRenderTarget == DEFAULT_RENDER_TARGET)
        {
            glReadBuffer(GL_FRONT);
        }
        else
        {
            glReadBuffer(GL_COLOR_ATTACHMENT0);
        }
        glReadPixels(x, y, width, height, glformat, GL_UNSIGNED_BYTE, data);
    }

    /* TODO

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
    */

    void iRenderer::setWireframeEnabled(bool active)
    {
        _data->_wireframeEnabled = active;
    }

    bool iRenderer::isWireframeEnabled() const
    {
        return _data->_wireframeEnabled;
    }

}
