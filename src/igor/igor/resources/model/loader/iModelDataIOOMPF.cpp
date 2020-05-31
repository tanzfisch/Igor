// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/loader/iModelDataIOOMPF.h>

#include <igor/resources/model/iModel.h>
#include <igor/graphics/scene/nodes/iNodeTransform.h>
#include <igor/graphics/scene/nodes/iNodeModel.h>
#include <igor/graphics/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/mesh/iMeshBuilder.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/graphics/iRenderStateSet.h>
#include <igor/resources/material/iShader.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/graphics/scene/nodes/iNodeManager.h>
#include <igor/graphics/scene/nodes/iNodeEmitter.h>
#include <igor/graphics/scene/nodes/iNodeParticleSystem.h>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaDirectory.h>
using namespace IgorAux;

#include <ompf/ompf.h>

#include <algorithm>

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

	void iModelDataIOOMPF::linkNodes(OMPF::ompfBaseChunk* currentChunk)
	{
		switch (currentChunk->getType())
		{
		case OMPF::OMPFChunkType::ParticleSystem:
		{
			OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(currentChunk);
			uint32 particleSystemNodeID = getNodeID(particleSystemChunk->getID());
			uint32 emitterNodeID = getNodeID(particleSystemChunk->getEmitterChunkID());
			iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().getNode(particleSystemNodeID));
			if (particleSystem != nullptr)
			{
				particleSystem->setEmitter(emitterNodeID);
			}
		}
		break;
		}

		auto iter = currentChunk->getChildren().begin();
		while (iter != currentChunk->getChildren().end())
		{
			linkNodes((*iter));
			iter++;
		}
	}

	iNodePtr iModelDataIOOMPF::createNodeTree(iNodePtr parent, OMPF::ompfBaseChunk* currentChunk)
	{
		iNodePtr result = nullptr;

		switch (currentChunk->getType())
		{
		case OMPF::OMPFChunkType::Transform:
		{
			OMPF::ompfTransformChunk* transformChunk = static_cast<OMPF::ompfTransformChunk*>(currentChunk);
			iNodeTransform* transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();

			iaMatrixf matrix;
			transformChunk->getMatrix(matrix);

			iaMatrixd matrixD;
			for (int i = 0; i < 16; ++i)
			{
				matrixD[i] = matrix[i];
			}
			transformNode->setMatrix(matrixD);

			result = transformNode;
			break;
		}

		case OMPF::OMPFChunkType::External:
		{
			OMPF::ompfExternalReferenceChunk* externalRefChunk = static_cast<OMPF::ompfExternalReferenceChunk*>(currentChunk);
			iNodeModel* modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
			modelNode->setModel(externalRefChunk->getFilename());
			result = modelNode;
			break;
		}

		case OMPF::OMPFChunkType::Group:
		{
			result = iNodeManager::getInstance().createNode<iNode>();
			break;
		}

		case OMPF::OMPFChunkType::Emitter:
		{
			OMPF::ompfEmitterChunk* emitterChunk = static_cast<OMPF::ompfEmitterChunk*>(currentChunk);
			iNodeEmitter* emitterNode = iNodeManager::getInstance().createNode<iNodeEmitter>();
			emitterNode->setSize(emitterChunk->getSize());
			emitterNode->setEmitterType(static_cast<iEmitterType>(emitterChunk->getType()));
			result = emitterNode;
			break;
		}

		case OMPF::OMPFChunkType::ParticleSystem:
			result = createParticleSystem(currentChunk);
			break;

		case OMPF::OMPFChunkType::ResourceSearchPath:
			// OMPF::ompfResourceSearchPathChunk* resourceSearchPathChunk = static_cast<OMPF::ompfResourceSearchPathChunk*>(currentChunk);
			// TODO
			break;

		case OMPF::OMPFChunkType::Mesh:
		{
			result = createMeshNode(currentChunk);
			break;
		}

		case OMPF::OMPFChunkType::Header:
		case OMPF::OMPFChunkType::Invalid:
			con_err("unexpected chunk type with id 0x" << std::hex << static_cast<uint64>(currentChunk->getType()));
			return result;

		default:
			con_err("unknown chunk type with id 0x" << std::hex << static_cast<uint64>(currentChunk->getType()));
			return result;
		}

		if (result != nullptr)
		{
			_nodeToChunk[result->getID()] = currentChunk->getID();
			_chunkToNode[currentChunk->getID()] = result->getID();

			result->setName(currentChunk->getName());

			if (parent != nullptr)
			{
				parent->insertNode(result);
			}

			auto iter = currentChunk->getChildren().begin();
			while (iter != currentChunk->getChildren().end())
			{
				createNodeTree(result, (*iter));
				iter++;
			}
		}

		return result;
	}

	iNodePtr iModelDataIOOMPF::createMeshNode(OMPF::ompfBaseChunk * chunk)
	{
		OMPF::ompfMeshChunk* meshChunk = static_cast<OMPF::ompfMeshChunk*>(chunk);

		con_assert(meshChunk->getVertexDataSize() >= 3, "invalid data");
		con_assert(meshChunk->getIndexDataSize() >= 3, "invalid data");

		// create mesh and mesh node
		iMesh* mesh = new iMesh();
		iNodeMesh* meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();

		if (_parameter != nullptr)
		{
			meshNode->setKeepMesh(_parameter->_keepMesh);
		}

		// set material properties
		iaColor3f ambient;
		iaColor3f diffuse;
		iaColor3f specular;
		iaColor3f emissive;

		iaConvert::convert(meshChunk->getAmbient(), ambient);
		iaConvert::convert(meshChunk->getDiffuse(), diffuse);
		iaConvert::convert(meshChunk->getSpecular(), specular);
		iaConvert::convert(meshChunk->getEmissive(), emissive);

		meshNode->getTargetMaterial()->setAmbient(ambient);
		meshNode->getTargetMaterial()->setDiffuse(diffuse);
		meshNode->getTargetMaterial()->setSpecular(specular);
		meshNode->getTargetMaterial()->setEmissive(emissive);
		meshNode->getTargetMaterial()->setShininess(meshChunk->getShininess());

		// set texture properties
		uint32 textureCount = meshChunk->getTextureCount();
		for (uint32 i = 0; i < textureCount; ++i)
		{
			iaString texturePath = _ompf->getFileDirectory();
			texturePath += __IGOR_PATHSEPARATOR__;
			texturePath += meshChunk->getTexture(i);

			meshNode->getTargetMaterial()->setTexture(iTextureResourceFactory::getInstance().requestFile(texturePath), i);
			mesh->setTexture(i, true);
		}

		// prepare mesh data
		float32* vertexData = reinterpret_cast<float32*>(new char[meshChunk->getVertexDataSize()]);
		uint32* indexData = reinterpret_cast<uint32*>(new char[meshChunk->getIndexDataSize()]);

		memcpy(vertexData, meshChunk->getVertexData(), meshChunk->getVertexDataSize());
		memcpy(indexData, meshChunk->getIndexData(), meshChunk->getIndexDataSize());

		mesh->setVertexData(vertexData, meshChunk->getVertexDataSize());
		mesh->setIndexData(indexData, meshChunk->getIndexDataSize());
		uint32 vertexSize = meshChunk->getVertexSize();
		mesh->setVertexSize(vertexSize);

		mesh->setVertexCount(meshChunk->getVertexCount());
		mesh->setIndexesCount(meshChunk->getIndexCount());
		mesh->setTrianglesCount(meshChunk->getIndexCount() / 3);

		mesh->setHasNormals(meshChunk->getNormalsPerVertex() ? true : false);
		mesh->setHasColors(meshChunk->getColorsPerVertex() ? true : false);
		mesh->setTextureCoordinatesCount(meshChunk->getTexCoordPerVertex());

		// calculate boundings
		iaVector3d minPos;
		iaVector3d maxPos;

		calculateBoundingBox(vertexData, vertexSize / 4, meshChunk->getVertexCount(), minPos, maxPos);

		iAABoxd bbox;
		bbox.set(minPos, maxPos);
		mesh->setBoundingBox(bbox);

		iSphered sphere;
		sphere._center = bbox._center;
		sphere._radius = std::max(bbox._halfWidths._x, std::max(bbox._halfWidths._y, bbox._halfWidths._z));
		mesh->setBoundingSphere(sphere);

		// push mesh to mesh node
		meshNode->setMesh(iMeshPtr(mesh));

		uint32 materialID = getMaterialID(meshChunk->getMaterialChunkID());
		meshNode->setMaterial(materialID);

		return meshNode;
	}

	void iModelDataIOOMPF::calculateBoundingBox(float32 * vertexData, uint32 vertexSize, uint32 vertexCount, iaVector3d & minPos, iaVector3d & maxPos)
	{
		uint32 stride = vertexSize - 3;
		float32* vertexDataIter = vertexData;

		for (uint32 vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
		{
			uint32 offset = vertexIndex * vertexSize;

			if (vertexIndex == 0)
			{
				minPos.set(vertexData[offset + 0], vertexData[offset + 1], vertexData[offset + 2]);
				maxPos = minPos;
			}
			else
			{
				if (vertexData[offset + 0] < minPos._x)
				{
					minPos._x = vertexData[offset + 0];
				}

				if (vertexData[offset + 1] < minPos._y)
				{
					minPos._y = vertexData[offset + 1];
				}

				if (vertexData[offset + 2] < minPos._z)
				{
					minPos._z = vertexData[offset + 2];
				}

				if (vertexData[offset + 0] > maxPos._x)
				{
					maxPos._x = vertexData[offset + 0];
				}

				if (vertexData[offset + 1] > maxPos._y)
				{
					maxPos._y = vertexData[offset + 1];
				}

				if (vertexData[offset + 2] > maxPos._z)
				{
					maxPos._z = vertexData[offset + 2];
				}
			}
		}
	}

	iNodePtr iModelDataIOOMPF::createParticleSystem(OMPF::ompfBaseChunk * chunk)
	{
		OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(chunk);
		iNodeParticleSystem* particleSystemNode = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
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
		particleSystemNode->setFirstTextureTiling(particleSystemChunk->getFirstTextureColumns(), particleSystemChunk->getFirstTextureRows());

		iaGradientVector2f startSizeGradient;
		particleSystemChunk->getStartSizeGradient(startSizeGradient);
		particleSystemNode->setStartSizeGradient(startSizeGradient);

		iaGradientf sizeScaleGradient;
		particleSystemChunk->getSizeScaleGradient(sizeScaleGradient);
		particleSystemNode->setSizeScaleGradient(sizeScaleGradient);

		iaGradientVector2f startVisibleTimeGradient;
		particleSystemChunk->getStartVisibleTimeGradient(startVisibleTimeGradient);
		particleSystemNode->setStartVisibleTimeGradient(startVisibleTimeGradient);

		iaGradientVector2f orientationRateGradient;
		particleSystemChunk->getStartOrientationRateGradient(orientationRateGradient);
		particleSystemNode->setStartOrientationRateGradient(orientationRateGradient);

		iaGradientVector2f orientationGradient;
		particleSystemChunk->getStartOrientationGradient(orientationGradient);
		particleSystemNode->setStartOrientationGradient(orientationGradient);

		iaGradientVector2f liftGradient;
		particleSystemChunk->getStartLiftGradient(liftGradient);
		particleSystemNode->setStartLiftGradient(liftGradient);

		iaGradientVector2f velocityGradient;
		particleSystemChunk->getStartVelocityGradient(velocityGradient);
		particleSystemNode->setStartVelocityGradient(velocityGradient);

		particleSystemNode->setVortexToParticleRate(particleSystemChunk->getVortexToParticleRate());
		particleSystemNode->setVorticityConfinement(particleSystemChunk->getVorticityConfinement());

		particleSystemNode->setAirDrag(particleSystemChunk->getAirDrag());
		particleSystemNode->setPeriodTime(particleSystemChunk->getPeriodTime());
		particleSystemNode->setVelocityOriented(particleSystemChunk->getVelocityOriented());

		particleSystemNode->setTextureA(particleSystemChunk->getTextureA());
		particleSystemNode->setTextureB(particleSystemChunk->getTextureB());
		particleSystemNode->setTextureC(particleSystemChunk->getTextureC());

		uint32 materialID = getMaterialID(particleSystemChunk->getMaterialChunkID());
		particleSystemNode->setMaterial(materialID);

		return particleSystemNode;
	}

	iNodePtr iModelDataIOOMPF::importData(const iaString & filename, iModelDataInputParameter * parameter)
	{
		_parameter = parameter;

		_ompf->loadFile(filename);

		if (_ompf->getRoot()->getChildren().size() == 0)
		{
			return nullptr;
		}

		createMaterials();

		iNodePtr result = createNodeTree(nullptr, _ompf->getRoot()->getChildren()[0]);

		linkNodes(_ompf->getRoot()->getChildren()[0]);

		if (result != nullptr)
		{
			con_info("loaded ompf", "\"" << filename << "\"");
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

	void iModelDataIOOMPF::createMaterial(OMPF::ompfMaterialChunk * materialChunk)
	{
		con_assert(materialChunk != nullptr, "zero pointer");

		if (materialChunk != nullptr)
		{
			int32 materialID = iMaterialResourceFactory::getInstance().createMaterial(materialChunk->getMaterialName());
			iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(materialID);

			con_assert(material != nullptr, "zero pointer");

			if (material != nullptr)
			{
				_materialMapping[materialChunk->getID()] = materialID;
				material->setOrder(materialChunk->getOrder());

				uint32 shaderObjectCount = materialChunk->getShaderObjectCount();
				for (uint32 i = 0; i < shaderObjectCount; ++i)
				{
					material->addShaderSource(materialChunk->getShaderFilename(i), static_cast<iShaderObjectType>(materialChunk->getShaderType(i)));
				}

				material->compileShader();

				for (int i = 0; i < 19; ++i) // TODO magic number
				{
					OMPF::OMPFRenderStateValue value = materialChunk->getRenderState(static_cast<OMPF::OMPFRenderState>(i));
					material->setRenderState(static_cast<iRenderState>(i), static_cast<iRenderStateValue>(value));
				}
			}
			else
			{
				con_err("material not found");
			}
		}
		else
		{
			con_err("zero pointer");
		}
	}

	void iModelDataIOOMPF::exportData(const iaString & filename, iNodePtr node, iSaveMode saveMode)
	{
		con_assert(node != nullptr, "zero pointer");
		con_assert(!filename.isEmpty(), "empty string");

		if (node != nullptr &&
			!filename.isEmpty())
		{
			_saveMode = saveMode;
			_filename = filename;

			traverseTree(node);

			linkChunks(_ompf->getRoot()->getChildren()[0]);

			_ompf->saveFile(filename);
		}
		else
		{
			con_err("invalid parameter");
		}
	}

	void iModelDataIOOMPF::linkChunks(OMPF::ompfBaseChunk * currentChunk)
	{
		switch (currentChunk->getType())
		{
		case OMPF::OMPFChunkType::ParticleSystem:
		{
			OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(currentChunk);
			uint32 chunkID = particleSystemChunk->getID();
			uint32 nodeID = getNodeID(chunkID);
			iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeManager::getInstance().getNode(nodeID));
			if (node != nullptr)
			{
				uint32 emitterNodeID = node->getEmitter();
				if (emitterNodeID != iNode::INVALID_NODE_ID)
				{
					uint32 emitterChunkID = getChunkID(emitterNodeID);
					particleSystemChunk->setEmitterChunkID(emitterChunkID);
				}
			}
		}
		break;
		}

		auto iter = currentChunk->getChildren().begin();
		while (iter != currentChunk->getChildren().end())
		{
			linkChunks((*iter));
			iter++;
		}
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

	bool iModelDataIOOMPF::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
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
			_nodeToChunk[node->getID()] = nextChunk->getID();
			_chunkToNode[nextChunk->getID()] = node->getID();

			nextChunk->setName(node->getName());
			_chunkStack.back()->insertChunk(nextChunk);

			_chunkStack.push_back(nextChunk);
		}

		return callChildren;
	}

	void iModelDataIOOMPF::postOrderVisit(iNodePtr node)
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

	OMPF::ompfExternalReferenceChunk* iModelDataIOOMPF::createExternalReferenceChunk(iNodeModel * node)
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

	OMPF::ompfEmitterChunk* iModelDataIOOMPF::createEmitterChunk(iNodeEmitter * node)
	{
		OMPF::ompfEmitterChunk* result = _ompf->createEmitterChunk();
		result->setSize(node->getSize());
		result->setType(static_cast<OMPF::OMPFEmitterType>(node->getEmitterType()));
		return result;
	}

	OMPF::ompfParticleSystemChunk* iModelDataIOOMPF::createParticleSystemChunk(iNodeParticleSystem * node)
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
		result->setFirstTextureTiling(node->getFirstTextureColumns(), node->getFirstTextureRows());

		iaGradientVector2f startSizeGradient;
		node->getStartSizeGradient(startSizeGradient);
		result->setStartSizeGradient(startSizeGradient);

		iaGradientf sizeScaleGradient;
		node->getSizeScaleGradient(sizeScaleGradient);
		result->setSizeScaleGradient(sizeScaleGradient);

		iaGradientVector2f startVisibleTimeGradient;
		node->getStartVisibleTimeGradient(startVisibleTimeGradient);
		result->setStartVisibleTimeGradient(startVisibleTimeGradient);

		iaGradientVector2f orientationRateGradient;
		node->getStartOrientationRateGradient(orientationRateGradient);
		result->setStartOrientationRateGradient(orientationRateGradient);

		iaGradientVector2f orientationGradient;
		node->getStartOrientationGradient(orientationGradient);
		result->setStartOrientationGradient(orientationGradient);

		iaGradientVector2f liftGradient;
		node->getStartLiftGradient(liftGradient);
		result->setStartLiftGradient(liftGradient);

		iaGradientVector2f velocityGradient;
		node->getStartVelocityGradient(velocityGradient);
		result->setStartVelocityGradient(velocityGradient);

		result->setVortexToParticleRate(node->getVortexToParticleRate());
		result->setVorticityConfinement(node->getVorticityConfinement());

		result->setAirDrag(node->getAirDrag());
		result->setPeriodTime(node->getPeriodTime());
		result->setVelocityOriented(node->getVelocityOriented());

		result->setTextureA(node->getTextureA());
		result->setTextureB(node->getTextureB());
		result->setTextureC(node->getTextureC());

		uint32 materialChunkID = getMaterialChunkID(node->getMaterial());
		result->setMaterialChunkID(materialChunkID);

		return result;
	}

	OMPF::ompfMeshChunk* iModelDataIOOMPF::createMeshChunk(iNodeMesh * node)
	{
		con_assert(node != nullptr, "zero pointer");

		OMPF::ompfMeshChunk* result = _ompf->createMeshChunk();

		if (node != nullptr)
		{
			con_assert(node->getMesh() != nullptr, "zero pointer");

			iaColor3c ambient;
			iaColor3c diffuse;
			iaColor3c specular;
			iaColor3c emissive;

			iaConvert::convert(node->getAmbient(), ambient);
			iaConvert::convert(node->getDiffuse(), diffuse);
			iaConvert::convert(node->getSpecular(), specular);
			iaConvert::convert(node->getEmissive(), emissive);

			result->setAmbient(ambient);
			result->setDiffuse(diffuse);
			result->setSpecular(specular);
			result->setEmissive(emissive);
			result->setShininess(node->getShininess());

			if (node->getMesh() != nullptr)
			{
				result->setNormalsPerVertex(node->getMesh()->hasNormals() ? 1 : 0);
				result->setColorsPerVertex(node->getMesh()->hasColors() ? 1 : 0);
				result->setTexCoordPerVertex(node->getMesh()->getTextureCoordinatesCount());

				result->setVertexCount(node->getMesh()->getVertexCount());
				result->setVertexData(reinterpret_cast<char*>(node->getMesh()->getVertexData()), node->getMesh()->getVertexDataSize());

				result->setIndexCount(node->getMesh()->getIndexesCount());
				result->setIndexData(reinterpret_cast<char*>(node->getMesh()->getIndexData()), node->getMesh()->getIndexDataSize());

				for (uint32 i = 0; i < node->getTargetMaterial()->getTextureUnitCount(); ++i)
				{
					iTexturePtr texture = node->getTargetMaterial()->getTexture(i);
					if (texture != nullptr)
					{
						iaString relative = iaDirectory::getRelativePath(_filename, texture->getFilename());
						result->setTexture(relative, i);
					}
				}
			}
			else
			{
				con_err("mesh was not loaded with keep mesh param true");
			}

			uint32 materialChunkID = getMaterialChunkID(node->getMaterial());
			result->setMaterialChunkID(materialChunkID);
		}

		return result;
	}

	OMPF::ompfTransformChunk* iModelDataIOOMPF::createTransformChunk(iNodeTransform * node)
	{
		OMPF::ompfTransformChunk* result = _ompf->createTransformChunk();
		iaMatrixd matrixD;
		node->getMatrix(matrixD);

		iaMatrixf matrix;
		for (int i = 0; i < 16; ++i)
		{
			matrix[i] = matrixD[i];
		}
		result->setMatrix(matrix);
		return result;
	}

	OMPF::ompfMaterialChunk* iModelDataIOOMPF::createMaterialChunk(uint32 materialID)
	{
		OMPF::ompfMaterialChunk* result = _ompf->createMaterialChunk();

		iMaterialPtr material = iMaterialResourceFactory::getInstance().getMaterial(materialID);
		if (material != nullptr)
		{
			auto shaderSources = material->getShaderSources();
			for (auto shaderSource : shaderSources)
			{
				result->addShader(shaderSource._filename, static_cast<OMPF::OMPFShaderType>(shaderSource._type));
			}

			result->setMaterialName(material->getName());
			iRenderStateSet& renderStateSet = material->getRenderStateSet();

			for (int i = 0; i < static_cast<int>(iRenderState::RenderStateCount); ++i)
			{
				result->setRenderStateValue(static_cast<OMPF::OMPFRenderState>(i), static_cast<OMPF::OMPFRenderStateValue>(renderStateSet.getRenderState(static_cast<iRenderState>(i))));
			}

			result->setOrder(material->getOrder());
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

	uint32 iModelDataIOOMPF::getNodeID(uint32 chunkID)
	{
		uint32 result = iNode::INVALID_NODE_ID;
		if (chunkID != 0)
		{
			auto iter = _chunkToNode.find(chunkID);
			if (iter != _chunkToNode.end())
			{
				result = (*iter).second;
			}
			else
			{
				con_err("chunk with ID " << chunkID << " not found");
			}
		}

		return result;
	}

	uint32 iModelDataIOOMPF::getChunkID(uint32 nodeID)
	{
		uint32 result = 0;
		if (nodeID != 0)
		{
			auto iter = _nodeToChunk.find(nodeID);
			if (iter != _nodeToChunk.end())
			{
				result = (*iter).second;
			}
			else
			{
				con_err("node with ID " << nodeID << " not found");
			}
		}

		return result;
	}

	uint32 iModelDataIOOMPF::getMaterialID(uint32 materialChunkID)
	{
		uint32 result = 0;
		if (materialChunkID != 0)
		{
			auto materiIter = _materialMapping.find(materialChunkID);
			if (materiIter != _materialMapping.end())
			{
				result = (*materiIter).second;
			}
			else
			{
				con_err("material chunk with ID " << materialChunkID << " not found");
			}
		}

		return result;
	}

	void iModelDataIOOMPF::clearMaterials()
	{
		_materialsInUse.clear();
	}

}