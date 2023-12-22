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
// (c) Copyright 2012-2023 by Martin Loga
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

#ifndef __IGOR_RENDERER_DEFINES__
#define __IGOR_RENDERER_DEFINES__

#include <igor/iDefines.h>

namespace igor
{

    /*! definition of invalid buffer id
     */
    static constexpr const uint32 INVALID_BUFFER_ID = 0;

    /*! maximum texture units supported
     */
    static constexpr const int32 MAX_TEXTURE_UNITS = 8;

    /*! render target id definition
     */
    typedef uint32 iRenderTargetID;

    /*! definition of render target types
     */
    enum class iRenderTargetType
    {
        ToTexture,
        ToRenderBuffer
    };

    /*! render primitives definition
     */
    enum class iRenderPrimitive
    {
        Points,
        Lines,
        Triangles
    };

    /*! prints given data type in the console

    \param stream the stream to log to
    \param type the data type to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iRenderPrimitive &type);

    /*! stencil function types
     */
    enum class iStencilFunction
    {
        Never,
        Less,
        Equal,
        LessOrEqual,
        Greater,
        NotEqual,
        GreaterOrEqual,
        Always
    };

    /*! prints given data type in the console

    \param stream the stream to log to
    \param type the data type to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iStencilFunction &type);    

    /*! stencil operation types
     */
    enum class iStencilOperation
    {
        Keep,
        Zero,
        Replace,
        Increment,
        Decrement,
        Invert
    };

    /*! prints given data type in the console

    \param stream the stream to log to
    \param type the data type to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iStencilOperation &type);     

    /*! render states
     */
    enum class iRenderState
    {
        DepthTest,
        DepthMask,
        Blend,
        CullFace,
        Wireframe,
        DepthFunc,
        CullFaceFunc,
        Instanced,
        InstancedFunc
    };

    const uint32 RENDER_STATE_COUNT = 9;

    /*! prints given render state in a stream

    \param stream the stream to log to
    \param state the render state to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iRenderState &state);

    /*! render state values

    Note: not everything maps to OpenGL
     */
    enum class iRenderStateValue
    {
        Off = 0,
        On = 1,
        One,
        Zero,
        DestinationColor,
        OneMinusDestinationColor,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        SourceColor,
        OneMinusSourceColor,
        Never,
        Less,
        Equal,
        LessOrEqual,
        Greater,
        NotEqual,
        GreaterOrEqual,
        Always,
        Front,
        Back,
        Keep,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert,
        Invalid,
        PositionOrientationInstancing
    };

    /*! prints given render state value in a stream

    \param stream the stream to log to
    \param value the render state value to print
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iRenderStateValue &value);

    /*! shader data type
     */
    enum class iShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Matrix3x3,
        Matrix4x4,
        Int,
        Int2,
        Int3,
        Int4,
        Boolean
    };

    /*! prints the shader data type in the console

    \param stream the stream to log to
    \param type the shader data type
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iShaderDataType &type);

    /*! shader object types
     */
    enum class iShaderObjectType
    {
        Vertex,
        Fragment,
        Geometry
    };

    /*! prints the shader object type in the console

    \param stream the stream to log to
    \param type the shader object type
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iShaderObjectType &type);

    /*! the default render target ID
     */
    static const iRenderTargetID DEFAULT_RENDER_TARGET = 0;

}

#endif // __IGOR_RENDERER_DEFINES__