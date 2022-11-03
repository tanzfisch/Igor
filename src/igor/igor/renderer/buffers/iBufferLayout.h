
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

    struct iBufferLayoutEntry
    {
        iBufferLayoutEntry() = default;
        iBufferLayoutEntry(iShaderDataType type, bool normalized = false);

        /*! \returns the amout of components within this layout entry
         */
        uint32_t getComponentCount() const;

        /*! shader data type of this entry
         */
        iShaderDataType _type;

        uint32 _size;
        uint32 _offset;
        bool _normalized;
    };

    /*! the information on how to interpret the data in a given buffer

    This is based on a great idea from https://github.com/TheCherno/Hazel
     */
    class iBufferLayout
    {
    public:
        iBufferLayout() = default;

        iBufferLayout(std::vector<iBufferLayoutEntry> elements);

        uint32 getStride() const;
        const std::vector<iBufferLayoutEntry> &getElements() const;

    private:
        void calcOffsetsAndStride();

    private:
        std::vector<iBufferLayoutEntry> _elements;
        uint32 _stride = 0;
    };

}

#endif // __IGOR_COMPONENT_INFO__