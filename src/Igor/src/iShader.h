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
// (c) Copyright 2014-2015 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __SHADER__
#define __SHADER__

#include <iaMatrix.h>
#include <iaVector2.h>
#include <iaVector3.h>
#include <iaColor3.h>
#include <iaColor4.h>
#include <iRenderer.h>

#include <iaString.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

	struct iShaderSource
	{
		iaString _filename;
		iShaderObjectType _type;
	};

    /*! handles glsl shader programs and objects
    
    nice tutorial http://wiki.delphigl.com/index.php/Tutorial_glsl

    \todo cleanup
    */
	class iShader
	{

        friend class iRenderer;

    public:

        static const int32 INVALID_SHADER_ID = -1;

        /*! load shader object from file

        \param filename file name to shader definition
        \param type the type of shader object
        */
        void loadFile(iaString filename, iShaderObjectType type);

        /*! loads shader from buffer

        \param source pointer to buffer with shader definition
        \param type the type of shader object
        */
        bool loadSource(const char* source, iShaderObjectType type);

        const vector<iShaderSource>& getShaderSources() const;

        /*! compiles shader programm with shader objects
        */
        void compile();

        /*! \returns true: if ready to use
        */
        bool isReady();

        /*! activates shader program
        */
        void enable();

        /*! deactivates shader program
        */
        void disable();

        /*! creates shader program
        */
        iShader();

        /*! destroyes shader program
        */
        virtual ~iShader();

	private:

        /*! list of shader objects
        */
        vector<int32> _shaderObjects;

        /*! id of glsl shader program
        */
        int32 _shaderProgram = iShader::INVALID_SHADER_ID;

        /*! true: if ready to use
        */
        bool _ready = false;

		/*! contains the names of the shadcers used

		mostly filenames
		*/
		vector<iShaderSource> _shaderSources;

	};

};

#endif
