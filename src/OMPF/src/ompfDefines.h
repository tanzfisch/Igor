//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
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

#ifndef __OMPF_DEFINES__
#define __OMPF_DEFINES__

#include <iaDefines.h>
using namespace IgorAux;

#ifdef __IGOR_WIN__
	#ifdef OMPF_BUILDING_DLL
		#define OMPF_API __declspec(dllexport)
		#define OMPF_API_Template __declspec(dllexport)
	#else
		#define OMPF_API __declspec(dllimport)
		#define OMPF_API_Template
	#endif
#endif

/*! ompf default configuration
*/
struct OMPFDefaultConfiguration
{
public:
    static const uint32 TypeIDSize = 1;
    static const uint32 ChunkIDSize = 4;
    static const uint32 ChunkSizeSize = 4;
    static const uint32 CustomDataSizeSize = 2; //! \todo only 2 possible right now. see implementation of iaMemBlock
    static const uint32 StringSizeSize = 2; //! \todo only 2 possible right now. see implementation of iaString
    static const uint32 InvalidChunkID = 0;
};

/*! ompf chunk types
*/
enum class OMPFChunkTypes
{
    Invalid = 0x00,
    Header = 0x01,
    Group = 0x02,
    Transform = 0x03,
    External = 0x04,
    ResourceSearchPath = 0x05,
    Mesh = 0x06,
    Material = 0x07
};

/*! ompf path types
*/
enum class OMPFPathType
{
    RelativeToModel = 0,
    RelativeToApplication = 1,
    RelativeToCurrentDirectory = 2,
    Absolute = 3,
    Undefined = 100
};


#endif