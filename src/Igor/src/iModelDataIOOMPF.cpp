// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
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

    void iModelDataIOOMPF::linkNodes(OMPF::ompfBaseChunk* currentChunk)
    {
        switch (currentChunk->getType())
        {
        case OMPFChunkType::ParticleSystem:
        {
            OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(currentChunk);
            uint32 particleSystemNodeID = getNodeID(particleSystemChunk->getID());
            uint32 emitterNodeID = getNodeID(particleSystemChunk->getEmitterChunkID());
            iNodeParticleSystem* particleSystem = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(particleSystemNodeID));
            if(particleSystem != nullptr)
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

    iNode* iModelDataIOOMPF::createNodeTree(iNode* parent, OMPF::ompfBaseChunk* currentChunk)
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

            iaMatrixd matrixD;
            for (int i = 0; i < 16; ++i)
            {
                matrixD[i] = matrix[i];
            }
            transformNode->setMatrix(matrixD);

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

            uint32 textureCount = meshChunk->getTextureCount();
            for (uint32 i = 0; i < textureCount; ++i)
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
            iSphered sphereD;
            sphereD._center._x = sphere._center._x;
            sphereD._center._y = sphere._center._y;
            sphereD._center._z = sphere._center._z;
            sphereD._radius = sphere._radius;
            mesh->setBoundingSphere(sphereD);

            iAABoxd bbox;
            bbox._center = sphereD._center;
            bbox._halfWidths._x = sphereD._radius;
            bbox._halfWidths._y = sphereD._radius;
            bbox._halfWidths._z = sphereD._radius;
            mesh->setBoundingBox(bbox);

            meshNode->setMesh(shared_ptr<iMesh>(mesh));

            uint32 materialID = getMaterialID(meshChunk->getMaterialChunkID());
            meshNode->setMaterial(materialID);

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

    iNode* iModelDataIOOMPF::importData(const iaString& filename, iModelDataInputParameter* parameter)
    {
        _parameter = parameter;

        _ompf->loadFile(filename);

        if (_ompf->getRoot()->getChildren().size() == 0)
        {
            return nullptr;
        }

        createMaterials();

        iNode* result = createNodeTree(nullptr, _ompf->getRoot()->getChildren()[0]);

        linkNodes(_ompf->getRoot()->getChildren()[0]);

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

            if (material != nullptr &&
                materialGroup != nullptr)
            {
                _materialMapping[materialChunk->getID()] = materialID;
                materialGroup->setOrder(materialChunk->getOrder());

                uint32 shaderObjectCount = materialChunk->getShaderObjectCount();
                for (uint32 i = 0; i < shaderObjectCount; ++i)
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

    void iModelDataIOOMPF::exportData(const iaString& filename, iNode* node, iSaveMode saveMode)
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

    void iModelDataIOOMPF::linkChunks(OMPF::ompfBaseChunk* currentChunk)
    {
        switch (currentChunk->getType())
        {
        case OMPFChunkType::ParticleSystem:
        {
            OMPF::ompfParticleSystemChunk* particleSystemChunk = static_cast<OMPF::ompfParticleSystemChunk*>(currentChunk);
            uint32 chunkID = particleSystemChunk->getID();
            uint32 nodeID = getNodeID(chunkID);
            iNodeParticleSystem* node = static_cast<iNodeParticleSystem*>(iNodeFactory::getInstance().getNode(nodeID));
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
            _nodeToChunk[node->getID()] = nextChunk->getID();
            _chunkToNode[nextChunk->getID()] = node->getID();

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

    OMPF::ompfMeshChunk* iModelDataIOOMPF::createMeshChunk(iNodeMesh *node)
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
                result->setTexCoordPerVertex(node->getMesh()->getTextureUnitCount());

                iaVector3f center(node->getBoundingSphere()._center._x, node->getBoundingSphere()._center._y, node->getBoundingSphere()._center._z);
                result->setBoundingSphere(center, node->getBoundingSphere()._radius);

                result->setVertexCount(node->getMesh()->getVertexCount());
                result->setVertexData(reinterpret_cast<char*>(node->getMesh()->getVertexData()), node->getMesh()->getVertexDataSize());

                result->setIndexCount(node->getMesh()->getIndexesCount());
                result->setIndexData(reinterpret_cast<char*>(node->getMesh()->getIndexData()), node->getMesh()->getIndexDataSize());

                for (uint32 i = 0; i < node->getMesh()->getTextureUnitCount(); ++i)
                {
                    iaString relative = iaDirectory::getRelativePath(_filename, node->getTargetMaterial()->getTexture(i)->getFilename());
                    result->setTexture(relative, i);
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

    OMPF::ompfTransformChunk* iModelDataIOOMPF::createTransformChunk(iNodeTransform *node)
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

        iMaterialGroup* materialGroup = iMaterialResourceFactory::getInstance().getMaterialGroup(materialID);
        if (materialGroup != nullptr)
        {
            iMaterial* material = materialGroup->getMaterial();

            if (material->hasShader())
            {
                auto shaderSources = material->getShaderSources();
                auto iterShaderSources = shaderSources.begin();
                while (iterShaderSources != shaderSources.end())
                {
                    result->addShader((*iterShaderSources)._filename, static_cast<OMPF::OMPFShaderType>((*iterShaderSources)._type));
                    iterShaderSources++;
                }
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