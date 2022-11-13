// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderer2.h>

#include <igor/renderer/iRenderer.h>

#include <igor/renderer/shader/iShaderProgram.h>

namespace igor
{
    static const uint32 MAX_POINT_VERTICES = 10000;

    static const uint32 MAX_LINES = 10000;
    static const uint32 MAX_LINE_VERTICES = MAX_LINES * 2;

    static const uint32 MAX_QUADS = 10000;
    static const uint32 MAX_QUAD_VERTICES = MAX_QUADS * 4;
    static const uint32 MAX_QUAD_INDICES = MAX_QUADS * 6;

    static const iaVector2f QUAD_TEXTURE_COORDS[] = {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
    static const iaVector3f QUAD_VERTEX_POSITIONS[] = {{-0.5f, -0.5, 0.0f}, {-0.5f, 0.5, 0.0f}, {0.5f, 0.5, 0.0f}, {0.5f, -0.5, 0.0f}};

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
        iaVector2f _texCoord;
        float32 _texIndex;
    };

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

    /*! render data set names
     */
    enum class iRenderDataSet
    {
        NoDataSet,
        Points,
        Lines,
        Quads,
        TexturedQuads
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

        /*! quads render data
         */
        iRendererDataQuads _quads;

        /*! textured quads render data
         */
        iRendererDataTexturedQuads _texQuads;

        ////////// SHADERS ////////////
        /*! shader for single color flat shading
         */
        iShaderProgramPtr _flatShader;

        /*! shader for textured shading
         */
        iShaderProgramPtr _textureShader;

        /*! quad index data
         */
        uint32 *_sharedQuadIndexData = nullptr;

        /*! quad index buffer
         */
        iIndexBufferPtr _sharedQuadIndexBuffer;

        /*! stores which render dataset was used last
         */
        iRenderDataSet _lastRenderDataSetUsed;

        /////////// MATRICES /////////////
        iaMatrixd _modelMatrix;
        iaMatrixd _viewMatrix;
        iaMatrixd _projectionMatrix;
        iaMatrixd _modelViewMatrix;
        iaMatrixd _modelViewProjectionMatrix;

        iaMatrixd _camMatrix;

        /////////// SETTINGS //////
        /*! if true render order will be kept by the cost of more draw calls beeing used
         */
        bool _keepRenderOrder = true;

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

        iaRectanglei _viewport;

        iaColor4f _clearColor;

        float32 _clearDepth;

        bool _blendingActive = false;

        bool _depthTestActive = false;

        bool _depthMaskActive = true;

        bool _cullFaceActive = true;

        ////// stats ////
        iRenderer2::iRendererStats _stats;
    };

    static void applyOGLStates(bool blending, bool depthTest, bool depthMask, bool cullFace)
    {
        blending ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        depthMask ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
        cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }

    iRenderer2::iRenderer2()
    {
        _data = std::make_unique<iRendererData>();
    }

    iRenderer2::~iRenderer2()
    {
        deinit();
    }

    void iRenderer2::init()
    {
        con_info("init renderer");

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

        //////// QUADS comon data ///////////
        _data->_sharedQuadIndexData = new uint32[MAX_QUAD_INDICES];

        for (uint32 i = 0; i < MAX_QUADS; ++i)
        {
            _data->_sharedQuadIndexData[i * 6 + 0] = i * 4 + 0;
            _data->_sharedQuadIndexData[i * 6 + 1] = i * 4 + 1;
            _data->_sharedQuadIndexData[i * 6 + 2] = i * 4 + 3;

            _data->_sharedQuadIndexData[i * 6 + 3] = i * 4 + 1;
            _data->_sharedQuadIndexData[i * 6 + 4] = i * 4 + 2;
            _data->_sharedQuadIndexData[i * 6 + 5] = i * 4 + 3;
        }

        _data->_sharedQuadIndexBuffer = iIndexBuffer::create(MAX_QUAD_INDICES, _data->_sharedQuadIndexData);

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

        ///////////// SHADER ////////////
        _data->_flatShader = iShaderProgram::create();
        _data->_flatShader->addShader("igor/renderer/flat_shader.vert", iShaderObjectType_New::Vertex);
        _data->_flatShader->addShader("igor/renderer/flat_shader.frag", iShaderObjectType_New::Fragment);
        _data->_flatShader->compile();

        _data->_textureShader = iShaderProgram::create();
        _data->_textureShader->addShader("igor/renderer/textured_shader.vert", iShaderObjectType_New::Vertex);
        _data->_textureShader->addShader("igor/renderer/textured_shader.frag", iShaderObjectType_New::Fragment);
        _data->_textureShader->compile();

        /////////// STATES ////////////
        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;

        /////////// OGL //////////
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
    }

    void iRenderer2::deinit()
    {
        con_info("deinit renderer");

        auto &lines = _data->_lines;
        lines._vertexBuffer = nullptr;
        lines._vertexArray = nullptr;
        delete[] lines._vertexData;
        lines._vertexDataPtr = lines._vertexData = nullptr;

        auto &points = _data->_points;
        points._vertexBuffer = nullptr;
        points._vertexArray = nullptr;
        delete[] points._vertexData;
        points._vertexDataPtr = points._vertexData = nullptr;

        auto &quads = _data->_quads;
        quads._vertexBuffer = nullptr;
        quads._vertexArray = nullptr;
        delete[] quads._vertexData;
        quads._vertexDataPtr = quads._vertexData = nullptr;

        auto &texQuads = _data->_texQuads;
        texQuads._vertexBuffer = nullptr;
        texQuads._vertexArray = nullptr;
        delete[] texQuads._vertexData;
        texQuads._vertexDataPtr = texQuads._vertexData = nullptr;
        for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
        {
            texQuads._textures[i] = nullptr;
        }

        _data->_sharedQuadIndexBuffer = nullptr;
        delete[] _data->_sharedQuadIndexData;
        _data->_sharedQuadIndexData = nullptr;
    }

    void iRenderer2::drawTexturedRectangle(float32 x, float32 y, float32 width, float32 height, const iTexturePtr &texture, const iaColor4f &color, const iaVector2f &tiling)
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
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == -1)
        {
            if (texQuads._nextTextureIndex > MAX_TEXTURE_UNITS)
            {
                flushTexQuads();
            }

            textureIndex = texQuads._nextTextureIndex;
            texQuads._textures[texQuads._nextTextureIndex] = texture;
            texQuads._nextTextureIndex++;
        }

        texQuads._vertexDataPtr->_pos.set(x, y, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = QUAD_TEXTURE_COORDS[0];
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos.set(x, y + height, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[1]._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[1]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos.set(x + width, y + height, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[2]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[2]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos.set(x + width, y, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[3]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[3]._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexCount += 4;
        texQuads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::TexturedQuads;
    }

    void iRenderer2::drawTexturedRectangle(const iaRectanglef &rect, const iTexturePtr &texture, const iaColor4f &color, const iaVector2f &tiling)
    {
        drawTexturedRectangle(rect._x, rect._y, rect._width, rect._height, texture, color, tiling);
    }

    void iRenderer2::drawTexturedQuad(const iaMatrixf &matrix, const iTexturePtr &texture, const iaColor4f &color, const iaVector2f &tiling)
    {
        drawTexturedQuad(matrix * QUAD_VERTEX_POSITIONS[0],
                         matrix * QUAD_VERTEX_POSITIONS[1],
                         matrix * QUAD_VERTEX_POSITIONS[2],
                         matrix * QUAD_VERTEX_POSITIONS[3],
                         texture, color, tiling);
    }

    __IGOR_INLINE__ int32 iRenderer2::beginTexturedQuad(const iTexturePtr &texture)
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
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == -1)
        {
            if (texQuads._nextTextureIndex > MAX_TEXTURE_UNITS)
            {
                flushTexQuads();
            }

            textureIndex = texQuads._nextTextureIndex;
            texQuads._textures[texQuads._nextTextureIndex] = texture;
            texQuads._nextTextureIndex++;
        }

        return textureIndex;
    }

    __IGOR_INLINE__ void iRenderer2::endTexturedQuad()
    {
        auto &texQuads = _data->_texQuads;
        texQuads._vertexCount += 4;
        texQuads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::TexturedQuads;
    }

    void iRenderer2::drawTexturedQuad(const iaVector3f &v1, const iaVector3f &v2, const iaVector3f &v3, const iaVector3f &v4, const iTexturePtr &texture, const iaColor4f &color, const iaVector2f &tiling)
    {
        const int32 textureIndex = beginTexturedQuad(texture);

        auto &texQuads = _data->_texQuads;
        texQuads._vertexDataPtr->_pos = v1;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = QUAD_TEXTURE_COORDS[0];
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v2;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[1]._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[1]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v3;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[2]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[2]._y * tiling._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = v4;
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord._x = QUAD_TEXTURE_COORDS[3]._x * tiling._x;
        texQuads._vertexDataPtr->_texCoord._y = QUAD_TEXTURE_COORDS[3]._y;
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        endTexturedQuad();
    }

    void iRenderer2::drawFrame(const iaMatrixf &matrix, const iAtlasPtr &atlas, uint32 frameIndex, const iaColor4f &color)
    {
        const int32 textureIndex = beginTexturedQuad(atlas->getTexture());

        const iAtlas::iFrame &frame = atlas->getFrame(frameIndex);

        // TODO origin
        const iaVector3f offset; // (frame._origin._x, frame._origin._y, 0.0);

        auto &texQuads = _data->_texQuads;
        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[0];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = frame._rect.getTopLeft();
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[1];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = frame._rect.getBottomLeft();
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[2];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = frame._rect.getBottomRight();
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[3];
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = frame._rect.getTopRight();
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        endTexturedQuad();
    }

    /*void iRenderer2::drawSprite(const iaMatrixf &matrix, const iAtlasPtr sprite, uint32 frameIndex, const iaVector2f &tiling, const iaColor4f &color)
    {
        iTexturePtr texture = sprite->getTexture();
        const iAtlas::iFrame &frame = sprite->getFrame(frameIndex);

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
    }*/

    void iRenderer2::drawPoint(float32 x, float32 y, const iaColor4f &color)
    {
        drawPoint(iaVector3f(x, y, 0.0), color);
    }
    void iRenderer2::drawPoint(const iaVector2f &v, const iaColor4f &color)
    {
        drawPoint(iaVector3f(v._x, v._y, 0.0), color);
    }
    void iRenderer2::drawPoint(const iaVector3f &v, const iaColor4f &color)
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

        points._vertexDataPtr->_pos = v;
        points._vertexDataPtr->_color = color;
        points._vertexDataPtr++;

        points._vertexCount++;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Points;
    }

    void iRenderer2::drawRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color)
    {
        iaVector3f v0(x, y, 0.0);
        iaVector3f v1(x, y + height, 0.0);
        iaVector3f v2(x + width, y + height, 0.0);
        iaVector3f v3(x + width, y, 0.0);

        drawLine(v0, v1, color);
        drawLine(v1, v2, color);
        drawLine(v2, v3, color);
        drawLine(v3, v0, color);
    }

    void iRenderer2::drawRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        drawRectangle(rect._x, rect._y, rect._width, rect._height, color);
    }

    void iRenderer2::drawFilledRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color)
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

        quads._vertexDataPtr->_pos.set(x, y, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(x, y + height, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(x + width, y + height, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(x + width, y, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexCount += 4;
        quads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Quads;
    }

    void iRenderer2::drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        drawFilledRectangle(rect._x, rect._y, rect._width, rect._height, color);
    }

    void iRenderer2::drawQuad(const iaMatrixf &matrix, const iaColor4f &color)
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

        quads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[0];
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[1];
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[2];
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos = matrix * QUAD_VERTEX_POSITIONS[3];
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexCount += 4;
        quads._indexCount += 6;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Quads;
    }

    void iRenderer2::drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color)
    {
        drawLine(iaVector3f(x1, y1, 0.0), iaVector3f(x2, y2, 0.0), color);
    }

    void iRenderer2::drawLine(const iaVector2f &v1, const iaVector2f &v2, const iaColor4f &color)
    {
        drawLine(iaVector3f(v1._x, v1._y, 0.0), iaVector3f(v2._x, v2._y, 0.0), color);
    }

    void iRenderer2::drawLineStrip(const std::vector<iaVector3f> &points, const iaColor4f &color)
    {
        if (points.size() <= 1)
        {
            return;
        }

        for (int i = 1; i < points.size(); ++i)
        {
            drawLine(points[i - 1], points[i], color);
        }
    }

    void iRenderer2::drawLine(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color)
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

        lines._vertexDataPtr->_pos = v1;
        lines._vertexDataPtr->_color = color;
        lines._vertexDataPtr++;

        lines._vertexDataPtr->_pos = v2;
        lines._vertexDataPtr->_color = color;
        lines._vertexDataPtr++;

        lines._vertexCount += 2;

        _data->_lastRenderDataSetUsed = iRenderDataSet::Lines;
    }

    void iRenderer2::flushTexQuads()
    {
        auto &texQuads = _data->_texQuads;

        if (texQuads._vertexCount == 0)
        {
            return;
        }

        applyOGLStates(_data->_blendingActive, _data->_depthTestActive, _data->_depthMaskActive, _data->_cullFaceActive);

        uint32 dataSize = (uint32)((uint8 *)texQuads._vertexDataPtr - (uint8 *)texQuads._vertexData);
        texQuads._vertexBuffer->setData(dataSize, texQuads._vertexData);
        texQuads._vertexArray->bind();

        for (int32 i = 0; i < texQuads._nextTextureIndex; ++i)
        {
            iRenderer::getInstance().bindTexture(texQuads._textures[i], i);
            // TODO texQuads._textures[i]->bind(i); we need the dummy texture in the new renderer
        }

        _data->_textureShader->bind();
        _data->_textureShader->setMatrix("igor_modelViewProjection", getMVP());

        glDrawElements(GL_TRIANGLES, texQuads._indexCount, GL_UNSIGNED_INT, nullptr);
        GL_CHECK_ERROR();
        texQuads._vertexArray->unbind();

        _data->_textureShader->unbind();

        _data->_stats._drawCalls++;
        _data->_stats._vertices += texQuads._vertexCount;
        _data->_stats._indices += texQuads._indexCount;
        _data->_stats._triangles += texQuads._vertexCount / 2;

        texQuads._vertexCount = 0;
        texQuads._indexCount = 0;
        texQuads._vertexDataPtr = texQuads._vertexData;

        texQuads._nextTextureIndex = 0;
    }

    void iRenderer2::flushQuads()
    {
        auto &quads = _data->_quads;

        if (quads._vertexCount == 0)
        {
            return;
        }

        applyOGLStates(_data->_blendingActive, _data->_depthTestActive, _data->_depthMaskActive, _data->_cullFaceActive);

        _data->_flatShader->bind();
        _data->_textureShader->setMatrix("igor_modelViewProjection", getMVP());

        uint32 dataSize = (uint32)((uint8 *)quads._vertexDataPtr - (uint8 *)quads._vertexData);
        quads._vertexBuffer->setData(dataSize, quads._vertexData);

        quads._vertexArray->bind();
        glDrawElements(GL_TRIANGLES, quads._indexCount, GL_UNSIGNED_INT, nullptr);
        GL_CHECK_ERROR();
        quads._vertexArray->unbind();

        _data->_flatShader->unbind();

        _data->_stats._drawCalls++;
        _data->_stats._vertices += quads._vertexCount;
        _data->_stats._indices += quads._indexCount;
        _data->_stats._triangles += quads._vertexCount / 2;

        quads._vertexCount = 0;
        quads._indexCount = 0;
        quads._vertexDataPtr = quads._vertexData;
    }

    void iRenderer2::flushLines()
    {
        auto &lines = _data->_lines;

        if (lines._vertexCount == 0)
        {
            return;
        }

        applyOGLStates(_data->_blendingActive, _data->_depthTestActive, _data->_depthMaskActive, _data->_cullFaceActive);

        _data->_flatShader->bind();
        _data->_textureShader->setMatrix("igor_modelViewProjection", getMVP());

        uint32 dataSize = (uint32)((uint8 *)lines._vertexDataPtr - (uint8 *)lines._vertexData);
        lines._vertexBuffer->setData(dataSize, lines._vertexData);

        lines._vertexArray->bind();
        glDrawArrays(GL_LINES, 0, lines._vertexCount);
        GL_CHECK_ERROR();
        lines._vertexArray->unbind();

        _data->_flatShader->unbind();

        _data->_stats._drawCalls++;
        _data->_stats._vertices += lines._vertexCount;

        lines._vertexCount = 0;
        lines._vertexDataPtr = lines._vertexData;
    }

    void iRenderer2::flushPoints()
    {
        auto &points = _data->_points;

        if (points._vertexCount == 0)
        {
            return;
        }

        applyOGLStates(_data->_blendingActive, _data->_depthTestActive, _data->_depthMaskActive, _data->_cullFaceActive);

        _data->_flatShader->bind();
        _data->_textureShader->setMatrix("igor_modelViewProjection", getMVP());

        uint32 dataSize = (uint32)((uint8 *)points._vertexDataPtr - (uint8 *)points._vertexData);
        points._vertexBuffer->setData(dataSize, points._vertexData);

        points._vertexArray->bind();
        glDrawArrays(GL_POINTS, 0, points._vertexCount);
        GL_CHECK_ERROR();
        points._vertexArray->unbind();

        _data->_flatShader->unbind();

        _data->_stats._drawCalls++;
        _data->_stats._vertices += points._vertexCount;

        points._vertexCount = 0;
        points._vertexDataPtr = points._vertexData;
    }

    void iRenderer2::flush()
    {
        flushTexQuads();
        flushQuads();
        flushLines();
        flushPoints();

        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;
    }

    void iRenderer2::flushLastUsed()
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

        case iRenderDataSet::TexturedQuads:
            flushTexQuads();
            break;
        }

        _data->_lastRenderDataSetUsed = iRenderDataSet::NoDataSet;
    }

    void iRenderer2::setIgnoreRenderOrder(bool ignoreRenderOrder)
    {
        _data->_keepRenderOrder = !ignoreRenderOrder;
        flush();
    }

    bool iRenderer2::isIgnoringRenderOrder()
    {
        return !_data->_keepRenderOrder;
    }

    void iRenderer2::setLineWidth(float32 lineWidth)
    {
        if (_data->_lineWidth == lineWidth)
        {
            return;
        }

        // draw previous lines and then set new width
        flushLines();

        _data->_lineWidth = lineWidth;
        glLineWidth(_data->_lineWidth);
    }

    float32 iRenderer2::getLineWidth() const
    {
        return _data->_lineWidth;
    }

    void iRenderer2::setPointSize(float32 pointSize)
    {
        if (_data->_pointSize == pointSize)
        {
            return;
        }

        // draw previous points and then set new size
        flushPoints();

        _data->_pointSize = pointSize;
        glPointSize(_data->_pointSize);
    }

    float32 iRenderer2::getPointSize() const
    {
        return _data->_pointSize;
    }

    void iRenderer2::updateMatrices()
    {
        _data->_modelViewMatrix = _data->_viewMatrix * _data->_modelMatrix;
        _data->_modelViewProjectionMatrix = _data->_projectionMatrix * _data->_modelViewMatrix;
    }

    void iRenderer2::setOrtho(float32 left, float32 right, float32 bottom, float32 top, float32 nearplain, float32 farplain)
    {
        iaMatrixd matrix;
        matrix.ortho(left, right, bottom, top, nearplain, farplain);
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer2::setPerspective(float32 fov, float32 aspect, float32 nearplain, float32 farplain)
    {
        iaMatrixd matrix;
        matrix.perspective(fov, aspect, nearplain, farplain);
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer2::setProjectionMatrix(const iaMatrixd &matrix)
    {
        if (_data->_projectionMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_projectionMatrix = matrix;
        updateMatrices();
    }

    void iRenderer2::setModelMatrix(const iaMatrixd &matrix)
    {
        if (_data->_modelMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_modelMatrix = matrix;
        updateMatrices();
    }

    void iRenderer2::setViewMatrix(const iaMatrixd &matrix)
    {
        if (_data->_viewMatrix == matrix)
        {
            return;
        }

        flush();

        _data->_viewMatrix = matrix;
        updateMatrices();
    }

    void iRenderer2::setViewMatrixFromCam(const iaMatrixd &camMatrix)
    {
        _data->_camMatrix = camMatrix;

        iaMatrixd camViewMatrix;
        camViewMatrix.lookAt(camMatrix._pos, camMatrix._pos - camMatrix._depth, camMatrix._top);

        setViewMatrix(camViewMatrix);
    }

    const iaMatrixd &iRenderer2::getModelMatrix() const
    {
        return _data->_modelMatrix;
    }

    const iaMatrixd &iRenderer2::getViewMatrix() const
    {
        return _data->_viewMatrix;
    }

    const iaMatrixd &iRenderer2::getProjectionMatrix() const
    {
        return _data->_projectionMatrix;
    }

    const iaMatrixd &iRenderer2::getModelViewMatrix() const
    {
        return _data->_modelViewMatrix;
    }

    const iaMatrixd &iRenderer2::getModelViewProjectionMatrix() const
    {
        return _data->_modelViewProjectionMatrix;
    }

    const iaMatrixf iRenderer2::getMVP() const
    {
        iaMatrixf matrix;
        for (int i = 0; i < 16; ++i)
        {
            matrix[i] = _data->_modelViewProjectionMatrix[i];
        }

        return matrix;
    }

    void iRenderer2::drawParticles(iParticle2DPtr particles, int32 particleCount, const iTexturePtr &texture, const iaGradientColor4f &gradient)
    {
        con_assert(particles != nullptr, "zero pointer");

        iaVector3f a, b, c, d;
        iaVector2f u, v;
        iaColor4f color;

        for (uint32 i = 0; i < particleCount; ++i)
        {
            const iParticle2D &particle = particles[i];

            if (particle._life <= 0)
            {
                continue;
            }

            const iaVector3f pos(particle._position._x, particle._position._y, 0.0f);

            gradient.getValue(particle._life, color);

            u.set(1.0f, 0.0f);
            u.rotateXY(particle._angle);
            v._x = -u._y;
            v._y = u._x;

            u *= 0.5f * particle._size;
            v *= 0.5f * particle._size;

            a = pos;
            a._x -= u._x;
            a._y -= u._y;
            a._x += v._x;
            a._y += v._y;

            b = pos;
            b._x += u._x;
            b._y += u._y;
            b._x += v._x;
            b._y += v._y;

            c = pos;
            c._x += u._x;
            c._y += u._y;
            c._x -= v._x;
            c._y -= v._y;

            d = pos;
            d._x -= u._x;
            d._y -= u._y;
            d._x -= v._x;
            d._y -= v._y;

            drawTexturedQuad(a, b, c, d, texture, color);
        }
    }

    void iRenderer2::drawString(float32 x, float32 y, const iaString &text, iHorizontalAlignment horz, iVerticalAlignment vert, const iaColor4f &color, float32 maxWidth)
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

    void iRenderer2::drawString(float32 x, float32 y, const iaString &text, const iaColor4f &color, float32 maxWidth)
    {
        if (text.isEmpty())
        {
            return;
        }

        bool blending = isBlendingActive();
        setBlendingActive(true);

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
            texQuads._vertexDataPtr->_texCoord.set(texX, texY + height);
            texQuads._vertexDataPtr->_texIndex = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x + renderSize._x, renderPos._y + renderSize._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord.set(texX + width, texY + height);
            texQuads._vertexDataPtr->_texIndex = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x + renderSize._x, renderPos._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord.set(texX + width, texY);
            texQuads._vertexDataPtr->_texIndex = textureIndex;
            texQuads._vertexDataPtr++;

            texQuads._vertexDataPtr->_pos.set(renderPos._x, renderPos._y, 0.0f);
            texQuads._vertexDataPtr->_color = color;
            texQuads._vertexDataPtr->_texCoord.set(texX, texY);
            texQuads._vertexDataPtr->_texIndex = textureIndex;
            texQuads._vertexDataPtr++;

            endTexturedQuad();

            renderPos._x += renderSize._x;
        }

        setBlendingActive(blending);
    }

    void iRenderer2::setFont(const iTextureFontPtr &font)
    {
        con_assert(font->isValid(), "invalid font used");

        _data->_font = font;
    }

    const iTextureFontPtr &iRenderer2::getFont() const
    {
        return _data->_font;
    }

    void iRenderer2::setFontSize(float32 fontSize)
    {
        _data->_fontSize = fontSize;
    }

    float32 iRenderer2::getFontSize() const
    {
        return _data->_fontSize;
    }

    void iRenderer2::setFontLineHeight(float32 lineheight)
    {
        _data->_fontLineHeight = lineheight;
    }

    float32 iRenderer2::getFontLineHeight() const
    {
        return _data->_fontLineHeight;
    }

    iRenderer2::iRendererStats iRenderer2::getStats() const
    {
        return _data->_stats;
    }

    void iRenderer2::clearStats()
    {
        _data->_stats._drawCalls = 0;
        _data->_stats._vertices = 0;
        _data->_stats._indices = 0;
        _data->_stats._triangles = 0;
    }

    const iaRectanglei &iRenderer2::getViewport() const
    {
        return _data->_viewport;
    }

    void iRenderer2::setViewport(const iaRectanglei &viewport)
    {
        if (_data->_viewport == viewport)
        {
            return;
        }

        flush();

        _data->_viewport = viewport;
        glViewport(viewport._x, viewport._y, viewport._width, viewport._height);
    }

    void iRenderer2::setViewport(int32 x, int32 y, int32 width, int32 height)
    {
        setViewport(iaRectanglei(x, y, width, height));
    }

    const iaColor4f &iRenderer2::getClearColor() const
    {
        return _data->_clearColor;
    }

    void iRenderer2::setClearColor(const iaColor4f &color)
    {
        _data->_clearColor = color;
        glClearColor(color._r, color._g, color._b, color._a);
    }

    float32 iRenderer2::getClearDepth() const
    {
        return _data->_clearDepth;
    }

    void iRenderer2::setClearDepth(float32 depth)
    {
        _data->_clearDepth = depth;
        glClearDepth(depth);
    }

    void iRenderer2::clearColorBuffer()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void iRenderer2::clearDepthBuffer()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void iRenderer2::clearStencilBuffer()
    {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    const iaVector3d iRenderer2::project(const iaVector3d &objectSpacePos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const
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

    const iaVector3d iRenderer2::unProject(const iaVector3d &screenpos, const iaMatrixd &modelview, const iaMatrixd &projection, const iaRectanglei &viewport) const
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

    static GLenum getOGLEnum(iRenderer2::iStencilFunction value)
    {
        switch (value)
        {
        case iRenderer2::iStencilFunction::Never:
            return GL_NEVER;
        case iRenderer2::iStencilFunction::Less:
            return GL_LESS;
        case iRenderer2::iStencilFunction::LessOrEqual:
            return GL_LEQUAL;
        case iRenderer2::iStencilFunction::Greater:
            return GL_GREATER;
        case iRenderer2::iStencilFunction::GreaterOrEqual:
            return GL_GEQUAL;
        case iRenderer2::iStencilFunction::Equal:
            return GL_EQUAL;
        case iRenderer2::iStencilFunction::NotEqual:
            return GL_NOTEQUAL;
        case iRenderer2::iStencilFunction::Always:
            return GL_ALWAYS;
        };

        con_crit("unknown value");
        return GL_NONE;
    }

    void iRenderer2::setStencilFunction(iStencilFunction function, int32 ref, uint32 mask)
    {
        glStencilFunc(getOGLEnum(function), ref, mask);
    }

    static GLenum getOGLEnum(iRenderer2::iStencilOperation value)
    {
        switch (value)
        {
        case iRenderer2::iStencilOperation::Zero:
            return GL_ZERO;
        case iRenderer2::iStencilOperation::Keep:
            return GL_KEEP;
        case iRenderer2::iStencilOperation::Replace:
            return GL_REPLACE;
        case iRenderer2::iStencilOperation::Increment:
            return GL_INCR;
        case iRenderer2::iStencilOperation::Decrement:
            return GL_DECR;
        case iRenderer2::iStencilOperation::Invert:
            return GL_INVERT;
        }

        con_err("invalid value");
        return GL_ZERO;
    }

    void iRenderer2::setStencilOperation(iStencilOperation fail, iStencilOperation zfail, iStencilOperation zpass)
    {
        glStencilOp(getOGLEnum(fail), getOGLEnum(zfail), getOGLEnum(zpass));
    }

    void iRenderer2::setStencilTestActive(bool enable)
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

    void iRenderer2::setStencilMask(uint8 mask)
    {
        glStencilMask(mask);
    }

    void iRenderer2::setBlendingActive(bool enable)
    {
        if (_data->_blendingActive == enable)
        {
            return;
        }

        flush();
        _data->_blendingActive = enable;
    }

    bool iRenderer2::isBlendingActive() const
    {
        return _data->_blendingActive;
    }

    void iRenderer2::setDepthTestActive(bool enable)
    {
        if (_data->_depthTestActive == enable)
        {
            return;
        }

        flush();
        _data->_depthTestActive = enable;
    }

    bool iRenderer2::isDepthTestActive() const
    {
        return _data->_depthTestActive;
    }

    void iRenderer2::drawBox(const iAACubed &box, const iaColor4f &color)
    {
        drawBox(iAABoxf(iaVector3f(box._center._x, box._center._y, box._center._z),
                        iaVector3f(box._halfEdgeLength, box._halfEdgeLength, box._halfEdgeLength)),
                color);
    }

    void iRenderer2::drawBox(const iAACubef &box, const iaColor4f &color)
    {
        drawBox(iAABoxf(iaVector3f(box._center._x, box._center._y, box._center._z),
                        iaVector3f(box._halfEdgeLength, box._halfEdgeLength, box._halfEdgeLength)),
                color);
    }

    void iRenderer2::drawBox(const iAABoxd &box, const iaColor4f &color)
    {
        drawBox(iAABoxf(iaVector3f(box._center._x, box._center._y, box._center._z),
                        iaVector3f(box._halfWidths._x, box._halfWidths._y, box._halfWidths._z)),
                color);
    }

    void iRenderer2::drawBox(const iAABoxf &box, const iaColor4f &color)
    {
        iaVector3f a = box._center;
        a -= box._halfWidths;
        iaVector3f b = box._center;
        b += box._halfWidths;

        drawLine(iaVector3f(a._x, a._y, a._z),
                 iaVector3f(b._x, a._y, a._z), color);

        drawLine(iaVector3f(a._x, a._y, a._z),
                 iaVector3f(a._x, b._y, a._z), color);

        drawLine(iaVector3f(a._x, a._y, a._z),
                 iaVector3f(a._x, a._y, b._z), color);

        drawLine(iaVector3f(b._x, a._y, a._z),
                 iaVector3f(b._x, a._y, b._z), color);

        drawLine(iaVector3f(b._x, a._y, a._z),
                 iaVector3f(b._x, b._y, a._z), color);

        drawLine(iaVector3f(b._x, a._y, b._z),
                 iaVector3f(b._x, b._y, b._z), color);

        drawLine(iaVector3f(a._x, a._y, b._z),
                 iaVector3f(b._x, a._y, b._z), color);

        drawLine(iaVector3f(a._x, a._y, b._z),
                 iaVector3f(a._x, b._y, b._z), color);

        drawLine(iaVector3f(a._x, b._y, a._z),
                 iaVector3f(a._x, b._y, b._z), color);

        drawLine(iaVector3f(a._x, b._y, b._z),
                 iaVector3f(b._x, b._y, b._z), color);

        drawLine(iaVector3f(a._x, b._y, a._z),
                 iaVector3f(b._x, b._y, a._z), color);

        drawLine(iaVector3f(b._x, b._y, a._z),
                 iaVector3f(b._x, b._y, b._z), color);
    }

}
