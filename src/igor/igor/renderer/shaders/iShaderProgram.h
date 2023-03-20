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

#ifndef __IGOR_SHADER_PROGRAM__
#define __IGOR_SHADER_PROGRAM__

#include <igor/renderer/utils/iRendererDefines.h>

#include <iaux/math/iaMatrix.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <vector>
#include <memory>

namespace igor
{

    class iShaderProgram;

    /*! pointer definition of shader program
     */
    typedef std::shared_ptr<iShaderProgram> iShaderProgramPtr;

    /*! shader source struct
    */
    struct IGOR_API iShaderSource
    {
        /*! type of shader
        */
        iShaderObjectType _type;

        /*! shader source
        */
        iaString _source;

        /*! shader filename (optional)
        */
        iaString _filename;
    };

    /*! glsl shader program wrapper
     */
    class IGOR_API iShaderProgram
    {
    public:
        /*! \returns a newly created shader program
         */
        static iShaderProgramPtr create();

        /*! initializes shader program
         */
        iShaderProgram();

        /*! destroyes shader program
         */
        virtual ~iShaderProgram();

        /*! add shader object from file

        \param filename file name to shader definition
        \param type the type of shader object.

        If type is undefined we use the file ending to determine the type
        */
        void addShader(const iaString &filename, iShaderObjectType type);

        /*! \returns shader source files

        it does not return sources added via addSource
        */
        const std::vector<iShaderSource> &getShaderSources() const;

        /*! add shader from buffer

        \param source pointer to buffer with shader definition (zero terminated)
        \param type the type of shader object
        \returns true if sucessful
        */
        bool addSource(const char *source, iShaderObjectType type, const iaString &sourceName = "");

        /*! compiles shader programm with shader objects
         */
        void compile();

        /*! \returns true: if ready to use
         */
        bool isValid();

        /*! activates shader program
         */
        void bind();

        /*! deactivates shader program
         */
        void unbind();

        /*! sets integer on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setInt(const iaString &uniform, int value);

        /*! sets float on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat(const iaString &uniform, float32 value);

        /*! sets 2 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat2(const iaString &uniform, const iaVector2f &value);

        /*! sets 3 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat3(const iaString &uniform, const iaVector3f &value);

        /*! sets 4 dimensional vector on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setFloat4(const iaString &uniform, const iaVector4f &value);

        /*! sets 4x4 matrix on given uniform

        \param uniform the uniform name
        \param value the value to set
        */
        void setMatrix(const iaString &uniform, const iaMatrixf &value);

        /*! \returns true if shader has given uniform location

        \param uniform the given uniform location
        */
        bool hasUniformLocation(const iaString &uniform);

    private:
        /*! list of shader objects
         */
        std::vector<uint32> _shaderObjects;

        /*! shader source files
         */
        std::vector<iShaderSource> _sources;

        /*! id of glsl shader program
         */
        uint32 _shaderProgram;

        /*! true: if ready to use
         */
        bool _isValid = false;
    };

}; // namespace igor

#endif // __IGOR_SHADER_PROGRAM__
