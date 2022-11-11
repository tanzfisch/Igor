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

#include <glad/glad.h>

#include <ostream>

namespace igor
{

/*! checks for openGL errors and prints them in the console
 */
#ifdef __IGOR_DEBUG__
#define GL_CHECK_ERROR()                                        \
    do                                                          \
    {                                                           \
        GLenum error = glGetError();                            \
        if (error != GL_NO_ERROR)                               \
        {                                                       \
            iaString errorCode;                                 \
            switch (error)                                      \
            {                                                   \
            case GL_INVALID_ENUM:                               \
                errorCode = "GL_INVALID_ENUM";                  \
                break;                                          \
            case GL_INVALID_VALUE:                              \
                errorCode = "GL_INVALID_VALUE";                 \
                break;                                          \
            case GL_INVALID_OPERATION:                          \
                errorCode = "GL_INVALID_OPERATION";             \
                break;                                          \
            case GL_INVALID_FRAMEBUFFER_OPERATION:              \
                errorCode = "GL_INVALID_FRAMEBUFFER_OPERATION"; \
                break;                                          \
            case GL_STACK_OVERFLOW:                             \
                errorCode = "GL_STACK_OVERFLOW";                \
                break;                                          \
            case GL_STACK_UNDERFLOW:                            \
                errorCode = "GL_STACK_UNDERFLOW";               \
                break;                                          \
            case GL_OUT_OF_MEMORY:                              \
                errorCode = "GL_OUT_OF_MEMORY";                 \
                break;                                          \
            default:                                            \
                errorCode = "UNKNOWN ERROR";                    \
                break;                                          \
            };                                                  \
            con_crit("GL_ERROR: " << errorCode);                \
        }                                                       \
        else                                                    \
        {                                                       \
            break;                                              \
        }                                                       \
    } while (0)

#else
#define GL_CHECK_ERROR()
#endif

/*! turns an index in to a pointer
 */
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

    /*! definition of invalid buffer id
     */
    static constexpr const uint32 INVALID_BUFFER_ID = 0;

    /*! maximum texture units supported
     */
    static constexpr const int32 MAX_TEXTURE_UNITS = 8;

    static constexpr const char *UNIFORM_SOLIDCOLOR = "igor_solidColor";

    static constexpr const char *UNIFORM_LIGHT_ORIENTATION = "igor_lightOrientation";
    static constexpr const char *UNIFORM_LIGHT_AMBIENT = "igor_lightAmbient";
    static constexpr const char *UNIFORM_LIGHT_DIFFUSE = "igor_lightDiffuse";
    static constexpr const char *UNIFORM_LIGHT_SPECULAR = "igor_lightSpecular";
    static constexpr const char *UNIFORM_EYE_POSITION = "igor_eyePosition";
    static constexpr const char *UNIFORM_MODEL_VIEW_PROJECTION = "igor_modelViewProjection";
    static constexpr const char *UNIFORM_MODEL = "igor_model";

    static constexpr const char *UNIFORM_MATERIAL_AMBIENT = "igor_matAmbient";
    static constexpr const char *UNIFORM_MATERIAL_DIFFUSE = "igor_matDiffuse";
    static constexpr const char *UNIFORM_MATERIAL_SPECULAR = "igor_matSpecular";
    static constexpr const char *UNIFORM_MATERIAL_SHININESS = "igor_matShininess";
    static constexpr const char *UNIFORM_MATERIAL_EMISSIVE = "igor_matEmissive";
    static constexpr const char *UNIFORM_MATERIAL_ALPHA = "igor_matAlpha";

    static constexpr const char *SAMPLER_TEXTURE = "igor_matTexture";

    /*! shader object types
     */
    enum class iShaderObjectType_New
    {
        Undefined,
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

    class IGOR_API iRendererUtils
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