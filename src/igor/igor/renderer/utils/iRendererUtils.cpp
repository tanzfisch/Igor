// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
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

    uint32 iRendererUtils::convertType(iRenderPrimitive type)
    {
        switch (type)
        {
        case iRenderPrimitive::Points:
            return GL_POINTS;
        case iRenderPrimitive::Lines:
            return GL_LINES;
        case iRenderPrimitive::Triangles:
            return GL_TRIANGLES;
        }

        con_crit("Unknown render primitive type");
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

    uint32 iRendererUtils::convertType(iStencilFunction type)
    {
        switch (type)
        {
        case iStencilFunction::Never:
            return GL_NEVER;
        case iStencilFunction::Less:
            return GL_LESS;
        case iStencilFunction::LessOrEqual:
            return GL_LEQUAL;
        case iStencilFunction::Greater:
            return GL_GREATER;
        case iStencilFunction::GreaterOrEqual:
            return GL_GEQUAL;
        case iStencilFunction::Equal:
            return GL_EQUAL;
        case iStencilFunction::NotEqual:
            return GL_NOTEQUAL;
        case iStencilFunction::Always:
            return GL_ALWAYS;

        default:
            con_crit("unsupported stencil function type");
        };

        return 0;
    }

    uint32 iRendererUtils::convertType(iStencilOperation type)
    {
        switch (type)
        {
        case iStencilOperation::Zero:
            return GL_ZERO;
        case iStencilOperation::Keep:
            return GL_KEEP;
        case iStencilOperation::Replace:
            return GL_REPLACE;
        case iStencilOperation::Increment:
            return GL_INCR;
        case iStencilOperation::Decrement:
            return GL_DECR;
        case iStencilOperation::Invert:
            return GL_INVERT;

        default:
            con_crit("unsupported stencil operation type");
        }

        con_err("invalid value");
        return 0;
    }

    std::wostream &operator<<(std::wostream &stream, const iShaderObjectType &type)
    {
        const static std::wstring text[] = {
            L"Vertex",
            L"Fragment",
            L"Geometry"};

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
        };

        con_crit("unsupported color format " << format);
        return -1; // never reached
    }

    std::wostream &operator<<(std::wostream &stream, const iColorFormat &colorFormat)
    {
        const static std::wstring text[] = {
            L"Undefined",
            L"RGB",
            L"RGBA"};

        stream << text[static_cast<int>(colorFormat)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iRenderPrimitive &type)
    {
        const static std::wstring text[] = {
            L"Points",
            L"Lines",
            L"Triangles"};

        stream << text[static_cast<int>(type)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iRenderStateValue &value)
    {
        const static std::wstring text[] = {
            L"Off",
            L"On",
            L"One",
            L"Zero",
            L"DestinationColor",
            L"OneMinusDestinationColor",
            L"SourceAlpha",
            L"OneMinusSourceAlpha",
            L"DestinationAlpha",
            L"OneMinusDestinationAlpha",
            L"SourceColor",
            L"OneMinusSourceColor",
            L"Never",
            L"Less",
            L"Equal",
            L"LessOrEqual",
            L"Greater",
            L"NotEqual",
            L"GreaterOrEqual",
            L"Always",
            L"Front",
            L"Back",
            L"Keep",
            L"Replace",
            L"Increment",
            L"IncrementWrap",
            L"Decrement",
            L"DecrementWrap",
            L"Invert",
            L"Invalid",
            L"PositionOrientation",
            L"Position"};

        stream << text[static_cast<int>(value)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iRenderState &state)
    {
        const static std::wstring text[] = {
            L"DepthTest",
            L"DepthMask",
            L"Blend",
            L"CullFace",
            L"Wireframe",
            L"DepthFunc",
            L"CullFaceFunc",
            L"Instanced",
            L"InstancedFunc"};

        stream << text[static_cast<int>(state)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iStencilFunction &func)
    {
        const static std::wstring text[] = {
            L"Never",
            L"Less",
            L"Equal",
            L"LessOrEqual",
            L"Greater",
            L"NotEqual",
            L"GreaterOrEqual",
            L"Always"};

        stream << text[static_cast<int>(func)];

        return stream;
    }

    std::wostream &operator<<(std::wostream &stream, const iStencilOperation &operation)
    {
        const static std::wstring text[] = {
            L"Keep",
            L"Zero",
            L"Replace",
            L"Increment",
            L"Decrement",
            L"Invert"};

        stream << text[static_cast<int>(operation)];

        return stream;
    }
}