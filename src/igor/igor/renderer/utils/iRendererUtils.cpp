// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

    GLenum iRendererUtils::convertType(iShaderDataType type)
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

    uint32 iRendererUtils::getSize(iShaderDataType type)
    {
        switch (type)
        {
        case iShaderDataType::Float:
            return 4;
        case iShaderDataType::Float2:
            return 4 * 2;
        case iShaderDataType::Float3:
            return 4 * 3;
        case iShaderDataType::Float4:
            return 4 * 4;
        case iShaderDataType::Matrix3x3:
            return 4 * 3 * 3;
        case iShaderDataType::Matrix4x4:
            return 4 * 4 * 4;
        case iShaderDataType::Int:
            return 4;
        case iShaderDataType::Int2:
            return 4 * 2;
        case iShaderDataType::Int3:
            return 4 * 3;
        case iShaderDataType::Int4:
            return 4 * 4;
        case iShaderDataType::Boolean:
            return 1;
        }

        con_crit("Unknown shader data type");
        return 0;
    }

    uint32 iRendererUtils::getComponentCount(iShaderDataType type)
    {
        switch (type)
        {
        case iShaderDataType::Float:
            return 1;
        case iShaderDataType::Float2:
            return 2;
        case iShaderDataType::Float3:
            return 3;
        case iShaderDataType::Float4:
            return 4;
        case iShaderDataType::Matrix3x3:
            return 3; // 3* float3
        case iShaderDataType::Matrix4x4:
            return 4; // 4* float4
        case iShaderDataType::Int:
            return 1;
        case iShaderDataType::Int2:
            return 2;
        case iShaderDataType::Int3:
            return 3;
        case iShaderDataType::Int4:
            return 4;
        case iShaderDataType::Boolean:
            return 1;
        }

        con_crit("Unknown shader data type");
        return 0;
    }

    uint32 iRendererUtils::convertType(iShaderObjectType type)
    {
        switch (type)
        {
        case iShaderObjectType::Vertex:
            return GL_VERTEX_SHADER;

        case iShaderObjectType::Fragment:
            return GL_FRAGMENT_SHADER;

        case iShaderObjectType::Geometry:
            return GL_GEOMETRY_SHADER;

        default:
            con_crit("unsupported shader type");
        }

        return 0;
    }

    std::wostream &operator<<(std::wostream &stream, const iShaderObjectType &type)
    {
        const static std::wstring text[] = {
            L"vertex",
            L"fragment",
            L"geometry"};

        stream << text[static_cast<int>(type)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iShaderDataType &type)
    {
        const static std::wstring text[] = {
            L"None",
            L"Float",
            L"Float2",
            L"Float3",
            L"Float4",
            L"Matrix3x3",
            L"Matrix4x4",
            L"Int",
            L"Int2",
            L"Int3",
            L"Int4",
            L"Boolean"};

        stream << text[static_cast<int>(type)];

        return stream;
    }

    uint32 iRendererUtils::convertType(iColorFormat format, bool sized)
    {
        switch (format)
        {
        case iColorFormat::RGB:            
            return sized ? GL_RGB8 : GL_RGB;
        case iColorFormat::RGBA:
            return sized ? GL_RGBA8 : GL_RGBA;
        case iColorFormat::BGR:
            return sized ? GL_RGB8 : GL_BGR;
        case iColorFormat::BGRA:
            return sized ? GL_RGBA8 : GL_BGRA;
        };

        con_err("unsupported color format " << format);
        return -1;
    }

    std::wostream &operator<<(std::wostream &stream, const iColorFormat &colorFormat)
    {
        const static std::wstring text[] = {
            L"Undefined",
            L"RGB",
            L"RGBA",
            L"BGR",
            L"BGRA"};

        stream << text[static_cast<int>(colorFormat)];

        return stream;
    }
}