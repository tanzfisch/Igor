//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __OMPF_SETTINGS__
#define __OMPF_SETTINGS__

#include <ompf/ompfVersion.h>

namespace OMPF
{

    /*! structure that contains all the settings defined for a specified ompf file in focus
    */
    class OMPF_API ompfSettings
    {

    public:
        /*! \returns type ID size
        */
        uint8 getTypeIDSize() const;

        /*! \returns chunk id size
        */
        uint8 getChunkIDSize() const;

        /*! \returns chunk size size
        */
        uint8 getChunkSizeSize() const;

        /*! \returns custom data size size
        */
        uint8 getCustomDataSizeSize() const;

        /*! \returns string size size
        */
        uint8 getStringSizeSize() const;

        /*! \returns major version of ompf
        */
        uint8 getMajorVersion() const;

        /*! \returns minor version of ompf
        */
        uint8 getMinorVersion() const;

        /*! \returns patch version of ompf
        */
        uint8 getPatchVersion() const;

        /*! \returns major minor and patch version encoded in a 32 bit integer
        */
        uint32 getVersion() const;

        /*! sets type id size

        \param typeIDSize the type id size
        \returns false if invalid/unsupported size
        */
        bool setTypeIDSize(uint8 typeIDSize);

        /*! sets chunk id size

        \param chunkIDSize the chunk id size
        \returns false if invalid/unsupported size
        */
        bool setChunkIDSize(uint8 chunkIDSize);

        /*! sets chunk size size

        \param chunkSizeSize the chunk size size
        \returns false if invalid/unsupported size
        */
        bool setChunkSizeSize(uint8 chunkSizeSize);

        /*! sets custom data size size

        \param size the custom data size size
        \returns false if invalid/unsupported size
        */
        bool setCustomDataSizeSize(uint8 size);

        /*! sets string size size

        \param size the string size size
        \returns false if invalid/unsupported size
        */
        bool setStringSizeSize(uint8 size);

    private:
        /*! current type id size
        */
        uint8 _typeIDSize = OMPFDefaultConfiguration::TypeIDSize;

        /*! current chunk id size
        */
        uint8 _chunkIDSize = OMPFDefaultConfiguration::ChunkIDSize;

        /*! current chunk size size
        */
        uint8 _chunkSizeSize = OMPFDefaultConfiguration::ChunkSizeSize;

        /*! current custom data size size
        */
        uint8 _customDataSizeSize = OMPFDefaultConfiguration::CustomDataSizeSize;

        /*! current string size size
        */
        uint8 _stringSizeSize = OMPFDefaultConfiguration::StringSizeSize;

        /*! major version of ompf
        */
        uint8 _majorVersion = OMPF_VERSION_MAJOR;

        /*! minor version of ompf
        */
        uint8 _minorVersion = OMPF_VERSION_MINOR;

        /*! patch version of ompf
        */
        uint8 _patchVersion = OMPF_VERSION_PATCH;
    };
} // namespace OMPF

#endif
