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
    static const uint32 MAX_VERTICES = 100000;

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

        iShaderProgramPtr _lineShader;
    };

    static iRendererData s_data;

    void iRenderer2::flush()
    {
        /////////// LINES //////////////
        if (s_data._lineVertexCount)
        {
            uint32 dataSize = (uint32)((uint8 *)s_data._lineVertexDataPtr - (uint8 *)s_data._lineVertexData);
            s_data._lineVertexBuffer->setData(dataSize, s_data._lineVertexData);

            s_data._lineShader->bind();

            iaMatrixf dst;
            iaMatrixd mvp = iRenderer::getInstance().getModelViewProjectionMatrix();
            for(int i=0;i<16;++i)
            {
                dst[i] = mvp[i];
            }

            s_data._lineShader->setMatrix("igor_modelViewProjection", dst);

            s_data._lineVertexArray->bind();
            glDrawArrays(GL_TRIANGLES, 0, s_data._lineVertexCount);
            GL_CHECK_ERROR();
            s_data._lineVertexArray->unbind();

            s_data._lineShader->unbind();

            s_data._lineVertexCount = 0;
            s_data._lineVertexDataPtr = s_data._lineVertexData;
        }
    }

    void iRenderer2::init()
    {
        /////////// LINES //////////////
        s_data._lineShader = iShaderProgram::create();
        s_data._lineShader->addShader("igor/line_shader.vert", iShaderObjectType_New::Vertex);
        s_data._lineShader->addShader("igor/line_shader.frag", iShaderObjectType_New::Fragment);
        s_data._lineShader->compile();

        s_data._lineVertexArray = iVertexArray::create();

        s_data._lineVertexBuffer = iVertexBuffer::create(MAX_VERTICES * sizeof(iLineVertex));
        s_data._lineVertexBuffer->setLayout(std::vector<iBufferLayoutEntry>{{iShaderDataType::Float3}, {iShaderDataType::Float4}});
        s_data._lineVertexArray->addVertexBuffer(s_data._lineVertexBuffer);

        s_data._lineVertexData = new iLineVertex[MAX_VERTICES];
        s_data._lineVertexDataPtr = s_data._lineVertexData;
        s_data._lineVertexCount = 0;
    }

    void iRenderer2::deinit()
    {
        delete[] s_data._lineVertexData;
        s_data._lineVertexDataPtr = s_data._lineVertexData = nullptr;
    }

    void iRenderer2::drawLine(float32 x1, float32 y1, float32 x2, float32 y2, const iaColor4f &color)
    {
        drawLine(iaVector3f(x1, y1, 0.0), iaVector3f(x2, y2, 0.0), color);
    }

    void iRenderer2::drawLine(const iaVector2f &p1, const iaVector2f &p2, const iaColor4f &color)
    {
        drawLine(iaVector3f(p1._x, p1._y, 0.0), iaVector3f(p2._x, p2._y, 0.0), color);
    }

    void iRenderer2::drawLine(const iaVector3f &p1, const iaVector3f &p2, const iaColor4f &color)
    {
        s_data._lineVertexDataPtr->_pos = p1;
        s_data._lineVertexDataPtr->_color = color;
        s_data._lineVertexDataPtr++;

        s_data._lineVertexDataPtr->_pos = p2;
        s_data._lineVertexDataPtr->_color = color;
        s_data._lineVertexDataPtr++;

        s_data._lineVertexCount += 2;
    }

}
