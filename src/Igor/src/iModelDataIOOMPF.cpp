// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iModelDataIOOMPF.h>

#include <iModel.h>
#include <iNodeTransform.h>
#include <iNodeModel.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iMeshBuilder.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iMaterialGroup.h>
#include <iRenderStateSet.h>
#include <iShader.h>
#include <iTargetMaterial.h>
#include <iNodeFactory.h>
#include <iNodeEmitter.h>
#include <iNodeParticleSystem.h>

#include <iaConvert.h>
#include <iaDirectory.h>
using namespace IgorAux;

#include <OMPF.h>
#include <ompfBaseChunk.h>
#include <ompfGroupChunk.h>
#include <ompfHeaderChunk.h>
#include <ompfResourceSearchPathChunk.h>
#include <ompfMeshChunk.h>
#include <ompfTransformChunk.h>
#include <ompfExternalReferenceChunk.h>
#include <ompfMaterialChunk.h>
#include <ompfEmitterChunk.h>
#include <ompfParticleSystemChunk.h>
#include <OMPF.h>

namespace Igor
{
    iModelDataIOOMPF::iModelDataIOOMPF()
    {
        _name = "OMPF!";
        _identifier = "ompf";

        _ompf = new OMPF::OMPF();
    }

    iModelDataIOOMPF::~iModelDataIOOMPF()
    {
        delete _ompf;
    }

    iNode* iModelDataIOOMPF::createTree(iNode* parent, OMPF::ompfBaseChunk* currentChunk)
    {
        iNode* result = nullptr;

        switch (currentChunk->getType())
        {
        case OMPFChunkType::Transform:
        {
            OMPF::ompfTransformChunk* transformChunk = static_cast<OMPF::ompfTransformChunk*>(currentChunk);
            iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeTransform));

            iaMatrixf matrix;
            transformChunk->getMatrix(matrix);
            transformNode->setMatrix(matrix);

            result = transformNode;
            break;
        }

        case OMPFChunkType::External:
        {
            OMPF::ompfExternalReferenceChunk* externalRefChunk = static_cast<OMPF::ompfExternalReferenceChunk*>(currentChunk);
            iNodeModel* modelNode = static_cast<iNodeModel*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeModel));
            modelNode->setModel(externalRefChunk->getFilename());
            result = modelNode;
            break;
        }

        case OMPFChunkType::Group:
        {
            result = iNodeFactory::getInstance().createNode(iNodeType::iNode);
            break;
        }

        case OMPFChunkType::Emitter:
        {
            OMPF::ompfEmitterChunk* emitterChunk = static_cast<OMPF::ompfEmitterChunk*>(currentChunk);
            iNodeEmitter* emitterNode = static_cast<iNodeEmitter*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeEmitter));
            emitterNode->setSize(emitterChunk->getSize());
            emitterNode->setType(static_cast<iEmitterType>(emitterChunk->getType()));
            result = emitterNode;
            break;
        }

        case OMPFChunkType::ParticleSystem:
        {
            OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(currentChunk);
            iNodeParticleSystem* particleSystemNode = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeParticleSystem));
            particleSystemNode->setMaxParticleCount(particleSystemChunk->getMaxParticleCount());
            particleSystemNode->setLoop(particleSystemChunk->getLoop());

            iaGradientColor4f colorGradient;
            particleSystemChunk->getColorGradient(colorGradient);
            particleSystemNode->setColorGradient(colorGradient);

            iaGradientf emissionGradient;
            particleSystemChunk->getEmissionGradient(emissionGradient);
            particleSystemNode->setEmissionGradient(emissionGradient);

            particleSystemNode->setVortexTorque(particleSystemChunk->getVortexTorqueMin(), particleSystemChunk->getVortexTorqueMax());
            particleSystemNode->setVortexRange(particleSystemChunk->getVortexRangeMin(), particleSystemChunk->getVortexRangeMax());
            particleSystemNode->setVortexCheckRange(particleSystemChunk->getVortexCheckRange());

            result = particleSystemNode;
            break;
        }

        case OMPFChunkType::ResourceSearchPath:
            // OMPF::ompfResourceSearchPathChunk* resourceSearchPathChunk = static_cast<OMPF::ompfResourceSearchPathChunk*>(currentChunk);
            // TODO
            break;

        case OMPFChunkType::Mesh:
        {
            // TODO we should use the iMeshBuilder here!
            OMPF::ompfMeshChunk* meshChunk = static_cast<OMPF::ompfMeshChunk*>(currentChunk);

            iMesh* mesh = new iMesh();
            iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));

            if (_parameter != nullptr)
            {
                meshNode->setKeepMesh(_parameter->_keepMesh);
            }

            meshNode->getTargetMaterial()->setAmbient(iaConvert::convert3f(meshChunk->getAmbient()));
            meshNode->getTargetMaterial()->setDiffuse(iaConvert::convert3f(meshChunk->getDiffuse()));
            meshNode->getTargetMaterial()->setSpecular(iaConvert::convert3f(meshChunk->getSpecular()));
            meshNode->getTargetMaterial()->setEmissive(iaConvert::convert3f(meshChunk->getEmissive()));
            meshNode->getTargetMaterial()->setShininess(meshChunk->getShininess());

            uint32 textureCount = meshChunk->getTextureCount();
            for (int i = 0; i < textureCount; ++i)
            {
                meshNode->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().requestFile(meshChunk->getTexture(i)), i);
                mesh->setTexture(i, true);
            }

            float32* vertexData = reinterpret_cast<float32*>(new char[meshChunk->getVertexDataSize()]);
            uint32* indexData = reinterpret_cast<uint32*>(new char[meshChunk->getIndexDataSize()]);

            memcpy(vertexData, meshChunk->getVertexData(), meshChunk->getVertexDataSize());
            memcpy(indexData, meshChunk->getIndexData(), meshChunk->getIndexDataSize());

            mesh->setVertexData(vertexData, meshChunk->getVertexDataSize());
            mesh->setIndexData(indexData, meshChunk->getIndexDataSize());
            mesh->setVertexSize(meshChunk->calcVertexSize());

            mesh->setVertexCount(meshChunk->getVertexCount());
            mesh->setIndexesCount(meshChunk->getIndexCount());
            mesh->setTrianglesCount(meshChunk->getIndexCount() / 3);

            mesh->setHasNormals(meshChunk->getNormalsPerVertex() ? true : false);
            mesh->setHasColors(meshChunk->getColorsPerVertex() ? true : false);

            iSpheref sphere;
            meshChunk->getBoundingSphere(sphere._center, sphere._radius);
            mesh->setBoundingSphere(sphere);
            meshNode->setMesh(shared_ptr<iMesh>(mesh));

            auto materiIter = _materialMapping.find(meshChunk->getMaterialChunkID());
            if (materiIter != _materialMapping.end())
            {
                meshNode->setMaterial(_materialMapping[meshChunk->getMaterialChunkID()]);
            }
            else
            {
                con_err("material chunk with ID " << meshChunk->getMaterialChunkID() << " not found");
            }

            result = meshNode;
            break;
        }

        case OMPFChunkType::Header:
        case OMPFChunkType::Invalid:
            con_err("unexpected chunk type with id 0x" << hex << static_cast<uint64>(currentChunk->getType()));
            return result;

        default:
            con_err("unknown chunk type with id 0x" << hex << static_cast<uint64>(currentChunk->getType()));
            return result;
        }

        if (result != nullptr)
        {
            result->setName(currentChunk->getName());

            if (parent != nullptr)
            {
                parent->insertNode(result);
            }

            auto iter = currentChunk->getChildren().begin();
            while (iter != currentChunk->getChildren().end())
            {
                createTree(result, (*iter));
                iter++;
            }
        }

        return result;
    }
    iNode* iModelDataIOOMPF::importData(const iaString& filename, iModelDataInputParameter* parameter)
    {
        _parameter = parameter;

        _ompf->loadFile(filename);

        if (_ompf->getRoot()->getChildren().size() == 0)
        {
            return nullptr;
        }

        createMaterials();

        iNode* result = createTree(nullptr, _ompf->getRoot()->getChildren()[0]);

        if (result != nullptr)
        {
            con_info("loaded OMPF", "\"" << filename << "\"");
        }

        return result;
    }

    void iModelDataIOOMPF::createMaterials()
    {
        auto materials = _ompf->getMaterialChunks();
        auto matIter = materials.begin();
        while (matIter != materials.end())
        {
            createMaterial(*matIter);
            matIter++;
        }
    }

    void iModelDataIOOMPF::createMaterial(OMPF::ompfMaterialChunk* materialChunk)
    {
        con_assert(materialChunk != nullptr, "zero pointer");

        if (materialChunk != nullptr)
        {
            int32 materialID = iMaterialResourceFactory::getInstance().createMaterial(materialChunk->getMaterialName());
            iMaterial* material = iMaterialResourceFactory::getInstance().getMaterial(materialID);
            iMaterialGroup* materialGroup = iMaterialResourceFactory::getInstance().getMaterialGroup(materialID);

            con_assert(material != nullptr, "zero pointer");
            con_assert(materialGroup != nullptr, "zero pointer");

            _materialMapping[materialChunk->getID()] = materialID;
            materialGroup->setOrder(materialChunk->getOrder());

            uint32 shaderObjectCount = materialChunk->getShaderObjectCount();
            for (int i = 0; i < shaderObjectCount; ++i)
            {
                material->addShaderSource(materialChunk->getShaderFilename(i), static_cast<iShaderObjectType>(materialChunk->getShaderType(i)));
            }

            material->compileShader();

            for (int i = 0; i < 19; ++i) // TODO magic number
            {
                OMPF::OMPFRenderStateValue value = materialChunk->getRenderStateValue(static_cast<OMPF::OMPFRenderState>(i));
                material->getRenderStateSet().setRenderState(static_cast<iRenderState>(i), static_cast<iRenderStateValue>(value));
            }
        }
    }

    void iModelDataIOOMPF::exportData(const iaString& filename, iNode* node, iSaveMode saveMode)
    {
        con_assert(node != nullptr, "zero pointer");
        con_assert(!filename.isEmpty(), "empty string");

        _saveMode = saveMode;
        _filename = filename;

        traverseTree(node);

        _ompf->saveFile(filename);
    }

    void iModelDataIOOMPF::preTraverse()
    {
        con_assert(_chunkStack.size() == 0, "stack should be empty");
        _chunkStack.push_back((OMPF::ompfBaseChunk*)_ompf->getRoot());
    }

    void iModelDataIOOMPF::postTraverse()
    {
        _chunkStack.pop_back();
        con_assert(_chunkStack.size() == 0, "stack should be empty");
    }

    bool iModelDataIOOMPF::preOrderVisit(iNode* node)
    {
        OMPF::ompfBaseChunk* nextChunk = nullptr;
        bool callChildren = true;

        switch (node->getType())
        {
        case iNodeType::iNode:
            nextChunk = static_cast<OMPF::ompfBaseChunk*>(_ompf->createGroupChunk());
            break;

        case iNodeType::iNodeModel:
            if (_saveMode == iSaveMode::KeepExternals)
            {
                nextChunk = static_cast<OMPF::ompfBaseChunk*>(createExternalReferenceChunk(static_cast<iNodeModel*>(node)));
                callChildren = false;
            }
            break;

        case iNodeType::iNodeMesh:
            nextChunk = static_cast<OMPF::ompfBaseChunk*>(createMeshChunk(static_cast<iNodeMesh*>(node)));
            break;

        case iNodeType::iNodeTransform:
            nextChunk = static_cast<OMPF::ompfBaseChunk*>(createTransformChunk(static_cast<iNodeTransform*>(node)));
            break;

        case iNodeType::iNodeEmitter:
            nextChunk = static_cast<OMPF::ompfBaseChunk*>(createEmitterChunk(static_cast<iNodeEmitter*>(node)));
            break;

        case iNodeType::iNodeParticleSystem:
            nextChunk = static_cast<OMPF::ompfBaseChunk*>(createParticleSystemChunk(static_cast<iNodeParticleSystem*>(node)));
            break;

        case iNodeType::iNodeRender:
        case iNodeType::iNodeSkyBox:
        case iNodeType::iSkyLightNode:
        case iNodeType::iNodeCamera:
        case iNodeType::iCelestialNode:
        case iNodeType::iNodeLight:
        case iNodeType::Undefined:
        default:
            con_warn("can't handle this type of node");
            callChildren = false;
            break;
        }

        if (nullptr != nextChunk)
        {
            nextChunk->setName(node->getName());
            _chunkStack.back()->insertChunk(nextChunk);

            _chunkStack.push_back(nextChunk);
        }

        return callChildren;
    }

    void iModelDataIOOMPF::postOrderVisit(iNode* node)
    {
        if (node->getType() == iNodeType::iNodeModel)
        {
            if (_saveMode != iSaveMode::KeepExternals)
            {
                return;
            }
        }

        _chunkStack.pop_back();
    }

    OMPF::ompfExternalReferenceChunk* iModelDataIOOMPF::createExternalReferenceChunk(iNodeModel *node)
    {
        OMPF::ompfExternalReferenceChunk* result = _ompf->createExternalReferenceChunk();
        result->setFilename(node->getModelName());
        return result;
    }

    iModelDataIO* iModelDataIOOMPF::createInstance()
    {
        iModelDataIOOMPF* result = new iModelDataIOOMPF();
        return static_cast<iModelDataIO*>(result);
    }

    OMPF::ompfEmitterChunk* iModelDataIOOMPF::createEmitterChunk(iNodeEmitter *node)
    {
        OMPF::ompfEmitterChunk* result = _ompf->createEmitterChunk();
        result->setSize(node->getSize());
        result->setType(static_cast<OMPF::OMPFEmitterType>(node->getType()));
        return result;
    }

    OMPF::ompfParticleSystemChunk* iModelDataIOOMPF::createParticleSystemChunk(iNodeParticleSystem *node)
    {
        OMPF::ompfParticleSystemChunk* result = _ompf->createParticleSystemChunk();
        result->setMaxParticleCount(node->getMaxParticleCount());
        result->setLoop(node->getLoop());

        iaGradientColor4f colorGradient;
        node->getColorGradient(colorGradient);
        result->setColorGradient(colorGradient);

        iaGradientf emissionGradient;
        node->getEmissionGradient(emissionGradient);
        result->setEmissionGradient(emissionGradient);

        result->setVortexTorque(node->getVortexTorqueMin(), node->getVortexTorqueMax());
        result->setVortexRange(node->getVortexRangeMin(), node->getVortexRangeMax());
        result->setVortexCheckRange(node->getVortexCheckRange());

        return result;
    }

    OMPF::ompfMeshChunk* iModelDataIOOMPF::createMeshChunk(iNodeMesh *node)
    {
		con_assert(node != nullptr, "zero pointer");
		
		OMPF::ompfMeshChunk* result = _ompf->createMeshChunk();

		if (node != nullptr)
		{
			con_assert(node->getMesh() != nullptr, "zero pointer");

			result->setAmbient(iaConvert::convert3c(node->getAmbient()));
			result->setDiffuse(iaConvert::convert3c(node->getDiffuse()));
			result->setSpecular(iaConvert::convert3c(node->getSpecular()));
			result->setEmissive(iaConvert::convert3c(node->getEmissive()));
			result->setShininess(node->getShininess());

			if (node->getMesh() != nullptr)
			{
				result->setNormalsPerVertex(node->getMesh()->hasNormals() ? 1 : 0);
				result->setColorsPerVertex(node->getMesh()->hasColors() ? 1 : 0);
				result->setTexCoordPerVertex(node->getMesh()->getTextureUnitCount());

				result->setBoundingSphere(node->getBoundingSphere()._center, node->getBoundingSphere()._radius);

				result->setVertexCount(node->getMesh()->getVertexCount());
				result->setVertexData(reinterpret_cast<char*>(node->getMesh()->getVertexData()), node->getMesh()->getVertexDataSize());

				result->setIndexCount(node->getMesh()->getIndexesCount());
				result->setIndexData(reinterpret_cast<char*>(node->getMesh()->getIndexData()), node->getMesh()->getIndexDataSize());

				for (int i = 0; i < node->getMesh()->getTextureUnitCount(); ++i)
				{
					iaString relative = iaDirectory::getRelativePath(_filename, node->getTargetMaterial()->getTexture(i)->getFilename());
					result->setTexture(relative, i);
				}
			}

			uint32 materialChunkID = getMaterialChunkID(node->getMaterial());
			result->setMaterialChunkID(materialChunkID);
		}

        return result;
    }

    OMPF::ompfTransformChunk* iModelDataIOOMPF::createTransformChunk(iNodeTransform *node)
    {
        OMPF::ompfTransformChunk* result = _ompf->createTransformChunk();
        iaMatrixf matrix;
        node->getMatrix(matrix);
        result->setMatrix(matrix);
        return result;
    }

    OMPF::ompfMaterialChunk* iModelDataIOOMPF::createMaterialChunk(uint32 materialID)
    {
        OMPF::ompfMaterialChunk* result = _ompf->createMaterialChunk();

        iMaterialGroup* materialGroup = iMaterialResourceFactory::getInstance().getMaterialGroup(materialID);
        if (materialGroup != nullptr)
        {
            iMaterial* material = materialGroup->getMaterial();

            auto shaderSources = material->getShaderSources();
            auto iterShaderSources = shaderSources.begin();
            while (iterShaderSources != shaderSources.end())
            {
                result->addShader((*iterShaderSources)._filename, static_cast<OMPF::OMPFShaderType>((*iterShaderSources)._type));
                iterShaderSources++;
            }

            result->setMaterialName(material->getName());
            iRenderStateSet& renderStateSet = material->getRenderStateSet();

            for (int i = 0; i < static_cast<int>(iRenderState::RenderStateCount); ++i)
            {
                result->setRenderStateValue(static_cast<OMPF::OMPFRenderState>(i), static_cast<OMPF::OMPFRenderStateValue>(renderStateSet.getRenderStateValue(static_cast<iRenderState>(i))));
            }

            result->setOrder(materialGroup->getOrder());
        }
        else
        {
            con_err("material id " << materialID << " not found");
        }

        return result;
    }

    uint32 iModelDataIOOMPF::getMaterialChunkID(uint32 materialID)
    {
        if (materialID != 0)
        {
            auto iter = _materialsInUse.find(materialID);
            if (iter == _materialsInUse.end())
            {
                _materialsInUse[materialID] = createMaterialChunk(materialID);
            }

            return _materialsInUse[materialID]->getID();
        }

        return 0;
    }

    void iModelDataIOOMPF::clearMaterials()
    {
        _materialsInUse.clear();
    }

}