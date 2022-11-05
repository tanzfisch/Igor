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

    static const uint32 MAX_QUADS = 100;
    static const uint32 MAX_QUAD_VERTICES = MAX_QUADS * 4;
    static const uint32 MAX_QUAD_INDICES = MAX_QUADS * 6;

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
        iaVector2f _texCoord;
        iaColor4f _color;
    };

    /*! all the data needed
     */
    struct iRendererData
    {
        /////////// POINTS //////////////
        /*! point vertex buffer
         */
        iVertexBufferPtr _pointVertexBuffer;

        /*! point vertex array
         */
        iVertexArrayPtr _pointVertexArray;

        /*! point vertex count
         */
        uint32 _pointVertexCount;

        /*! oing vertex data
         */
        iFlatVertex *_pointVertexData = nullptr;

        /*! pointer to current position in point vertex data
         */
        iFlatVertex *_pointVertexDataPtr = nullptr;

        /////////// LINES //////////////
        /*! line vertex buffer
         */
        iVertexBufferPtr _lineVertexBuffer;

        /*! line vertex array
         */
        iVertexArrayPtr _lineVertexArray;

        /*! line vertex count
         */
        uint32 _lineVertexCount;

        /*! line vertex data
         */
        iFlatVertex *_lineVertexData = nullptr;

        /*! pointer to current position in line vertex data
         */
        iFlatVertex *_lineVertexDataPtr = nullptr;

        /////////// FLAT QUADS //////////////
        /*! quad vertex buffer
         */
        iVertexBufferPtr _quadVertexBuffer;

        /*! quad index buffer
         */
        iIndexBufferPtr _quadIndexBuffer;

        /*! quad vertex array
         */
        iVertexArrayPtr _quadVertexArray;

        /*! quad vertex count
         */
        uint32 _quadVertexCount;

        /*! quad vertex count
         */
        uint32 _quadIndexCount;

        /*! quad vertex data
         */
        iFlatVertex *_quadVertexData = nullptr;        

        /*! pointer to current position in quad vertex data
         */
        iFlatVertex *_quadVertexDataPtr = nullptr;

        /*! quad index data
         */
        uint32 *_quadIndexData = nullptr;     

        /*! shader for single color flat shading
         */
        iShaderProgramPtr _flatShader;

        // debug
        uint32 _drawCalls;
    };

    /*! internal data
     */
    static iRendererData s_data;

    void iRenderer2::init()
    {
        /////////// LINES //////////////
        s_data._lineVertexArray = iVertexArray::create();

        s_data._lineVertexBuffer = iVertexBuffer::create(MAX_LINE_VERTICES * sizeof(iFlatVertex));
        s_data._lineVertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        s_data._lineVertexArray->addVertexBuffer(s_data._lineVertexBuffer);

        s_data._lineVertexData = new iFlatVertex[MAX_LINE_VERTICES];
        s_data._lineVertexDataPtr = s_data._lineVertexData;
        s_data._lineVertexCount = 0;

        //////////// POINTS /////////////
        s_data._pointVertexArray = iVertexArray::create();

        s_data._pointVertexBuffer = iVertexBuffer::create(MAX_POINT_VERTICES * sizeof(iFlatVertex));
        s_data._pointVertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        s_data._pointVertexArray->addVertexBuffer(s_data._pointVertexBuffer);

        s_data._pointVertexData = new iFlatVertex[MAX_POINT_VERTICES];
        s_data._pointVertexDataPtr = s_data._pointVertexData;
        s_data._pointVertexCount = 0;

        //////////// FLAT QUADS /////////////
        s_data._quadVertexArray = iVertexArray::create();

        s_data._quadVertexBuffer = iVertexBuffer::create(MAX_QUAD_VERTICES * sizeof(iFlatVertex));
        s_data._quadVertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        s_data._quadVertexArray->addVertexBuffer(s_data._quadVertexBuffer);

        s_data._quadIndexData = new uint32[MAX_QUAD_INDICES];

        for(uint32 i=0;i<MAX_QUADS;++i)
        {
            s_data._quadIndexData[i*6 + 0] = i*4 + 0;
            s_data._quadIndexData[i*6 + 1] = i*4 + 1;
            s_data._quadIndexData[i*6 + 2] = i*4 + 3;

            s_data._quadIndexData[i*6 + 3] = i*4 + 1;
            s_data._quadIndexData[i*6 + 4] = i*4 + 2;
            s_data._quadIndexData[i*6 + 5] = i*4 + 3;
        }

        s_data._quadIndexBuffer = iIndexBuffer::create(MAX_QUAD_INDICES, s_data._quadIndexData);
        s_data._quadVertexArray->setIndexBuffer(s_data._quadIndexBuffer);

        s_data._quadVertexData = new iFlatVertex[MAX_QUAD_VERTICES];
        s_data._quadVertexDataPtr = s_data._quadVertexData;
        s_data._quadVertexCount = 0;
        s_data._quadIndexCount = 0;

        ///////////// SHADER ////////////
        s_data._flatShader = iShaderProgram::create();
        s_data._flatShader->addShader("igor/flat_shader.vert", iShaderObjectType_New::Vertex);
        s_data._flatShader->addShader("igor/flat_shader.frag", iShaderObjectType_New::Fragment);
        s_data._flatShader->compile();
    }

    void iRenderer2::deinit()
    {
        delete[] s_data._lineVertexData;
        s_data._lineVertexDataPtr = s_data._lineVertexData = nullptr;

        delete[] s_data._pointVertexData;
        s_data._pointVertexDataPtr = s_data._pointVertexData = nullptr;

        delete[] s_data._quadVertexData;
        s_data._quadVertexDataPtr = s_data._quadVertexData = nullptr;
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
        if (s_data._pointVertexCount >= MAX_POINT_VERTICES)
        {
            flushPoints();
        }

        s_data._pointVertexDataPtr->_pos = v;
        s_data._pointVertexDataPtr->_color = color;
        s_data._pointVertexDataPtr++;

        s_data._pointVertexCount++;
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
        if (s_data._quadVertexCount >= MAX_QUAD_VERTICES)
        {
            flushQuads();
        }

        s_data._quadVertexDataPtr->_pos.set(x, y, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(x, y + height, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(x + width, y + height, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(x + width, y, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexCount += 4;
        s_data._quadIndexCount += 6;
    }

    void iRenderer2::drawFilledRectangle(const iaRectanglef &rect, const iaColor4f &color)
    {
        drawFilledRectangle(rect._x, rect._y, rect._width, rect._height, color);
    }

    void iRenderer2::drawFilledRectangleOnCenter(const iaVector2f &center, const iaVector2f &size, const iaColor4f &color)
    {
        if (s_data._quadVertexCount >= MAX_QUAD_VERTICES)
        {
            flushQuads();
        }

        s_data._quadVertexDataPtr->_pos.set(center._x - size._x * 0.5f, center._y + size._y * 0.5f, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(center._x + size._x * 0.5f, center._y + size._y * 0.5f, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(center._x + size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexDataPtr->_pos.set(center._x - size._x * 0.5f, center._y - size._y * 0.5f, 0.0);
        s_data._quadVertexDataPtr->_color = color;
        s_data._quadVertexDataPtr++;

        s_data._quadVertexCount += 4;
        s_data._quadIndexCount += 6;
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
        if (s_data._lineVertexCount >= MAX_LINE_VERTICES)
        {
            flushLines();
        }

        s_data._lineVertexDataPtr->_pos = v1;
        s_data._lineVertexDataPtr->_color = color;
        s_data._lineVertexDataPtr++;

        s_data._lineVertexDataPtr->_pos = v2;
        s_data._lineVertexDataPtr->_color = color;
        s_data._lineVertexDataPtr++;

        s_data._lineVertexCount += 2;
    }

    void iRenderer2::flushQuads()
    {
        if (s_data._quadVertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)s_data._quadVertexDataPtr - (uint8 *)s_data._quadVertexData);
        s_data._quadVertexBuffer->setData(dataSize, s_data._quadVertexData);

        s_data._quadVertexArray->bind();
        glDrawElements(GL_TRIANGLES, s_data._quadIndexCount, GL_UNSIGNED_INT, nullptr);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        s_data._quadVertexArray->unbind();

        s_data._flatShader->unbind();

        s_data._quadVertexCount = 0;
        s_data._quadIndexCount = 0;
        s_data._quadVertexDataPtr = s_data._quadVertexData;
    }

    void iRenderer2::flushLines()
    {
        if (s_data._lineVertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)s_data._lineVertexDataPtr - (uint8 *)s_data._lineVertexData);
        s_data._lineVertexBuffer->setData(dataSize, s_data._lineVertexData);

        s_data._lineVertexArray->bind();
        glDrawArrays(GL_LINES, 0, s_data._lineVertexCount);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        s_data._lineVertexArray->unbind();

        s_data._flatShader->unbind();

        s_data._lineVertexCount = 0;
        s_data._lineVertexDataPtr = s_data._lineVertexData;
    }

    void iRenderer2::flushPoints()
    {
        if (s_data._pointVertexCount == 0)
        {
            return;
        }

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        uint32 dataSize = (uint32)((uint8 *)s_data._pointVertexDataPtr - (uint8 *)s_data._pointVertexData);
        s_data._pointVertexBuffer->setData(dataSize, s_data._pointVertexData);

        s_data._pointVertexArray->bind();
        glDrawArrays(GL_POINTS, 0, s_data._pointVertexCount);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        s_data._pointVertexArray->unbind();

        s_data._flatShader->unbind();

        s_data._pointVertexCount = 0;
        s_data._pointVertexDataPtr = s_data._pointVertexData;
    }

    void iRenderer2::flush()
    {
        flushQuads();
        flushLines();
        flushPoints();

        con_endl("s_data._drawCalls " << s_data._drawCalls);

        s_data._drawCalls = 0;
    }
}
