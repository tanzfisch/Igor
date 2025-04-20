//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef __OMPF_DEFINES__
#define __OMPF_DEFINES__

#include <iaux/iaDefines.h>
using namespace iaux;

#ifdef IGOR_MSCOMPILER
	#ifdef __OMPF_BUILDING_DLL__
		#define OMPF_API __declspec(dllexport)
		#define OMPF_API_Template __declspec(dllexport)
	#else
		#define OMPF_API __declspec(dllimport)
		#define OMPF_API_Template
	#endif
#endif

#ifdef IGOR_GCC
	#define OMPF_API
	#define OMPF_API_Template
#endif

namespace OMPF
{

	/*! ompf default configuration
	*/
	struct OMPFDefaultConfiguration
	{
	public:
		/*! default type id size in bytes
		*/
		static const uint32 TypeIDSize = 1;

		/*! default chunk id size in bytes
		*/
		static const uint32 ChunkIDSize = 4;

		/*! default chunk size size in bytes.

		thats the size in bytes of the chunk size field
		*/
		static const uint32 ChunkSizeSize = 4;

		/*! default custom data size size in bytes.

		thats the size in bytes of the custom data size field

		\todo only 2 possible right now. see implementation of iaMemBlock
		*/
		static const uint32 CustomDataSizeSize = 2;

		/*! default string size size in bytes.

		thats the size in bytes of the string size field

		\todo only 2 possible right now.see implementation of iaString
		*/
		static const uint32 StringSizeSize = 2;

		/*! definition of invalid chunk id
		*/
		static const uint32 INVALID_CHUNK_ID = 0;
	};

	/*! ompf chunk types
	*/
	enum class OMPFChunkType
	{
		Invalid = 0x00,
		Header = 0x01,
		Group = 0x02,
		Transform = 0x03,
		External = 0x04,
		Mesh = 0x06,
		Emitter = 0x08,
		ParticleSystem = 0x09,
        MaterialReference = 0x0a
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

} // namespace OMPF

#endif