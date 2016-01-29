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

#ifndef __IGOR_STB_IMAGE_EXT__
#define __IGOR_STB_IMAGE_EXT__

#pragma warning( push )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4456 )
#pragma warning( disable : 4457 )

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*! wide char version of stbi__fopen

\param filename file name to open
\param mode read and or write mode
*/
static FILE *stbi__wfopen(wchar_t const *filename, wchar_t const *mode)
{
    FILE *f;
#if defined(_MSC_VER) && _MSC_VER >= 1400
    if (0 != _wfopen_s(&f, filename, mode))
    {
        f = 0;
    }
#else
    f = _wfopen(filename, mode);    
#endif
    return f;
}

/*! wide char version of stbi_load

\param filename file name to open
\param[out] x width of picture in pixels
\param[out] y height of picture in pixels
\param[out] comp component count per pixel in bytes
\param req_comp todo
\todo docu
*/
STBIDEF stbi_uc *stbi_load(wchar_t const *filename, int *x, int *y, int *comp, int req_comp)
{
    FILE *f = stbi__wfopen(filename, L"rb");
    unsigned char *result;
    if (!f)
    {
        return stbi__errpuc("can't fopen", "Unable to open file");
    }

    result = stbi_load_from_file(f, x, y, comp, req_comp);
    fclose(f);

    return result;
}

#pragma warning( pop ) 

#endif

