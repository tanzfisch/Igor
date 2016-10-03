// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "PlantMeshGenerator.h"

#include <iLSystem.h>
#include <iNodeFactory.h>
#include <iNodeMesh.h>
#include <iNodeLODSwitch.h>
#include <iModel.h>
#include <iaMemBlock.h>
#include <iMaterialResourceFactory.h>
#include <iTextureResourceFactory.h>
#include <iTargetMaterial.h>
#include <iNodeTransform.h>
#include <iBoneFactory.h>
using namespace Igor;

PlantMeshGenerator::PlantMeshGenerator()
{
    _identifier = "pg";
    _name = "Plant Generator";
    _meshBuilder.setJoinVertexes();
}

iModelDataIO* PlantMeshGenerator::createInstance()
{
    PlantMeshGenerator* result = new PlantMeshGenerator();
    return static_cast<iModelDataIO*>(result);
}

iNode* PlantMeshGenerator::importData(const iaString& sectionName, iModelDataInputParameter* parameter)
{
    PlantInformation* plantInformation = reinterpret_cast<PlantInformation*>(parameter->_parameters.getDataPointer());
    iLSystem* lSystem = plantInformation->_lSystem;
    _segmentLength = plantInformation->_segmentLenght;
    _segmentAngle = plantInformation->_segmentAngle;

    iNode* result = iNodeFactory::getInstance().createNode(iNodeType::iNode);

    srand(plantInformation->_seed);
    iaString sentence = lSystem->generate(plantInformation->_axiom, plantInformation->_iterations);
    generateSkeleton(sentence);

    iJoint* joint = iBoneFactory::getInstance().getJoint(_skeleton.getRootJoint());
    generateMesh(joint);

    iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    shared_ptr<iMesh> mesh = _meshBuilder.createMesh();

    if (mesh.get() != nullptr)
    {
        meshNode->setMesh(mesh);
        meshNode->setMaterial(plantInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().loadFile("blue.png"), 0);
        targetMaterial->setAmbient(iaColor3f(0.2f, 0.7f, 0.1f));
        targetMaterial->setDiffuse(iaColor3f(0.3f, 0.9f, 0.2f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNode);
    }
    return result;
}

void PlantMeshGenerator::generateMesh(iJoint* joint)
{
    if (joint != nullptr)
    {
        iaMatrixf matrixRotate;
        iaMatrixf saveModelMatrix;

        vector<uint64> children = joint->getChildren();
        for (auto childBone : children)
        {
            saveModelMatrix = _modelMatrix;

            iBone* bone = iBoneFactory::getInstance().getBone(childBone);
            iaVector3f dir(0, bone->getLenght(), 0);
            bone->getMatrix(matrixRotate);

            _modelMatrix *= matrixRotate;

            // generate actual mesh now
            generateMesh(reinterpret_cast<int>(bone->getCustomData()), dir, 0.05);
            
            _modelMatrix.translate(dir);

            if (bone->getTopJoint() != iJoint::INVALID_JOINT_ID)
            {
                iJoint* joint = iBoneFactory::getInstance().getJoint(bone->getTopJoint());
                generateMesh(joint);
            }

            _modelMatrix = saveModelMatrix;
        }
    }
}

void PlantMeshGenerator::generateMesh(int customValue, const iaVector3f& dir, float32 size)
{
    iaVector3f a(1, 0, 1);
    iaVector3f b(1, 0, -1);
    iaVector3f c(-1, 0, -1);
    iaVector3f d(-1, 0, 1);

    a *= size;
    b *= size;
    c *= size;
    d *= size;

    uint32 ai = _meshBuilder.addVertex(_modelMatrix * a);
    uint32 bi = _meshBuilder.addVertex(_modelMatrix * b);
    uint32 ci = _meshBuilder.addVertex(_modelMatrix * c);
    uint32 di = _meshBuilder.addVertex(_modelMatrix * d);

    a += dir;
    b += dir;
    c += dir;
    d += dir;

    uint32 ati = _meshBuilder.addVertex(_modelMatrix * a);
    uint32 bti = _meshBuilder.addVertex(_modelMatrix * b);
    uint32 cti = _meshBuilder.addVertex(_modelMatrix * c);
    uint32 dti = _meshBuilder.addVertex(_modelMatrix * d);

    _meshBuilder.setNormal(ai, iaVector3f(1, 0, 1));
    _meshBuilder.setNormal(bi, iaVector3f(1, 0, -1));
    _meshBuilder.setNormal(ci, iaVector3f(-1, 0, -1));
    _meshBuilder.setNormal(di, iaVector3f(-1, 0, 1));

    _meshBuilder.setNormal(ati, iaVector3f(1, 0, 1));
    _meshBuilder.setNormal(bti, iaVector3f(1, 0, -1));
    _meshBuilder.setNormal(cti, iaVector3f(-1, 0, -1));
    _meshBuilder.setNormal(dti, iaVector3f(-1, 0, 1));

    _meshBuilder.addTriangle(ai, bi, bti);
    _meshBuilder.addTriangle(ai, bti, ati);

    _meshBuilder.addTriangle(ai, ati, dti);
    _meshBuilder.addTriangle(dti, di, ai);

    _meshBuilder.addTriangle(bi, ci, cti);
    _meshBuilder.addTriangle(cti, bti, bi);

    _meshBuilder.addTriangle(ci, di, dti);
    _meshBuilder.addTriangle(dti, cti, ci);
}


void PlantMeshGenerator::generateSkeleton(const iaString& sentence)
{
    iaMatrixf rotationMatrix;

    for (int i = 0; i < sentence.getSize(); ++i)
    {
        float32 variation = 1.5 - (((rand() % 100) / 100.0f));

        switch (sentence[i])
        {
        case 'F':
        case 'X':
            _skeleton.addBone(rotationMatrix, _segmentLength * variation);
            rotationMatrix.identity();
            break;

        case '*':
        {
            iBone* bone = iBoneFactory::getInstance().getBone(_skeleton.getLastBone());
            if (bone != nullptr)
            {
                bone->setCustomData((void*)0x01);
            }
        }
        break;

        case 'o':
        {
            iBone* bone = iBoneFactory::getInstance().getBone(_skeleton.getLastBone());
            if (bone != nullptr)
            {
                bone->setCustomData((void*)0x02);
            }
        }
        break;

        case 'O':
        {
            iBone* bone = iBoneFactory::getInstance().getBone(_skeleton.getLastBone());
            if (bone != nullptr)
            {
                bone->setCustomData((void*)0x03);
            }
        }
        break;

        case 'R':
        {
            rotationMatrix.rotate(_segmentAngle * variation, iaAxis::X);
        }
        break;

        case 'L':
        {
            rotationMatrix.rotate(-_segmentAngle * variation, iaAxis::X);
        }
        break;

        case '+':
        {
            rotationMatrix.rotate(_segmentAngle * variation, iaAxis::Z);
        }
        break;

        case '-':
        {
            rotationMatrix.rotate(-_segmentAngle * variation, iaAxis::Z);
        }
        break;

        case '[':
            _skeleton.push();
            break;

        case ']':
            _skeleton.pop();
            break;
        }
    }
}