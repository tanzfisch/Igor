
//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#include <igor/renderer/utils/iRendererDefines.h>

#include <vector>

namespace igor
{

    /*! buffer layout entry defnition
     */
    struct iBufferLayoutEntry
    {
        /*! does nothing
         */
        iBufferLayoutEntry() = default;

        /*! init with parameters

        \param type the shader data type to use
        \param normalized if true data will be normalized
        */
        iBufferLayoutEntry(iShaderDataType type, bool normalized = false);

        /*! \returns the amout of components within this layout entry
         */
        uint32_t getComponentCount() const;

        /*! shader data type of this entry
         */
        iShaderDataType _type;

        /*! size in bytes
         */
        uint32 _size;

        /*! offset in bytes
         */
        uint32 _offset;

        /*! if true data will be normalized when passed to the shader

        \todo is that true?
        */
        bool _normalized;
    };

    /*! the information on how to interpret the data in a given buffer

    This is based on a great idea from https://github.com/TheCherno/Hazel
     */
    class iBufferLayout
    {
    public:
        /*! does nothing
         */
        iBufferLayout() = default;

        /*! init layout with given layout elements

        \param elements given elements
        */
        iBufferLayout(const std::vector<iBufferLayoutEntry> &elements);

        /*! adds an element to the layout

        \param element the element to be added 
        */
        void addElement(const iBufferLayoutEntry &element);

        /*! \returns stride of layout configuration in byte
         */
        uint32 getStride() const;

        /*! \returns elements of layout
         */
        const std::vector<iBufferLayoutEntry> &getElements() const;

    private:
        /*! calculate offsets and stride
         */
        void calcOffsetsAndStride();

        /*! elements of the layout
         */
        std::vector<iBufferLayoutEntry> _elements;

        /*! the stride
         */
        uint32 _stride = 0;
    };

}

#endif // __IGOR_COMPONENT_INFO__