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

#ifndef __IGOR_RENDERER_UTILS__
#define __IGOR_RENDERER_UTILS__

#include <igor/iDefines.h>

#define GL_GLEXT_PROTOTYPES
#include <GLee.h>

#include <ostream>

namespace igor
{

// needs to be a mecro so we can retrive the line number of the actual problem
#ifdef __IGOR_DEBUG__
#define GL_CHECK_ERROR()                                                 \
    do                                                                   \
    {                                                                    \
        GLenum error = glGetError();                                     \
        if (error != GL_NO_ERROR)                                        \
        {                                                                \
            iaString errorCode;                                          \
            switch (error)                                               \
            {                                                            \
            case GL_INVALID_ENUM:                                        \
                errorCode = "GL_INVALID_ENUM";                           \
                break;                                                   \
            case GL_INVALID_VALUE:                                       \
                errorCode = "GL_INVALID_VALUE";                          \
                break;                                                   \
            case GL_INVALID_OPERATION:                                   \
                errorCode = "GL_INVALID_OPERATION";                      \
                break;                                                   \
            case GL_STACK_OVERFLOW:                                      \
                errorCode = "GL_STACK_OVERFLOW";                         \
                break;                                                   \
            case GL_STACK_UNDERFLOW:                                     \
                errorCode = "GL_STACK_UNDERFLOW";                        \
                break;                                                   \
            case GL_OUT_OF_MEMORY:                                       \
                errorCode = "GL_OUT_OF_MEMORY";                          \
                break;                                                   \
            default:                                                     \
                errorCode = "UNKNOWN ERROR";                             \
                break;                                                   \
            };                                                           \
            con_assert(error != GL_NO_ERROR, "GL_ERROR: " << errorCode); \
        }                                                                \
        else                                                             \
        {                                                                \
            break;                                                       \
        }                                                                \
    } while (0)
#else
#define GL_CHECK_ERROR() 1
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

/*! definition of invalid buffer id
 */
#define INVALID_BUFFER_ID 0

    /*! shader object types
     */
    enum class iShaderObjectType_New
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
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iShaderObjectType_New &type);  

    class iRendererUtils
    {
    public:
        /*! \returns size of given shader data type in bytes

        \param type the given shader data type
        */
        static uint32 getSize(iShaderDataType type);

        /*! \returns component count of given shader data type

        \param type the given shader data type
        */
        static uint32 getComponentCount(iShaderDataType type);

        /*! \returns openGL shader object type for given igor type

        \param type the igor shader object type
        */
        static uint32 getOGLShaderType(iShaderObjectType_New type);
    };

}

#endif // __IGOR_RENDERER_UTILS__