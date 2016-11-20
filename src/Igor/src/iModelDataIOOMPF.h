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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iLOADERIGOR__
#define __iLOADERIGOR__

#include <iaString.h>
using namespace IgorAux;

#include <iModelDataIO.h>
#include <iNodeVisitor.h>

#include <map>
using namespace std;

namespace OMPF
{
    class ompfTransformChunk;
    class ompfExternalReferenceChunk;
    class ompfMeshChunk;
    class ompfEmitterChunk;
    class ompfMaterialChunk;
    class OMPF;
    class ompfBaseChunk;
    class ompfParticleSystemChunk;
}

namespace Igor
{

	class iNodeTransform;
    class iNodeModel;
    class iNodeMesh;
    class iNodeEmitter;
    class iNodeParticleSystem;

    /*! OMPF loader
    
    Uses ompf library to load ompf models

    \todo should this be a node visitor?
    */
	class iModelDataIOOMPF : public iModelDataIO,  public iNodeVisitor
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
        static iModelDataIO* createInstance();

        /*! loades the data from filesystem and returns the result

        \param filename filename of file to load
        \return pointer to node tree
        */
        virtual iNode* importData(const iaString& filename, iModelDataInputParameter* parameter = nullptr);

        /*! saved node and underlying tree to filesystem

        \param filename destination filename
        \param node root node of tree to save
        */
        virtual void exportData(const iaString& filename, iNode* node, iSaveMode saveMode = iSaveMode::KeepExternals);

	private:

        /*! the ompf loader it self
        */
        OMPF::OMPF* _ompf = nullptr;

        /*! chunk stack used while traversing the node tree
        */
        vector<OMPF::ompfBaseChunk*> _chunkStack;

        /*! mode to use while exporting model data
        */
        iSaveMode _saveMode;

        /*! file name in use while exporting

        needed to create relative paths to referenced resources
        */
        iaString _filename;

        /*! maps chunk material id to materil id
        */
        map<uint32, uint32> _materialMapping;

        /*! maps chunk id to node id
        */
        map<uint32, uint32> _nodeMapping;

        /*! map of materials currently in use
        */
        map<uint32, OMPF::ompfMaterialChunk*> _materialsInUse;

        /*! additional input parameter

        do not delete. it does not belong to you
        */
        iModelDataInputParameter* _parameter = nullptr;

        /*! handles start of traversal
        */
        virtual void preTraverse();

        /*! creates materials based on what ompf delivered
        */
        void createMaterials();

        /*! creates one material

        \param materialChunk the data to create the material with
        */
        void createMaterial(OMPF::ompfMaterialChunk* materialChunk);

        /*! generates ompf chunks for every node

        handles node stack

        \param node current node
        */
        virtual bool preOrderVisit(iNode* node);

        /*! handles node stack
        */
        virtual void postOrderVisit(iNode* node);

        /*! handles end of traversal
        */
        virtual void postTraverse();

        /*! creates a model data tree out of ompf chunks

        \param parent current model data node
        \param currentChunk current ompf chunk
        */
        iNode* createNodeTree(iNode* parent, OMPF::ompfBaseChunk* currentChunk);

        void postCreation(OMPF::ompfBaseChunk* currentChunk);

        /*! creates transform chunk

        \param node the node the chunk is created from
        */
		OMPF::ompfTransformChunk* createTransformChunk(iNodeTransform *node);

        /*! creates external reference chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfExternalReferenceChunk* createExternalReferenceChunk(iNodeModel *node);

        /*! creates mesh chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfMeshChunk* createMeshChunk(iNodeMesh *node);

        /*! creates emitter chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfEmitterChunk* createEmitterChunk(iNodeEmitter *node);

        /*! creates particle system chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfParticleSystemChunk* createParticleSystemChunk(iNodeParticleSystem *node);

        /*! creates material chunk

        \param node the node the chunk is created from
        */
        OMPF::ompfMaterialChunk* createMaterialChunk(uint32 materialID);

        /*! \returns material chunk id for given material id

        \param materialID the material id
        */
        uint32 getMaterialChunkID(uint32 materialID);

        /*! \returns material id from material chunk id

        \param materialChunkID material chunk id
        */
        uint32 getMaterialID(uint32 materialChunkID);

        uint32 getNodeID(uint32 chunkID);

        /*! clears internal material list
        */
		void clearMaterials();

	};

}

#endif
