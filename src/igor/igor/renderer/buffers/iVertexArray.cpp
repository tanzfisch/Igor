// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/buffers/iVertexArray.h>

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

    class iVertexArrayDeleter
    {
    public:
        void operator()(iVertexArray *p) { delete p; }
    };

    iVertexArrayPtr iVertexArray::create()
    {
        return std::shared_ptr<iVertexArray>(new iVertexArray(), iVertexArrayDeleter());
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

    uint32 iVertexArray::getVertexCount() const
    {
        if(_vertexBuffers.empty())
        {
            return 0;
        }

        uint32 result = 0;

        for(const auto &vertexBuffer : _vertexBuffers)
        {
            result += vertexBuffer->getVertexCount();
        }

        return result;
    }

    uint32 iVertexArray::getIndexCount() const
    {
        if(_indexBuffer == nullptr)
        {
            return 0;
        }

        return _indexBuffer->getIndexCount();
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
                glEnableVertexArrayAttrib(_vertexArrayObject, _totalComponentCount);
                GL_CHECK_ERROR();
                glVertexArrayAttribBinding(_vertexArrayObject, _totalComponentCount, 0);
                GL_CHECK_ERROR();
                glVertexArrayAttribFormat(_vertexArrayObject,
                                          _totalComponentCount,
                                          component.getComponentCount(),
                                          iRendererUtils::convertType(component._type),
                                          component._normalized ? GL_TRUE : GL_FALSE,
                                          component._offset);
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
                glEnableVertexArrayAttrib(_vertexArrayObject, _totalComponentCount);
                GL_CHECK_ERROR();
                glVertexArrayAttribBinding(_vertexArrayObject, _totalComponentCount, 0);
                GL_CHECK_ERROR();
                glVertexArrayAttribFormat(_vertexArrayObject,
                                          _totalComponentCount,
                                          component.getComponentCount(),
                                          iRendererUtils::convertType(component._type),
                                          component._normalized ? GL_TRUE : GL_FALSE,
                                          component._offset);
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
                    glEnableVertexArrayAttrib(_vertexArrayObject, _totalComponentCount);
                    glVertexArrayAttribBinding(_vertexArrayObject, _totalComponentCount, 0);

                    const uint32 offset = component._offset + sizeof(float) * count * i;

                    glVertexArrayAttribFormat(_vertexArrayObject,
                                          _totalComponentCount,
                                          component.getComponentCount(),
                                          iRendererUtils::convertType(component._type),
                                          component._normalized ? GL_TRUE : GL_FALSE,
                                          offset);

                    glVertexArrayBindingDivisor(_vertexArrayObject, _totalComponentCount, 1);
                    GL_CHECK_ERROR();
                    _totalComponentCount++;
                }
                break;
            }
            default:
                con_crit("Unknown shader data type");
            }
        }

        glVertexArrayVertexBuffer(_vertexArrayObject, 0, vertexBuffer->_vertexBufferObject, 0, info.getStride());
        GL_CHECK_ERROR();

        _vertexBuffers.push_back(vertexBuffer);
    }

    void iVertexArray::setIndexBuffer(const iIndexBufferPtr &indexBuffer)
    {
        glVertexArrayElementBuffer(_vertexArrayObject, indexBuffer->_indexBufferObject);
        GL_CHECK_ERROR();
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
