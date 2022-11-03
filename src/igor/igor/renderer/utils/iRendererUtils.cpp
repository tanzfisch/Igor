// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/renderer/utils/iRendererUtils.h>

namespace igor
{

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


}