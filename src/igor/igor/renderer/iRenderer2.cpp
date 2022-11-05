// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderer2.h>

#include <igor/renderer/iRenderer.h>

#include <igor/renderer/shader/iShaderProgram.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{
    static const uint32 MAX_POINT_VERTICES = 10000;

    static const uint32 MAX_LINES = 10000;
    static const uint32 MAX_LINE_VERTICES = MAX_LINES * 2;

    static const uint32 MAX_QUADS = 10000;
    static const uint32 MAX_QUAD_VERTICES = MAX_QUADS * 4;
    static const uint32 MAX_QUAD_INDICES = MAX_QUADS * 6;

    static const iaVector2f QUAD_TEXTURE_COORDS[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

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
        int32 _texIndex;
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
        uint32 *_quadIndexData = nullptr;

        /*! quad index buffer
         */
        iIndexBufferPtr _quadIndexBuffer;        

        ////// DEBUG ////
        uint32 _drawCalls;
    };

    /*! internal data
     */
    static iRendererData s_data;

    void iRenderer2::init()
    {
        /////////// LINES //////////////
        auto &lines = s_data._lines;
        lines._vertexArray = iVertexArray::create();

        lines._vertexBuffer = iVertexBuffer::create(MAX_LINE_VERTICES * sizeof(iFlatVertex));
        lines._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        lines._vertexArray->addVertexBuffer(lines._vertexBuffer);

        lines._vertexData = new iFlatVertex[MAX_LINE_VERTICES];
        lines._vertexDataPtr = lines._vertexData;
        lines._vertexCount = 0;

        //////////// POINTS /////////////
        auto &points = s_data._points;
        points._vertexArray = iVertexArray::create();

        points._vertexBuffer = iVertexBuffer::create(MAX_POINT_VERTICES * sizeof(iFlatVertex));
        points._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        points._vertexArray->addVertexBuffer(points._vertexBuffer);

        points._vertexData = new iFlatVertex[MAX_POINT_VERTICES];
        points._vertexDataPtr = points._vertexData;
        points._vertexCount = 0;

        //////// QUADS comon data ///////////
        s_data._quadIndexData = new uint32[MAX_QUAD_INDICES];

        for (uint32 i = 0; i < MAX_QUADS; ++i)
        {
            s_data._quadIndexData[i * 6 + 0] = i * 4 + 0;
            s_data._quadIndexData[i * 6 + 1] = i * 4 + 1;
            s_data._quadIndexData[i * 6 + 2] = i * 4 + 3;

            s_data._quadIndexData[i * 6 + 3] = i * 4 + 1;
            s_data._quadIndexData[i * 6 + 4] = i * 4 + 2;
            s_data._quadIndexData[i * 6 + 5] = i * 4 + 3;
        }

        s_data._quadIndexBuffer = iIndexBuffer::create(MAX_QUAD_INDICES, s_data._quadIndexData);

        //////////// FLAT QUADS /////////////
        auto &quads = s_data._quads;
        quads._vertexArray = iVertexArray::create();

        quads._vertexBuffer = iVertexBuffer::create(MAX_QUAD_VERTICES * sizeof(iFlatVertex));
        quads._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        quads._vertexArray->addVertexBuffer(quads._vertexBuffer);
        quads._vertexArray->setIndexBuffer(s_data._quadIndexBuffer);

        quads._vertexData = new iFlatVertex[MAX_QUAD_VERTICES];
        quads._vertexDataPtr = quads._vertexData;
        quads._vertexCount = 0;
        quads._indexCount = 0;

        //////////// TEXTURED QUADS /////////////
        auto &texQuads = s_data._texQuads;
        texQuads._vertexArray = iVertexArray::create();

        texQuads._vertexBuffer = iVertexBuffer::create(MAX_QUAD_VERTICES * sizeof(iTexturedVertex));
        texQuads._vertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}, {iShaderDataType::Float2}, {iShaderDataType::Int}});
        texQuads._vertexArray->addVertexBuffer(texQuads._vertexBuffer);
        texQuads._vertexArray->setIndexBuffer(s_data._quadIndexBuffer);

        texQuads._vertexData = new iTexturedVertex[MAX_QUAD_VERTICES];
        texQuads._vertexDataPtr = texQuads._vertexData;
        texQuads._vertexCount = 0;
        texQuads._indexCount = 0;

        texQuads._nextTextureIndex = 0;

        ///////////// SHADER ////////////
        s_data._flatShader = iShaderProgram::create();
        s_data._flatShader->addShader("igor/renderer/flat_shader.vert", iShaderObjectType_New::Vertex);
        s_data._flatShader->addShader("igor/renderer/flat_shader.frag", iShaderObjectType_New::Fragment);
        s_data._flatShader->compile();

        s_data._textureShader = iShaderProgram::create();
        s_data._textureShader->addShader("igor/renderer/textured_shader.vert", iShaderObjectType_New::Vertex);
        s_data._textureShader->addShader("igor/renderer/flat_shader.frag", iShaderObjectType_New::Fragment);
        s_data._textureShader->compile();
    }

    void iRenderer2::deinit()
    {
        auto &lines = s_data._lines;
        lines._vertexBuffer = nullptr;
        lines._vertexArray = nullptr;
        delete[] lines._vertexData;
        lines._vertexDataPtr = lines._vertexData = nullptr;

        auto &points = s_data._points;
        points._vertexBuffer = nullptr;
        points._vertexArray = nullptr;
        delete[] points._vertexData;
        points._vertexDataPtr = points._vertexData = nullptr;

        auto &quads = s_data._quads;
        quads._vertexBuffer = nullptr;
        quads._vertexArray = nullptr;
        delete[] quads._vertexData;
        quads._vertexDataPtr = quads._vertexData = nullptr;

        auto &texQuads = s_data._texQuads;
        texQuads._vertexBuffer = nullptr;
        texQuads._vertexArray = nullptr;
        delete[] texQuads._vertexData;
        texQuads._vertexDataPtr = texQuads._vertexData = nullptr;

        s_data._quadIndexBuffer = nullptr;
        delete[] s_data._quadIndexData;
        s_data._quadIndexData = nullptr;        
    }

    void iRenderer2::drawTexture(float32 x, float32 y, float32 width, float32 height, const iTexturePtr &texture, const iaColor4f &color)
    {
        auto &texQuads = s_data._texQuads;

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
                con_endl("flushTexQuads");
                flushTexQuads();
            }

            con_endl("use next texture " << texQuads._nextTextureIndex);
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
        texQuads._vertexDataPtr->_texCoord = QUAD_TEXTURE_COORDS[1];
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos.set(x + width, y + height, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = QUAD_TEXTURE_COORDS[2];
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexDataPtr->_pos.set(x + width, y, 0.0);
        texQuads._vertexDataPtr->_color = color;
        texQuads._vertexDataPtr->_texCoord = QUAD_TEXTURE_COORDS[3];
        texQuads._vertexDataPtr->_texIndex = textureIndex;
        texQuads._vertexDataPtr++;

        texQuads._vertexCount += 4;
        texQuads._indexCount += 6;
    }

    void iRenderer2::drawTexture(const iaRectanglef &rect, const iTexturePtr &texture, const iaColor4f &color)
    {
    }

    void iRenderer2::drawTextureOnCenter(const iaVector2f &center, const iaVector2f &size, const iTexturePtr &texture, const iaColor4f &color)
    {
    }

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
        auto &points = s_data._points;

        if (points._vertexCount >= MAX_POINT_VERTICES)
        {
            flushPoints();
        }

        points._vertexDataPtr->_pos = v;
        points._vertexDataPtr->_color = color;
        points._vertexDataPtr++;

        points._vertexCount++;
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

    void iRenderer2::drawRectangleOnCenter(const iaVector2f &center, const iaVector2f &size, const iaColor4f &color)
    {
        iaVector3f v0(center._x - size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        iaVector3f v1(center._x + size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        iaVector3f v2(center._x + size._x * 0.5f, center._y + size._y * 0.5f, 0.0);
        iaVector3f v3(center._x - size._x * 0.5f, center._y + size._y * 0.5f, 0.0);

        drawLine(v0, v1, color);
        drawLine(v1, v2, color);
        drawLine(v2, v3, color);
        drawLine(v3, v0, color);
    }

    void iRenderer2::drawFilledRectangle(float32 x, float32 y, float32 width, float32 height, const iaColor4f &color)
    {
        auto &quads = s_data._quads;

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
    }

    void iRenderer2::drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        drawFilledRectangle(rect._x, rect._y, rect._width, rect._height, color);
    }

    void iRenderer2::drawFilledRectangleOnCenter(const iaVector2f &center, const iaVector2f &size, const iaColor4f &color)
    {
        auto &quads = s_data._quads;

        if (quads._vertexCount >= MAX_QUAD_VERTICES)
        {
            flushQuads();
        }

        quads._vertexDataPtr->_pos.set(center._x - size._x * 0.5f, center._y + size._y * 0.5f, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(center._x + size._x * 0.5f, center._y + size._y * 0.5f, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(center._x + size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexDataPtr->_pos.set(center._x - size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        quads._vertexDataPtr->_color = color;
        quads._vertexDataPtr++;

        quads._vertexCount += 4;
        quads._indexCount += 6;
    }

    void iRenderer2::drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color)
    {
        drawLine(iaVector3f(x1, y1, 0.0), iaVector3f(x2, y2, 0.0), color);
    }

    void iRenderer2::drawLine(const iaVector2f &v1, const iaVector2f &v2, const iaColor4f &color)
    {
        drawLine(iaVector3f(v1._x, v1._y, 0.0), iaVector3f(v2._x, v2._y, 0.0), color);
    }

    void iRenderer2::drawLine(const iaVector3f &v1, const iaVector3f &v2, const iaColor4f &color)
    {
        auto &lines = s_data._lines;
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
    }

    void iRenderer2::flushTexQuads()
    {
        auto &texQuads = s_data._texQuads;
        
        if (texQuads._vertexCount == 0)
        {
            return;
        }

        s_data._textureShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._textureShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)texQuads._vertexDataPtr - (uint8 *)texQuads._vertexData);
        texQuads._vertexBuffer->setData(dataSize, texQuads._vertexData);

        texQuads._vertexArray->bind();
        glDrawElements(GL_TRIANGLES, texQuads._indexCount, GL_UNSIGNED_INT, nullptr);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        texQuads._vertexArray->unbind();

        s_data._textureShader->unbind();

        texQuads._vertexCount = 0;
        texQuads._indexCount = 0;
        texQuads._vertexDataPtr = texQuads._vertexData;

        texQuads._nextTextureIndex = 0;
    }

    void iRenderer2::flushQuads()
    {
        auto &quads = s_data._quads;

        if (quads._vertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)quads._vertexDataPtr - (uint8 *)quads._vertexData);
        quads._vertexBuffer->setData(dataSize, quads._vertexData);

        quads._vertexArray->bind();
        glDrawElements(GL_TRIANGLES, quads._indexCount, GL_UNSIGNED_INT, nullptr);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        quads._vertexArray->unbind();

        s_data._flatShader->unbind();

        quads._vertexCount = 0;
        quads._indexCount = 0;
        quads._vertexDataPtr = quads._vertexData;
    }

    void iRenderer2::flushLines()
    {
        auto &lines = s_data._lines;

        if (lines._vertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)lines._vertexDataPtr - (uint8 *)lines._vertexData);
        lines._vertexBuffer->setData(dataSize, lines._vertexData);

        lines._vertexArray->bind();
        glDrawArrays(GL_LINES, 0, lines._vertexCount);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        lines._vertexArray->unbind();

        s_data._flatShader->unbind();

        lines._vertexCount = 0;
        lines._vertexDataPtr = lines._vertexData;
    }

    void iRenderer2::flushPoints()
    {
        auto &points = s_data._points;

        if (points._vertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)points._vertexDataPtr - (uint8 *)points._vertexData);
        points._vertexBuffer->setData(dataSize, points._vertexData);

        points._vertexArray->bind();
        glDrawArrays(GL_POINTS, 0, points._vertexCount);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        points._vertexArray->unbind();

        s_data._flatShader->unbind();

        points._vertexCount = 0;
        points._vertexDataPtr = points._vertexData;
    }

    void iRenderer2::flush()
    {
        flushTexQuads();
        flushQuads();
        flushLines();
        flushPoints();

        con_endl("s_data._drawCalls " << s_data._drawCalls);

        s_data._drawCalls = 0;
    }
}
