//
//  ##### #   # ####  #####
//  #   # ## ## #   # #
//  #   # # # # ####  ####
//  #   # #   # #     #
//  ##### #   # #     #  3d model file format
//
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef OMPF_EXTERNAL_REFERENCE_CHUNK_H
#define OMPF_EXTERNAL_REFERENCE_CHUNK_H

#include <ompf/chunks/ompfBaseChunk.h>

namespace OMPF
{

    /*! chunk that contains an external reference to an other file
    */
    class OMPF_API ompfExternalReferenceChunk : public ompfBaseChunk
    {

    public:
        /*! does nothing
        */
        ompfExternalReferenceChunk();

        /*! does nothing
        */
        virtual ~ompfExternalReferenceChunk() = default;

        /*! sets external reference

        \param reference the filename, alias or id
        */
        void setReference(const iaString &reference);

        /*! \returns external reference
        */
        const iaString& getReference() const;

    private:
        /*! external reference
        */
        iaString _reference;

        /*! writes chunk to stream

        \param stream destination stream
        \param settings the settings how to write the chunk
        */
        bool write(std::ofstream &stream, const ompfSettings &settings) override;

        /*! reads chunk from stream

        \param stream source stream
        \param settings the settings how to read the chunk
        */
        bool read(std::ifstream &stream, ompfSettings &settings) override;

        /*! \returns size of chunk in bytes

        \param settings the settings the chunk's size depends on
        */
        uint32 getSize(const ompfSettings &settings) override;        
    };

} // namespace OMPF

#endif // OMPF_EXTERNAL_REFERENCE_CHUNK_H