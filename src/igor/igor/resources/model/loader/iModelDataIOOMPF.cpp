// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/resources/model/loader/iModelDataIOOMPF.h>

#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/resources/mesh/iMesh.h>
#include <igor/resources/mesh/iMeshBuilder.h>

#include <igor/renderer/iRenderStateSet.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeEmitter.h>
#include <igor/scene/nodes/iNodeParticleSystem.h>
#include <igor/resources/iResourceManager.h>

#include <iaux/data/iaConvert.h>
#include <iaux/system/iaDirectory.h>
#include <iaux/math/iaRandom.h>
using namespace iaux;

#include <ompf/ompf.h>

#include <algorithm>

namespace igor
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

    void iModelDataIOOMPF::linkNodes(OMPF::ompfBaseChunk *currentChunk)
    {
        switch (currentChunk->getType())
        {
        case OMPF::OMPFChunkType::ParticleSystem:
        {
            OMPF::ompfParticleSystemChunk *particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk *>(currentChunk);
            uint32 particleSystemNodeID = getNodeID(particleSystemChunk->getID());
            uint32 emitterNodeID = getNodeID(particleSystemChunk->getEmitterChunkID());
            iNodeParticleSystem *particleSystem = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(particleSystemNodeID));
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

    iNodePtr iModelDataIOOMPF::createNodeTree(iNodePtr parent, OMPF::ompfBaseChunk *currentChunk)
    {
        iNodePtr result = nullptr;

        switch (currentChunk->getType())
        {
        case OMPF::OMPFChunkType::Transform:
        {
            OMPF::ompfTransformChunk *transformChunk = static_cast<OMPF::ompfTransformChunk *>(currentChunk);
            iNodeTransform *transformNode = iNodeManager::getInstance().createNode<iNodeTransform>();

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
            OMPF::ompfExternalReferenceChunk *externalRefChunk = static_cast<OMPF::ompfExternalReferenceChunk *>(currentChunk);
            iNodeModel *modelNode = iNodeManager::getInstance().createNode<iNodeModel>();
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
            OMPF::ompfEmitterChunk *emitterChunk = static_cast<OMPF::ompfEmitterChunk *>(currentChunk);
            iNodeEmitter *emitterNode = iNodeManager::getInstance().createNode<iNodeEmitter>();
            emitterNode->setSize(emitterChunk->getSize());
            emitterNode->setEmitterType(static_cast<iEmitterType>(emitterChunk->getType()));
            result = emitterNode;
            break;
        }

        case OMPF::OMPFChunkType::ParticleSystem:
            result = createParticleSystem(currentChunk);
            break;

        case OMPF::OMPFChunkType::ResourceSearchPath:
            // TODO OMPF::ompfResourceSearchPathChunk* resourceSearchPathChunk = static_cast<OMPF::ompfResourceSearchPathChunk*>(currentChunk);
            break;

        case OMPF::OMPFChunkType::Mesh:
        {
            result = createMeshNode(currentChunk);
            break;
        }

        case OMPF::OMPFChunkType::Header:
        case OMPF::OMPFChunkType::Invalid:
            con_err("unexpected chunk type with id 0x" << std::hex << static_cast<uint64>(currentChunk->getType()) << std::dec);
            return result;

        default:
            con_err("unknown chunk type with id 0x" << std::hex << static_cast<uint64>(currentChunk->getType()) << std::dec);
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

    static void calculateBoundingBox(const float32 *vertexData, uint32 vertexSize, uint32 vertexCount, iaVector3d &minPos, iaVector3d &maxPos)
    {
        uint32 stride = vertexSize - 3;

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

    iNodePtr iModelDataIOOMPF::createMeshNode(OMPF::ompfBaseChunk *chunk)
    {
        OMPF::ompfMeshChunk *meshChunk = static_cast<OMPF::ompfMeshChunk *>(chunk);

        con_assert(meshChunk->getVertexDataSize() >= 3, "invalid data");
        con_assert(meshChunk->getIndexDataSize() >= 3, "invalid data");

        const bool keepMesh = _parameters.getParameter<bool>(IGOR_RESOURCE_PARAM_KEEP_MESH, false);

        iNodeMesh *meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();

        iMeshPtr mesh = iMesh::create();

        // set texture properties
        uint32 textureCount = meshChunk->getTextureCount();
        for (uint32 i = 0; i < textureCount; ++i)
        {
            meshNode->getMaterial()->setTexture(iResourceManager::getInstance().requestResource<iTexture>(meshChunk->getTexture(i)), i);
            mesh->setTexture(i, true);
        }

        iBufferLayout layout;
        layout.addElement({iShaderDataType::Float3});
        if (meshChunk->getNormalsPerVertex() ? true : false)
        {
            layout.addElement({iShaderDataType::Float3});
        }
        if (meshChunk->getColorsPerVertex() ? true : false)
        {
            layout.addElement({iShaderDataType::Float4});
        }
        for (int i = 0; i < meshChunk->getTexCoordPerVertex(); ++i)
        {
            layout.addElement({iShaderDataType::Float2});
        }

        mesh->setData(meshChunk->getIndexData(), meshChunk->getIndexDataSize(), meshChunk->getVertexData(), meshChunk->getVertexDataSize(), layout, keepMesh);

        mesh->setVertexCount(meshChunk->getVertexCount());
        mesh->setIndexCount(meshChunk->getIndexCount());
        mesh->setTrianglesCount(meshChunk->getIndexCount() / 3);

        mesh->setHasNormals(meshChunk->getNormalsPerVertex() ? true : false);
        mesh->setHasColors(meshChunk->getColorsPerVertex() ? true : false);
        mesh->setTextureCoordinatesCount(meshChunk->getTexCoordPerVertex());

        // calculate bounding
        iaVector3d minPos;
        iaVector3d maxPos;

        calculateBoundingBox(reinterpret_cast<const float32 *>(meshChunk->getVertexData()), layout.getStride() / 4, meshChunk->getVertexCount(), minPos, maxPos);

        iAABoxd bbox;
        bbox.setMinMax(minPos, maxPos);
        mesh->setBoundingBox(bbox);

        // push mesh to mesh node
        meshNode->setMesh(mesh);

        iMaterialID materialID = getMaterialID(meshChunk->getMaterialChunkID());
        meshNode->setMaterial(iResourceManager::getInstance().getResource<iMaterial>(materialID));

        return meshNode;
    }

    iNodePtr iModelDataIOOMPF::createParticleSystem(OMPF::ompfBaseChunk *chunk)
    {
        OMPF::ompfParticleSystemChunk *particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk *>(chunk);
        iNodeParticleSystem *particleSystemNode = iNodeManager::getInstance().createNode<iNodeParticleSystem>();
        particleSystemNode->setMaxParticleCount(particleSystemChunk->getMaxParticleCount());
        particleSystemNode->setLoop(particleSystemChunk->isLooped());

        iaKeyFrameGraphColor4f colorGradient;
        particleSystemChunk->getColorGradient(colorGradient);
        particleSystemNode->setColorGradient(colorGradient);

        iaKeyFrameGraphf emissionGradient;
        particleSystemChunk->getEmissionGradient(emissionGradient);
        particleSystemNode->setEmissionGradient(emissionGradient);

        particleSystemNode->setVortexTorque(particleSystemChunk->getVortexTorqueMin(), particleSystemChunk->getVortexTorqueMax());
        particleSystemNode->setVortexRange(particleSystemChunk->getVortexRangeMin(), particleSystemChunk->getVortexRangeMax());
        particleSystemNode->setVortexCheckRange(particleSystemChunk->getVortexCheckRange());
        particleSystemNode->setTextureTiling(particleSystemChunk->getTextureColumns(), particleSystemChunk->getTextureRows());

        iaKeyFrameGraphVector2f startSizeGradient;
        particleSystemChunk->getStartSizeGradient(startSizeGradient);
        particleSystemNode->setStartSizeGradient(startSizeGradient);

        iaKeyFrameGraphf sizeScaleGradient;
        particleSystemChunk->getSizeScaleGradient(sizeScaleGradient);
        particleSystemNode->setSizeScaleGradient(sizeScaleGradient);

        iaKeyFrameGraphVector2f startVisibleTimeGradient;
        particleSystemChunk->getStartAgeGradient(startVisibleTimeGradient);
        particleSystemNode->setStartAgeGradient(startVisibleTimeGradient);

        iaKeyFrameGraphVector2f orientationRateGradient;
        particleSystemChunk->getStartOrientationRateGradient(orientationRateGradient);
        particleSystemNode->setStartOrientationRateGradient(orientationRateGradient);

        iaKeyFrameGraphVector2f orientationGradient;
        particleSystemChunk->getStartOrientationGradient(orientationGradient);
        particleSystemNode->setStartOrientationGradient(orientationGradient);

        iaKeyFrameGraphVector2f liftGradient;
        particleSystemChunk->getStartLiftGradient(liftGradient);
        particleSystemNode->setStartLiftGradient(liftGradient);

        iaKeyFrameGraphVector2f velocityGradient;
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

        iMaterialID materialID = getMaterialID(particleSystemChunk->getMaterialChunkID());
        particleSystemNode->setMaterial(iResourceManager::getInstance().getResource<iMaterial>(materialID));

        return particleSystemNode;
    }

    iNodePtr iModelDataIOOMPF::importData(const iParameters &parameters)
    {
        _parameters = parameters;

        const iaString filename = _parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
        _ompf->loadFile(filename);

        if (_ompf->getRoot()->getChildren().size() == 0)
        {
            return nullptr;
        }

        createMaterials();
        iNodePtr result = createNodeTree(nullptr, _ompf->getRoot()->getChildren()[0]);
        linkNodes(_ompf->getRoot()->getChildren()[0]);

        return result;
    }

    void iModelDataIOOMPF::createMaterials()
    {
        for (const auto materialReferenceChunk : _ompf->getMaterialReferenceChunks())
        {
            createMaterial(materialReferenceChunk);
        }
    }

    void iModelDataIOOMPF::createMaterial(OMPF::ompfMaterialReferenceChunk *materialReferenceChunk)
    {
        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_CACHE_MODE, iResourceCacheMode::Cache}});

        if (iaUUID::isUUID(materialReferenceChunk->getReference()))
        {
            iaUUID uuid(materialReferenceChunk->getReference());
            param.setParameter(IGOR_RESOURCE_PARAM_ID, uuid);
        }
        else
        {
            param.setParameter(IGOR_RESOURCE_PARAM_SOURCE, materialReferenceChunk->getReference());
        }

        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);
        if (material == nullptr)
        {
            return;
        }

        _materialMapping[materialReferenceChunk->getID()] = material->getID();
    }

    void iModelDataIOOMPF::exportData(const iParameters &parameters)
    {
        iNodePtr node = parameters.getParameter<iNodePtr>(IGOR_RESOURCE_PARAM_NODE, nullptr);
        const iaString filename = parameters.getParameter<iaString>(IGOR_RESOURCE_PARAM_SOURCE, "");
        const iSaveMode saveMode = parameters.getParameter<iSaveMode>(IGOR_RESOURCE_PARAM_EXPORT_MODE, iSaveMode::KeepExternals);

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

    void iModelDataIOOMPF::linkChunks(OMPF::ompfBaseChunk *currentChunk)
    {
        switch (currentChunk->getType())
        {
        case OMPF::OMPFChunkType::ParticleSystem:
        {
            OMPF::ompfParticleSystemChunk *particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk *>(currentChunk);
            uint32 chunkID = particleSystemChunk->getID();
            uint32 nodeID = getNodeID(chunkID);
            iNodeParticleSystem *node = static_cast<iNodeParticleSystem *>(iNodeManager::getInstance().getNode(nodeID));
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
        _chunkStack.push_back((OMPF::ompfBaseChunk *)_ompf->getRoot());
    }

    void iModelDataIOOMPF::postTraverse()
    {
        _chunkStack.pop_back();
        con_assert(_chunkStack.size() == 0, "stack should be empty");
    }

    bool iModelDataIOOMPF::preOrderVisit(iNodePtr node, iNodePtr nextSibling)
    {
        OMPF::ompfBaseChunk *nextChunk = nullptr;
        bool callChildren = true;

        switch (node->getType())
        {
        case iNodeType::iNode:
            nextChunk = static_cast<OMPF::ompfBaseChunk *>(_ompf->createGroupChunk());
            break;

        case iNodeType::iNodeModel:
            if (_saveMode == iSaveMode::KeepExternals)
            {
                nextChunk = static_cast<OMPF::ompfBaseChunk *>(createExternalReferenceChunk(static_cast<iNodeModel *>(node)));
                callChildren = false;
            }
            break;

        case iNodeType::iNodeMesh:
            nextChunk = static_cast<OMPF::ompfBaseChunk *>(createMeshChunk(static_cast<iNodeMesh *>(node)));
            break;

        case iNodeType::iNodeTransform:
            nextChunk = static_cast<OMPF::ompfBaseChunk *>(createTransformChunk(static_cast<iNodeTransform *>(node)));
            break;

        case iNodeType::iNodeEmitter:
            nextChunk = static_cast<OMPF::ompfBaseChunk *>(createEmitterChunk(static_cast<iNodeEmitter *>(node)));
            break;

        case iNodeType::iNodeParticleSystem:
            nextChunk = static_cast<OMPF::ompfBaseChunk *>(createParticleSystemChunk(static_cast<iNodeParticleSystem *>(node)));
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

    OMPF::ompfExternalReferenceChunk *iModelDataIOOMPF::createExternalReferenceChunk(iNodeModel *node)
    {
        OMPF::ompfExternalReferenceChunk *result = _ompf->createExternalReferenceChunk();
        result->setFilename(iResourceManager::getInstance().getRelativePath(node->getModelName()));
        return result;
    }

    iModelDataIO *iModelDataIOOMPF::createInstance()
    {
        iModelDataIOOMPF *result = new iModelDataIOOMPF();
        return static_cast<iModelDataIO *>(result);
    }

    OMPF::ompfEmitterChunk *iModelDataIOOMPF::createEmitterChunk(iNodeEmitter *node)
    {
        OMPF::ompfEmitterChunk *result = _ompf->createEmitterChunk();
        result->setSize(node->getSize());
        result->setType(static_cast<OMPF::OMPFEmitterType>(node->getEmitterType()));
        return result;
    }

    OMPF::ompfParticleSystemChunk *iModelDataIOOMPF::createParticleSystemChunk(iNodeParticleSystem *node)
    {
        OMPF::ompfParticleSystemChunk *result = _ompf->createParticleSystemChunk();
        result->setMaxParticleCount(node->getMaxParticleCount());
        result->setLoop(node->isLooped());

        iaKeyFrameGraphColor4f colorGradient;
        node->getColorGradient(colorGradient);
        result->setColorGradient(colorGradient);

        iaKeyFrameGraphf emissionGradient;
        node->getEmissionGradient(emissionGradient);
        result->setEmissionGradient(emissionGradient);

        result->setVortexTorque(node->getVortexTorqueMin(), node->getVortexTorqueMax());
        result->setVortexRange(node->getVortexRangeMin(), node->getVortexRangeMax());
        result->setVortexCheckRange(node->getVortexCheckRange());
        result->setTextureTiling(node->getTextureColumns(), node->getTextureRows());

        iaKeyFrameGraphVector2f startSizeGradient;
        node->getStartSizeGradient(startSizeGradient);
        result->setStartSizeGradient(startSizeGradient);

        iaKeyFrameGraphf sizeScaleGradient;
        node->getSizeScaleGradient(sizeScaleGradient);
        result->setSizeScaleGradient(sizeScaleGradient);

        iaKeyFrameGraphVector2f startVisibleTimeGradient;
        node->getStartAgeGradient(startVisibleTimeGradient);
        result->setStartAgeGradient(startVisibleTimeGradient);

        iaKeyFrameGraphVector2f orientationRateGradient;
        node->getStartOrientationRateGradient(orientationRateGradient);
        result->setStartOrientationRateGradient(orientationRateGradient);

        iaKeyFrameGraphVector2f orientationGradient;
        node->getStartOrientationGradient(orientationGradient);
        result->setStartOrientationGradient(orientationGradient);

        iaKeyFrameGraphVector2f liftGradient;
        node->getStartLiftGradient(liftGradient);
        result->setStartLiftGradient(liftGradient);

        iaKeyFrameGraphVector2f velocityGradient;
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

        uint32 materialChunkID = getMaterialChunkID(node->getMaterial()->getID());
        result->setMaterialChunkID(materialChunkID);

        return result;
    }

    OMPF::ompfMeshChunk *iModelDataIOOMPF::createMeshChunk(iNodeMesh *node)
    {
        con_assert(node != nullptr, "zero pointer");

        OMPF::ompfMeshChunk *result = _ompf->createMeshChunk();

        if (node != nullptr)
        {
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

            const iMeshPtr &mesh = node->getMesh();

            if (mesh != nullptr &&
                mesh->hasRawData())
            {
                result->setNormalsPerVertex(mesh->hasNormals() ? 1 : 0);
                result->setColorsPerVertex(mesh->hasColors() ? 1 : 0);
                result->setTexCoordPerVertex(mesh->getTextureCoordinatesCount());

                result->setVertexCount(mesh->getVertexCount());

                void *indexData;
                uint32 indexDataSize;
                void *vertexData;
                uint32 vertexDataSize;
                mesh->getRawData(indexData, indexDataSize, vertexData, vertexDataSize);

                result->setVertexData(reinterpret_cast<char *>(vertexData), vertexDataSize);

                result->setIndexCount(mesh->getIndexCount());
                result->setIndexData(reinterpret_cast<char *>(indexData), indexDataSize);

                for (const auto &pair : node->getMaterial()->getTextures())
                {
                    if (pair.second == nullptr)
                    {
                        continue;
                    }

                    result->setTexture(pair.second->getID().toString(), pair.first);
                }
            }
            else
            {
                con_err("mesh was not loaded with keep mesh param true");
            }

            uint32 materialChunkID = getMaterialChunkID(node->getMaterial()->getID());
            result->setMaterialChunkID(materialChunkID);
        }

        return result;
    }

    OMPF::ompfTransformChunk *iModelDataIOOMPF::createTransformChunk(iNodeTransform *node)
    {
        OMPF::ompfTransformChunk *result = _ompf->createTransformChunk();
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

    OMPF::ompfMaterialReferenceChunk *iModelDataIOOMPF::createMaterialReferenceChunk(iMaterialPtr material)
    {
        OMPF::ompfMaterialReferenceChunk *result = _ompf->createMaterialReferenceChunk();
        result->setReference(material->getID().toString());
        return result;
    }

    uint32 iModelDataIOOMPF::getMaterialChunkID(const iMaterialID &materialID)
    {
        iMaterialPtr material = iResourceManager::getInstance().getResource<iMaterial>(materialID);

        if (material == nullptr)
        {
            con_err("material id \"" << materialID << "\" not found");
            return 0;
        }

        auto iter = _materialReferencesInUse.find(materialID);
        if (iter == _materialReferencesInUse.end())
        {
            _materialReferencesInUse[materialID] = createMaterialReferenceChunk(material);
        }

        return _materialReferencesInUse[materialID]->getID();
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

    iMaterialID iModelDataIOOMPF::getMaterialID(uint32 materialChunkID)
    {
        iMaterialID result;

        if (materialChunkID != 0)
        {
            auto materiIter = _materialMapping.find(materialChunkID);
            if (materiIter != _materialMapping.end())
            {
                result = materiIter->second;
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
        _materialReferencesInUse.clear();
    }

} // namespace igor