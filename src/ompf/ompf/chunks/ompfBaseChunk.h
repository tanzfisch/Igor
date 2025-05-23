//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
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

#ifndef __BASECHUNK__
#define __BASECHUNK__

#include <ompf/ompfDefines.h>

#include <iaux/data/iaSerializable.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <iostream>
#include <vector>
#include <fstream>
#include <ostream>

namespace OMPF
{

    class ompfSettings;

    /*! ompf base chunk. all chunks have to derive from this class
    */
    class OMPF_API ompfBaseChunk
    {

    public:
        /*! ctor 
        */
        ompfBaseChunk(OMPFChunkType typeID);

        /*! dtor does nothing
        */
        virtual ~ompfBaseChunk() = default;

        /*! writes current this cunk to specified stream

        \param stream destination stream
        \param settings the settings used to write the ompf file
        \returns true if there was no error
        */
        virtual bool write(std::ofstream &stream, const ompfSettings &settings);

        /*! reads a chunk from stream

        \param stream source stream
        \param settings the settings used to read the stream
        \returns true if there was no error
        */
        virtual bool read(std::ifstream &stream, ompfSettings &settings);

        /*! insert chunk in chunk

        \param chunk the child chunk to insert
        */
        void insertChunk(ompfBaseChunk *chunk);

        /*! removes chunk from chunk

        \param chunk the child chunk to remove
        */
        void removeChunk(ompfBaseChunk *chunk);

        /*! \returns true if chunk has a parent chunk
        */
        bool hasParent() const;

        /*! \returns parent chunk id. returns INVALID_CHUNK_ID if there is no parent
        */
        uint32 getParentID() const;

        /*! sets parent id

        \param parentID the parents ID
        */
        void setParentID(const uint32 parentID);

        /*! \returns true if chunk has children
        */
        bool hasChildren() const;

        /*! \returns list of children
        */
        const std::vector<ompfBaseChunk *> &getChildren() const;

        /*! sets chunk id

        \param chunkID
        */
        void setID(const uint32 chunkID);

        /*! \returns chunk id
        */
        uint32 getID() const;

        /*! \returns chunk size in bytes

        size can differ for different settings

        \param settings the settings to take in to account
        */
        virtual uint32 getSize(const ompfSettings &settings);

        /*! \returns chunk type id
        */
        OMPFChunkType getType() const;

        /*! sets name of chunk

        \param name the name of the chunk
        */
        void setName(const iaString &name);

        /*! \returns name of chunk
        */
        iaString getName() const;

    private:
        /*! chunk type id
        */
        OMPFChunkType _type = OMPFChunkType::Invalid;

        /*! chunk size in bytes
        */
        uint32 _chunkSize = 0;

        /*! chunk id
        */
        uint32 _ID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        /*! parent chunk id
        */
        uint32 _parentID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        /*! chunk's name
        */
        iaString _name;

        /*! children of chunk
        */
        std::vector<ompfBaseChunk *> _children;
    };

    /*! stream operator for chunk type
    \param stream the destination
    \param chunkType the chunk type to stream
    \returns the resulting stream
    */
    OMPF_API std::wostream &operator<<(std::wostream &stream, const OMPFChunkType chunkType);

    /*! stream operator for chunk type
    \param stream the destination
    \param pathType the path type to stream
    \returns the resulting stream
    */
    OMPF_API std::wostream &operator<<(std::wostream &stream, const OMPFPathType pathType);

} // namespace OMPF

#endif