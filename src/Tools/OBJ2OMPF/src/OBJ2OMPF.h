//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __OBJ2OMPF__
#define __OBJ2OMPF__

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{
    class iNode;
}

using namespace Igor;

/*! obj to ompf converter
*/
class OBJ2OMPF
{

public:

    /*! converts obj (wavefront) file in to an ompf file

    \param src source model expected to be an wavefront file
    \param dst destination file expected to have ompf as file extension
    */
	void convert(int argc, char* argv[]);

private:

    void setMaterialRecursive(iNode* node, uint64 materialID);
    bool analyzeParam(int argc, char* argv[]);

    bool _joinVertexes = true;
    iaString _src;
    iaString _dst;

};

#endif
