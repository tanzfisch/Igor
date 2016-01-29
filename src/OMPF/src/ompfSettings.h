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

#ifndef __OMPF_SETTINGS__
#define __OMPF_SETTINGS__

#include <ompfVersion.h>

namespace OMPF
{
	class OMPF_API ompfSettings
	{

	public:

		uint8 getTypeIDSize() const;
		uint8 getChunkIDSize() const;
		uint8 getChunkSizeSize() const;
        uint8 getCustomDataSizeSize() const;
        uint8 getStringSizeSize() const;

		uint8 getMajorVersion() const;
		uint8 getMinorVersion() const;
		uint8 getPatchVersion() const;

        uint32 getVersion() const;

		bool setTypeIDSize(uint8 typeIDSize);
		bool setChunkIDSize(uint8 chunkIDSize);
		bool setChunkSizeSize(uint8 chunkSizeSize);

        bool setCustomDataSizeSize(uint8 size);
        bool setStringSizeSize(uint8 size);

	private:

		uint8 _typeIDSize = OMPFDefaultConfiguration::TypeIDSize;
		uint8 _chunkIDSize = OMPFDefaultConfiguration::ChunkIDSize;
		uint8 _chunkSizeSize = OMPFDefaultConfiguration::ChunkSizeSize;
        uint8 _customDataSizeSize = OMPFDefaultConfiguration::CustomDataSizeSize;
        uint8 _stringSizeSize = OMPFDefaultConfiguration::StringSizeSize;

		uint8 _majorVersion = OMPF_VERSION_MAJOR;
		uint8 _minorVersion = OMPF_VERSION_MINOR;
		uint8 _patchVersion = OMPF_VERSION_PATCH;

	};
}

#endif
