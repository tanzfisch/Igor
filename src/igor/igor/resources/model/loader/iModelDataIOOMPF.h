//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
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

#ifndef __IGOR_LOADERIGOR__
#define __IGOR_LOADERIGOR__

#include <igor/resources/model/loader/iModelDataIO.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/scene/traversal/iNodeVisitor.h>

#include <iaux/data/iaString.h>
using namespace iaux;

#include <map>
#include <unordered_map>

namespace OMPF
{
    class OMPF;
    class ompfTransformChunk;
    class ompfExternalReferenceChunk;
    class ompfMeshChunk;
    class ompfEmitterChunk;
    class ompfMaterialChunk;
    class ompfMaterialReferenceChunk;
    class ompfBaseChunk;
    class ompfParticleSystemChunk;
} // namespace OMPF

namespace igor
{

    class iNodeTransform;
    class iNodeModel;
    class iNodeMesh;
    class iNodeEmitter;
    class iNodeParticleSystem;

    /*! OMPF loader

    Uses ompf library to load ompf models
    */
    class iModelDataIOOMPF : public iModelDataIO, public iNodeVisitor
    {

    public:
        /*! does nothing
         */
        iModelDataIOOMPF();

        /*! does nothing
         */
        virtual ~iModelDataIOOMPF();

        /*! creates instance of this class

        \returns new instance
        */
        static iModelDataIO *createInstance();

        /*! loads or generates data based on given parameters

        \param parameters key values pairs determine what to load or generate
        */
        iNodePtr importData(const iParameters &parameters) override;

        /*! save node and underlying tree to filesystem

        \param parameters export parameters
        */
        void exportData(const iParameters &parameters) override;

    private:
        /*! the ompf loader it self
         */
        OMPF::OMPF *_ompf = nullptr;

        /*! chunk stack used while traversing the node tree
         */
        std::vector<OMPF::ompfBaseChunk *> _chunkStack;

        /*! mode to use while exporting model data
         */
        iSaveMode _saveMode;

        /*! file name in use while exporting

        needed to create relative paths to referenced resources
        */
        iaString _filename;

        /*! maps chunk material id to material id
         */
        std::unordered_map<uint32, iMaterialID> _materialMapping;

        /*! maps chunk id to node id
         */
        std::map<uint32, uint32> _chunkToNode;

        /*! maps node id to chunk id
         */
        std::map<uint32, uint32> _nodeToChunk;

        /*! map of reference materials in use
         */
        std::unordered_map<iMaterialID, OMPF::ompfMaterialReferenceChunk *> _materialReferencesInUse;

        /*! additional input parameter
        */
        iParameters _parameters;

        /*! handles start of traversal
         */
        virtual void preTraverse();

        /*! creates materials based on what ompf delivered
         */
        void createMaterials();

        /*! creates material

        \param materialReferenceChunk the data to create the material with
        */
        void createMaterial(OMPF::ompfMaterialReferenceChunk *materialReferenceChunk);

        /*! generates ompf chunks for every node

        handles node stack

        \param node current node
        */
        bool preOrderVisit(iNodePtr node, iNodePtr nextSibling) override;

        /*! handles node stack
         */
        void postOrderVisit(iNodePtr node) override;

        /*! handles end of traversal
         */
        void postTraverse() override;

        /*! creates a model data tree out of ompf chunks

        \param parent current model data node
        \param currentChunk current ompf chunk
        */
        iNodePtr createNodeTree(iNodePtr parent, OMPF::ompfBaseChunk *currentChunk);

        /*! creates mesh node from chunk

        \param chunk the chunk to create a mesh node from
        \returns mesh node
        */
        iNodePtr createMeshNode(OMPF::ompfBaseChunk *chunk);

        /*! creates particle system node from chunk

        \param chunk the chunk to create a particle system from
        \returns particle system node
        */
        iNodePtr createParticleSystem(OMPF::ompfBaseChunk *chunk);

        /*! creates transform chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfTransformChunk *createTransformChunk(iNodeTransform *node);

        /*! creates external reference chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfExternalReferenceChunk *createExternalReferenceChunk(iNodeModel *node);

        /*! creates mesh chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfMeshChunk *createMeshChunk(iNodeMesh *node);

        /*! creates emitter chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfEmitterChunk *createEmitterChunk(iNodeEmitter *node);

        /*! creates particle system chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfParticleSystemChunk *createParticleSystemChunk(iNodeParticleSystem *node);

        /*! creates material reference chunk from given igor material

        \param material the igor material
        */
        OMPF::ompfMaterialReferenceChunk *createMaterialReferenceChunk(iMaterialPtr material);

        /*! \returns material chunk id for given material id

        \param materialID the igor material id
        */
        uint32 getMaterialChunkID(const iMaterialID &materialID);

        /*! \returns igor material ID

        \param materialChunkID material chunk id
        */
        iMaterialID getMaterialID(uint32 materialChunkID);

        /*! \returns node id based on chunk id

        \param chunkID the chunk id
        */
        uint32 getNodeID(uint32 chunkID);

        /*! \returns chunk id based on node id

        \param nodeID the node id
        */
        uint32 getChunkID(uint32 nodeID);

        /*! post chunk creation step that links chunk IDs with each other

        \param currentChunk current chunk to process
        */
        void linkChunks(OMPF::ompfBaseChunk *currentChunk);

        /*! post node creation step that links node IDs with each other

        \param currentChunk current chunk to process
        */
        void linkNodes(OMPF::ompfBaseChunk *currentChunk);

        /*! clears internal material list
         */
        void clearMaterials();
    };

} // namespace igor

#endif
