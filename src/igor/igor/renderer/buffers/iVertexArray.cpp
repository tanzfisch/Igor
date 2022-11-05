// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iVertexArray.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{
    static GLenum shaderDataTypeToOpenGLBaseType(iShaderDataType type)
    {
        switch (type)
        {
        case iShaderDataType::Float:
            return GL_FLOAT;
        case iShaderDataType::Float2:
            return GL_FLOAT;
        case iShaderDataType::Float3:
            return GL_FLOAT;
        case iShaderDataType::Float4:
            return GL_FLOAT;
        case iShaderDataType::Matrix3x3:
            return GL_FLOAT;
        case iShaderDataType::Matrix4x4:
            return GL_FLOAT;
        case iShaderDataType::Int:
            return GL_INT;
        case iShaderDataType::Int2:
            return GL_INT;
        case iShaderDataType::Int3:
            return GL_INT;
        case iShaderDataType::Int4:
            return GL_INT;
        case iShaderDataType::Boolean:
            return GL_BOOL;
        }

        con_crit("Unknown shader data type");
        return 0;
    }

    iVertexArrayPtr iVertexArray::create()
    {
        return std::make_shared<iVertexArray>();
    }

    iVertexArray::iVertexArray()
    {
        glCreateVertexArrays(1, &_vertexArrayObject);
        GL_CHECK_ERROR();
    }

    iVertexArray::~iVertexArray()
    {
        glDeleteVertexArrays(1, &_vertexArrayObject);
        GL_CHECK_ERROR();
    }

    void iVertexArray::bind() const
    {
        glBindVertexArray(_vertexArrayObject);
        GL_CHECK_ERROR();
    }

    void iVertexArray::unbind()
    {
        glBindVertexArray(INVALID_BUFFER_ID);
        GL_CHECK_ERROR();
    }

    void iVertexArray::addVertexBuffer(const iVertexBufferPtr &vertexBuffer)
    {
        con_assert(vertexBuffer->getLayout().getElements().size(), "Vertex buffer has no layout");

        glBindVertexArray(_vertexArrayObject);
        GL_CHECK_ERROR();
        vertexBuffer->bind();

        const auto &info = vertexBuffer->getLayout();
        for (const auto &component : info.getElements())
        {
            switch (component._type)
            {
            case iShaderDataType::Float:
            case iShaderDataType::Float2:
            case iShaderDataType::Float3:
            case iShaderDataType::Float4:
            {
                glEnableVertexAttribArray(_totalComponentCount);
                GL_CHECK_ERROR();

                glVertexAttribPointer(_totalComponentCount,
                                      component.getComponentCount(),
                                      shaderDataTypeToOpenGLBaseType(component._type),
                                      component._normalized ? GL_TRUE : GL_FALSE,
                                      info.getStride(),
                                      BUFFER_OFFSET(component._offset));
                GL_CHECK_ERROR();
                _totalComponentCount++;
                break;
            }
            case iShaderDataType::Int:
            case iShaderDataType::Int2:
            case iShaderDataType::Int3:
            case iShaderDataType::Int4:
            case iShaderDataType::Boolean:
            {
                glEnableVertexAttribArray(_totalComponentCount);
                GL_CHECK_ERROR();

                glVertexAttribIPointer(_totalComponentCount,
                                       component.getComponentCount(),
                                       shaderDataTypeToOpenGLBaseType(component._type),
                                       info.getStride(),
                                       BUFFER_OFFSET(component._offset));
                GL_CHECK_ERROR();
                _totalComponentCount++;
                break;
            }
            case iShaderDataType::Matrix3x3:
            case iShaderDataType::Matrix4x4:
            {
                uint8_t count = component.getComponentCount();
                for (uint8_t i = 0; i < count; i++)
                {
                    glEnableVertexAttribArray(_totalComponentCount);
                    GL_CHECK_ERROR();

                    glVertexAttribPointer(_totalComponentCount,
                                          count,
                                          shaderDataTypeToOpenGLBaseType(component._type),
                                          component._normalized ? GL_TRUE : GL_FALSE,
                                          info.getStride(),
                                          BUFFER_OFFSET(component._offset + sizeof(float) * count * i));
                    GL_CHECK_ERROR();
                    glVertexAttribDivisor(_totalComponentCount, 1);
                    GL_CHECK_ERROR();
                    _totalComponentCount++;
                }
                break;
            }
            default:
                con_crit("Unknown shader data type");
            }
        }

        _vertexBuffers.push_back(vertexBuffer);
    }

    void iVertexArray::setIndexBuffer(const iIndexBufferPtr &indexBuffer)
    {
        glBindVertexArray(_vertexArrayObject);
        GL_CHECK_ERROR();
        indexBuffer->bind();

        _indexBuffer = indexBuffer;
    }

    const std::vector<iVertexBufferPtr> &iVertexArray::getVertexBuffers() const
    {
        return _vertexBuffers;
    }
    const iIndexBufferPtr &iVertexArray::getIndexBuffer() const
    {
        return _indexBuffer;
    }

}
