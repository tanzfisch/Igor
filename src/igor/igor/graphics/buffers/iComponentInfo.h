
//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2022 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_COMPONENT_INFO__
#define __IGOR_COMPONENT_INFO__

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>

#include <vector>

namespace igor
{

    struct iComponentEntry
    {
        iaString _name;
        iShaderDataType _type;
        uint32 _size;
        uint32 _offset;
        bool _normalized;

        iComponentEntry() = default;

        iComponentEntry(iShaderDataType type, const iaString &name, bool normalized = false)
            : _name(name), _type(type), _offset(0), _normalized(normalized)
        {
            _size = getSize();
        }

        uint32 getSize() const
        {
            switch (_type)
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

        uint32_t getComponentCount() const
        {
            switch (_type)
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
    };

    /*! the information on how to interpret the data in a given buffer
    */
    class iComponentInfo
    {
    public:
        iComponentInfo() = default;

        iComponentInfo(std::vector<iComponentEntry> components);

        uint32 getStride() const;
        const std::vector<iComponentEntry> &getComponents() const;

    private:
        void calcOffsetsAndStride();

    private:
        std::vector<iComponentEntry> _components;
        uint32 _stride = 0;
    };

}

#endif // __IGOR_COMPONENT_INFO__