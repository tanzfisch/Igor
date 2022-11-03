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

#ifndef __IGOR_SHADER__
#define __IGOR_SHADER__

#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/renderer/utils/iRendererUtils.h>

#include <vector>

namespace igor
{
    /*! glsl shader wrapper
    */
    class iShader
    {

    public:        

        /*! initializes shader program
        */
        iShader();

        /*! destroyes shader program
        */
        virtual ~iShader();        

        /*! add shader object from file

        \param filename file name to shader definition
        \param type the type of shader object
        */
        void addShader(iaString filename, iShaderObjectType_New type);

        /*! add shader from buffer

        \param source pointer to buffer with shader definition (zero terminated)
        \param type the type of shader object
        \returns true if sucessful
        */
        bool addShader(const char *source, iShaderObjectType_New type);

        /*! compiles shader programm with shader objects
        */
        void compile();

        /*! \returns true: if ready to use
        */
        bool isReady();

        /*! activates shader program
        */
        void bind();

        /*! deactivates shader program
        */
        void unbind();

    private:
        /*! list of shader objects
        */
        std::vector<uint32> _shaderObjects;

        /*! id of glsl shader program
        */
        uint32 _shaderProgram;

        /*! true: if ready to use
        */
        bool _ready = false;
    };

}; // namespace igor

#endif // __IGOR_SHADER__
