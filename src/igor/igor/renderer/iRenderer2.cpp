// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/iRenderer2.h>

#include <igor/renderer/iRenderer.h>

#include <igor/renderer/shader/iShaderProgram.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{
    /*! some arbitrary buffer size
     */
    static const uint32 MAX_LINE_VERTICES = 10000;

    // line vertex definition
    struct iLineVertex
    {
        iaVector3f _pos;
        iaColor4f _color;
    };

    /*! all the data needed
     */
    struct iRendererData
    {
        bool _initialized = false;

        /////////// LINES //////////////

        /*! line vertex buffer
         */
        iVertexBufferPtr _lineVertexBuffer;

        /*! line vertex array
         */
        iVertexArrayPtr _lineVertexArray;

        uint32 _lineVertexCount = 0;
        iLineVertex *_lineVertexData = nullptr;
        iLineVertex *_lineVertexDataPtr = nullptr;
        float32 _lineWidth = 1.0f;

        /*! shader for single color flat shading
        */
        iShaderProgramPtr _flatShader;

        uint32 _drawCalls;
    };

    /*! internal data
    */
    static iRendererData s_data;

    void iRenderer2::init()
    {
        /////////// LINES //////////////
        s_data._flatShader = iShaderProgram::create();
        s_data._flatShader->addShader("igor/flat_shader.vert", iShaderObjectType_New::Vertex);
        s_data._flatShader->addShader("igor/flat_shader.frag", iShaderObjectType_New::Fragment);
        s_data._flatShader->compile();

        s_data._lineVertexArray = iVertexArray::create();

        s_data._lineVertexBuffer = iVertexBuffer::create(MAX_LINE_VERTICES * sizeof(iLineVertex));
        s_data._lineVertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        s_data._lineVertexArray->addVertexBuffer(s_data._lineVertexBuffer);

        s_data._lineVertexData = new iLineVertex[MAX_LINE_VERTICES];
        s_data._lineVertexDataPtr = s_data._lineVertexData;
        s_data._lineVertexCount = 0;
    }

    void iRenderer2::deinit()
    {
        delete[] s_data._lineVertexData;
        s_data._lineVertexDataPtr = s_data._lineVertexData = nullptr;
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
        iaVector3f v0(rect._x, rect._y, 0.0);
		iaVector3f v1(rect._x, rect._y + rect._height, 0.0);
		iaVector3f v2(rect._x + rect._width, rect._y + rect._height, 0.0);
		iaVector3f v3(rect._x + rect._width, rect._y, 0.0);

		drawLine(v0, v1, color);
		drawLine(v1, v2, color);
		drawLine(v2, v3, color);
		drawLine(v3, v0, color);
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

    void iRenderer2::flushLines()
    {
        if (s_data._lineVertexCount == 0)
        {
            return;
        }

        uint32 dataSize = (uint32)((uint8 *)s_data._lineVertexDataPtr - (uint8 *)s_data._lineVertexData);
        s_data._lineVertexBuffer->setData(dataSize, s_data._lineVertexData);

        s_data._flatShader->bind();

        // TODO
        iaMatrixf mvp;
        mvp.ortho(0, 500, 500, 0, 0.0, 100);
        mvp.translate(0, 0, -1);
        s_data._flatShader->setMatrix("igor_modelViewProjection", mvp);

        s_data._lineVertexArray->bind();
        glDrawArrays(GL_LINES, 0, s_data._lineVertexCount);
        s_data._drawCalls++;
        GL_CHECK_ERROR();
        s_data._lineVertexArray->unbind();

        s_data._flatShader->unbind();

        s_data._lineVertexCount = 0;
        s_data._lineVertexDataPtr = s_data._lineVertexData;
    }

    void iRenderer2::flush()
    {
        flushLines();

        con_endl("s_data._drawCalls " << s_data._drawCalls);

        s_data._drawCalls = 0;
    }
}
