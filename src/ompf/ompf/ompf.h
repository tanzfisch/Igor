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

#ifndef OMPF_H
#define OMPF_H

#include <ompf/ompfVersion.h>
#include <ompf/ompfSettings.h>

#include <ompf/chunks/ompfBaseChunk.h>
#include <ompf/chunks/ompfEmitterChunk.h>
#include <ompf/chunks/ompfExternalReferenceChunk.h>
#include <ompf/chunks/ompfGroupChunk.h>
#include <ompf/chunks/ompfHeaderChunk.h>
#include <ompf/chunks/ompfMaterialReferenceChunk.h>
#include <ompf/chunks/ompfMeshChunk.h>
#include <ompf/chunks/ompfParticleSystemChunk.h>
#include <ompf/chunks/ompfTransformChunk.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <map>
#include <fstream>

namespace OMPF
{

    /*! this is the OMPF file format reference implementation
    */
    class OMPF_API OMPF
    {

    public:
        /*! initializes member variables
        */
        OMPF();

        /*! clears all referenced data
        */
        ~OMPF();

        /*! reinitializes member variables
        */
        void reset();

        /*! export data in to file

        \param filename the destination file name
        */
        void saveFile(iaString filename);

        /*! load data from file

        \param filename the source file name
        */
        void loadFile(iaString filename);

        /*! \returns the root node of currently loaded data

        is never zero
        */
        ompfHeaderChunk *getRoot();

        /*! creates a group chunk

        \returns pointer to new group chunk
        */
        ompfGroupChunk *createGroupChunk();

        /*! creates a transform chunk

        \returns pointer to new transform chunk
        */
        ompfTransformChunk *createTransformChunk();

        /*! creates a material reference chunk

		\returns pointer to material chunk
		*/
        ompfMaterialReferenceChunk *createMaterialReferenceChunk();

        /*! creates a mesh chunk

        \returns pointer to new mesh chunk
        */
        ompfMeshChunk *createMeshChunk();

        /*! creates a emitter chunk

        \returns pointer to emitter chunk
        */
        ompfEmitterChunk *createEmitterChunk();

        /*! creates particle system chunk

        \returns pointer to particle system chunk
        */
        ompfParticleSystemChunk *createParticleSystemChunk();

        /*! creates a external reference chunk

        \returns pointer to new external reference chunk
        */
        ompfExternalReferenceChunk *createExternalReferenceChunk();

        /*! \returns list of material references
        */
        const std::vector<ompfMaterialReferenceChunk *> &getMaterialReferenceChunks() const;

        /*! \returns the directory of the file we want to load or save

        e.g. if the file path is c:\foo\bar.ompf then the return value will be c:\foo

        the return string can be empty if load or save is not in process yet
        */
        const iaString &getFileDirectory() const;

    private:
        /*! initializes member data
        */
        void init();

        /*! deinitializes member data
        */
        void deInit();

        /*! analyzes data to detect and evaluate chunks

        \param file stream handle
        */
        bool analyze(std::ifstream &file);

        /*! reads the next few bytes (depends on configuration) to detect the beginning of a new chunk

        \param file stream handle
        \param typeID suspected type Id (in out)
        \param chunkSize suspected chunk size (in out)
        */
        bool getNextChunk(std::ifstream &file, uint32 &typeID, uint32 &chunkSize);

        /*! write recursive to file

        only writes the nodes to file that are connected direct or indirect to root node

        \param currentChunk current chunk during traversation
        \param parentChunk parent of the current
        */
        void write(std::ofstream &outfile, ompfBaseChunk *currentChunk, ompfBaseChunk *parentChunk);

        /*! write all the registered materials to stream

        \param outfile the stream to write to
        */
        void writeMaterials(std::ofstream &outfile);

        /*! \returns chunk by given ID

        \param chunkID the chunk id given
        */
        ompfBaseChunk *getChunk(uint32 chunkID);

        /*! clears all data in OMPF
        */
        void clearChunks();

        /*! makes ompf start counting ids from the beginning again
        */
        void resetChunkID();

        /*! \returns next generated chunk id
        */
        uint32 getNextChunkID();

        /*! map with all the chunks
        */
        std::map<uint32, ompfBaseChunk *> _chunks;

        /*! list of material reference chunks
        */
        std::vector<ompfMaterialReferenceChunk *> _materialReferenceChunks;

        /*! internal counter for chunk IDs
        */
        uint32 _nextChunkID = OMPFDefaultConfiguration::INVALID_CHUNK_ID;

        /*! the root chunk
        */
        ompfHeaderChunk *_root = nullptr;

        /*! contains a set of ompf settings like size of IDs etc.
        */
        ompfSettings _settings;

        /*! the path to the file we are working with
        */
        iaString _filepath;
    };

} // namespace OMPF

#endif // OMPF_H 
