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

#ifndef __IGOR_RENDERER_UTILS__
#define __IGOR_RENDERER_UTILS__

#include <igor/renderer/utils/iRendererDefines.h>

#include <glad/glad.h>

namespace igor
{

/*! checks for openGL errors and prints them in the console
 */
#ifdef IGOR_DEBUG
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

#if defined(IGOR_DEBUG) && defined(GL_DEBUG_SEVERITY_HIGH)
    // print out openGL debug messages
    static void onOGLDebugOutput(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
    {
        return;
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            con_crit(id << " - " << message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            if(id == 7) // ignore line width deprecation error
            {
                return;
            }
            con_err(id << " - " << message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            con_warn(id << " - " << message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            con_debug(id << " - " << message);
            return;
        }

        con_crit("Unknown Error");
    }
#endif

    /*! igor specific shader properties
    */
    static constexpr const char *UNIFORM_SOLIDCOLOR = "igor_solidColor";
    static constexpr const char *UNIFORM_EYE_POSITION = "igor_eyePosition";

    static constexpr const char *UNIFORM_LIGHT_ORIENTATION = "igor_lightOrientation";
    static constexpr const char *UNIFORM_LIGHT_AMBIENT = "igor_lightAmbient";
    static constexpr const char *UNIFORM_LIGHT_DIFFUSE = "igor_lightDiffuse";
    static constexpr const char *UNIFORM_LIGHT_SPECULAR = "igor_lightSpecular";

    static constexpr const char *UNIFORM_MODEL_VIEW_PROJECTION = "igor_modelViewProjection";
    static constexpr const char *UNIFORM_VIEW_PROJECTION = "igor_viewProjection";
    static constexpr const char *UNIFORM_MODEL = "igor_model";
    static constexpr const char *UNIFORM_MODEL_VIEW = "igor_modelView";

    static constexpr const char *UNIFORM_MATERIAL_AMBIENT = "igor_matAmbient";
    static constexpr const char *UNIFORM_MATERIAL_DIFFUSE = "igor_matDiffuse";
    static constexpr const char *UNIFORM_MATERIAL_SPECULAR = "igor_matSpecular";
    static constexpr const char *UNIFORM_MATERIAL_SHININESS = "igor_matShininess";
    static constexpr const char *UNIFORM_MATERIAL_EMISSIVE = "igor_matEmissive";
    static constexpr const char *UNIFORM_MATERIAL_ALPHA = "igor_matAlpha";

    static constexpr const char *UNIFORM_CONFIG_TILING = "igor_configTiling";
    static constexpr const char *UNIFORM_CONFIG_VELOCITY_ORIENTED = "igor_configVelocityOriented";

    static constexpr const char *SAMPLER_TEXTURE = "igor_matTexture";
    static constexpr const char *SAMPLER_TEXTURE0 = "igor_matTexture0";
    static constexpr const char *SAMPLER_TEXTURE1 = "igor_matTexture1";
    static constexpr const char *SAMPLER_TEXTURE2 = "igor_matTexture2";
    static constexpr const char *SAMPLER_TEXTURE3 = "igor_matTexture3";
    static constexpr const char *SAMPLER_TEXTURE4 = "igor_matTexture4";
    static constexpr const char *SAMPLER_TEXTURE5 = "igor_matTexture5";
    static constexpr const char *SAMPLER_TEXTURE6 = "igor_matTexture6";
    static constexpr const char *SAMPLER_TEXTURE7 = "igor_matTexture7";

    /*! renderer utils shared across renderer, shader etc implementations
    */
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

        /*! \returns opengl type for given shader data type

        \param type the shader data type
        */
        static uint32 convertType(iShaderDataType type);

        /*! \returns openGL shader object type for given igor type

        \param type the igor shader object type
        */
        static uint32 convertType(iShaderObjectType type);

        /*! \returns ogl color format type for given format type

        \param format the igor color format type
        \param sized if true it returns the sized internal format
        */
        static uint32 convertType(iColorFormat format, bool sized = false);

        /*! \returns openGL render primitive type for given igor type

        \param type the igor render primitive type
        */
        static uint32 convertType(iRenderPrimitive type);        

        /*! \returns openGL stencil function type for given igor type

        \param type the igor stencil function type
        */
        static uint32 convertType(iStencilFunction type);

        /*! \returns openGL stencil operation type for given igor type

        \param type the igor stencil operation type
        */
        static uint32 convertType(iStencilOperation type);

        /*! \returns texture sampler name for given texture unit
        
        \param texUnit the given texture unit
        */
        static iaString getTextureSamplerName(uint32 texUnit);

    };

}

#endif // __IGOR_RENDERER_UTILS__