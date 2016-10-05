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
using namespace Igor;

PlantMeshGenerator::PlantMeshGenerator()
{
    _identifier = "pg";
    _name = "Plant Generator";
    _meshBuilderTrunk.setJoinVertexes();
    _meshBuilderLeaves.setJoinVertexes();
    _meshBuilderBuds.setJoinVertexes();
    _meshBuilderFlowers.setJoinVertexes();
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

    generateMesh(_skeleton.getRootJoint());

    iNodeMesh* meshNodeTrunk = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    shared_ptr<iMesh> meshTrunk = _meshBuilderTrunk.createMesh();

    if (meshTrunk.get() != nullptr)
    {
        meshNodeTrunk->setMesh(meshTrunk);
        meshNodeTrunk->setMaterial(plantInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNodeTrunk->getTargetMaterial();
        targetMaterial->setAmbient(plantInformation->_trunkColor);
        targetMaterial->setDiffuse(iaColor3f(0.5f, 0.5f, 0.5f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNodeTrunk);
    }

    iNodeMesh* meshNodeFlowers = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    shared_ptr<iMesh> meshFlowers = _meshBuilderFlowers.createMesh();

    if (meshFlowers.get() != nullptr)
    {
        meshNodeFlowers->setMesh(meshFlowers);
        meshNodeFlowers->setMaterial(plantInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNodeFlowers->getTargetMaterial();
        targetMaterial->setAmbient(plantInformation->_flowerColor);
        targetMaterial->setDiffuse(iaColor3f(0.5f, 0.5f, 0.5f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNodeFlowers);
    }

    iNodeMesh* meshNodeBuds = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    shared_ptr<iMesh> meshBuds = _meshBuilderBuds.createMesh();

    if (meshBuds.get() != nullptr)
    {
        meshNodeBuds->setMesh(meshBuds);
        meshNodeBuds->setMaterial(plantInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNodeBuds->getTargetMaterial();
        targetMaterial->setAmbient(plantInformation->_budColor);
        targetMaterial->setDiffuse(iaColor3f(0.5f, 0.5f, 0.5f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNodeBuds);
    }

    iNodeMesh* meshNodeLeafs = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
    _meshBuilderLeaves.calcNormals();
    shared_ptr<iMesh> meshLeafs = _meshBuilderLeaves.createMesh();

    if (meshLeafs.get() != nullptr)
    {
        meshNodeLeafs->setMesh(meshLeafs);
        meshNodeLeafs->setMaterial(plantInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNodeLeafs->getTargetMaterial();
        targetMaterial->setAmbient(plantInformation->_leafColor);
        targetMaterial->setDiffuse(iaColor3f(0.5f, 0.5f, 0.5f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNodeLeafs);
    }

    return result;
}

void PlantMeshGenerator::generateMesh(iJoint* joint)
{
    if (joint != nullptr)
    {
        iaMatrixf matrixRotate;
        iaMatrixf saveModelMatrix;

        vector<iBone*> children = joint->getChildren();
        for (auto bone : children)
        {
            saveModelMatrix = _modelMatrix;
            
            iaVector3f dir(0, bone->getLenght(), 0);
            bone->getMatrix(matrixRotate);

            _modelMatrix *= matrixRotate;

            // generate actual mesh now
            generateMesh(static_cast<SectionType>(reinterpret_cast<int>(bone->getCustomData())), dir);

            _modelMatrix.translate(dir);

            if (bone->getTopJoint() != nullptr)
            {
                generateMesh(bone->getTopJoint());
            }

            _modelMatrix = saveModelMatrix;
        }
    }
}

void PlantMeshGenerator::generateTrunk(const iaVector3f& dir)
{
    const iaVector3f vecs[4] = { {1,0,1}, { 1, 0, -1 } , { -1, 0, -1 }, { -1, 0, 1 } };

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a *= 0.03;
    b *= 0.03;
    c *= 0.03;
    d *= 0.03;

    uint32 ai = _meshBuilderTrunk.addVertex(_modelMatrix * a);
    uint32 bi = _meshBuilderTrunk.addVertex(_modelMatrix * b);
    uint32 ci = _meshBuilderTrunk.addVertex(_modelMatrix * c);
    uint32 di = _meshBuilderTrunk.addVertex(_modelMatrix * d);

    a += dir;
    b += dir;
    c += dir;
    d += dir;

    uint32 ati = _meshBuilderTrunk.addVertex(_modelMatrix * a);
    uint32 bti = _meshBuilderTrunk.addVertex(_modelMatrix * b);
    uint32 cti = _meshBuilderTrunk.addVertex(_modelMatrix * c);
    uint32 dti = _meshBuilderTrunk.addVertex(_modelMatrix * d);

    _meshBuilderTrunk.setNormal(ai, vecs[0]);
    _meshBuilderTrunk.setNormal(bi, vecs[1]);
    _meshBuilderTrunk.setNormal(ci, vecs[2]);
    _meshBuilderTrunk.setNormal(di, vecs[3]);

    _meshBuilderTrunk.setNormal(ati, vecs[0]);
    _meshBuilderTrunk.setNormal(bti, vecs[1]);
    _meshBuilderTrunk.setNormal(cti, vecs[2]);
    _meshBuilderTrunk.setNormal(dti, vecs[3]);

    _meshBuilderTrunk.addTriangle(ai, bi, bti);
    _meshBuilderTrunk.addTriangle(ai, bti, ati);

    _meshBuilderTrunk.addTriangle(ai, ati, dti);
    _meshBuilderTrunk.addTriangle(dti, di, ai);

    _meshBuilderTrunk.addTriangle(bi, ci, cti);
    _meshBuilderTrunk.addTriangle(cti, bti, bi);

    _meshBuilderTrunk.addTriangle(ci, di, dti);
    _meshBuilderTrunk.addTriangle(dti, cti, ci);
}

void PlantMeshGenerator::generateFlower(const iaVector3f& dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.6, 0.6, 0.6);

    const iaVector3f vecs[4] = { { 0,0,0 },{ 1, 0.2f, 0 } ,{ 0.75f, 0.4f, -0.2f },{ 0.75f, 0.4f, 0.2f } };

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a = scale * a;
    b = scale * b;
    c = scale * c;
    d = scale * d;

    for (int i = 0; i < 6; ++i)
    {
        current = _modelMatrix;
        current *= rotate;
        rotate.rotate(1.047, iaAxis::Y);

        uint32 ai = _meshBuilderFlowers.addVertex(current * a);
        uint32 bi = _meshBuilderFlowers.addVertex(current * b);
        uint32 ci = _meshBuilderFlowers.addVertex(current * c);
        uint32 di = _meshBuilderFlowers.addVertex(current * d);

        _meshBuilderFlowers.setNormal(ai, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(bi, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(ci, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(di, iaVector3f(0, 1, 0));

        _meshBuilderFlowers.addTriangle(ai, di, bi);
        _meshBuilderFlowers.addTriangle(ai, bi, ci);
    }
}

void PlantMeshGenerator::generateBud(const iaVector3f& dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.2, 0.2, 0.2);

    const iaVector3f vecs[4] = { { 0,0,0 },{ 1, 1.2f, 0 } ,{ 0, 1.2f, 1 },{ 0, 2, 0 } };

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a = scale * a;
    b = scale * b;
    c = scale * c;
    d = scale * d;

    for (int i = 0; i < 4; ++i)
    {
        current = _modelMatrix;
        current *= rotate;
        rotate.rotate(M_PI * 0.5, iaAxis::Y);

        uint32 ai = _meshBuilderBuds.addVertex(current * a);
        uint32 bi = _meshBuilderBuds.addVertex(current * b);
        uint32 ci = _meshBuilderBuds.addVertex(current * c);
        uint32 di = _meshBuilderBuds.addVertex(current * d);

        _meshBuilderBuds.setNormal(ai, iaVector3f(0, -1, 0));
        _meshBuilderBuds.setNormal(bi, iaVector3f(1, 0, 0));
        _meshBuilderBuds.setNormal(ci, iaVector3f(0, 0, 1));
        _meshBuilderBuds.setNormal(di, iaVector3f(0, 1, 0));

        _meshBuilderBuds.addTriangle(ai, bi, ci);
        _meshBuilderBuds.addTriangle(ci, bi, di);
    }
}

void PlantMeshGenerator::generateLeaf(const iaVector3f& dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.9, 0.9, 0.9);

    const iaVector3f vecs[4] = { { 0,0,0 },{ 1, 0.2f, 0 } ,{ 0.75f, 0.4f, -0.2f },{ 0.75f, 0.4f, 0.2f } };

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a = scale * a;
    b = scale * b;
    c = scale * c;
    d = scale * d;

    rotate.rotate(rand() % 100 / 100.0 * M_PI * 2, iaAxis::Y);
    current = _modelMatrix;
    current *= rotate;

    uint32 ai = _meshBuilderLeaves.addVertex(current * a);
    uint32 bi = _meshBuilderLeaves.addVertex(current * b);
    uint32 ci = _meshBuilderLeaves.addVertex(current * c);
    uint32 di = _meshBuilderLeaves.addVertex(current * d);

    _meshBuilderLeaves.setNormal(ai, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(bi, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(ci, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(di, iaVector3f(0, 1, 0));

    _meshBuilderLeaves.addTriangle(ai, di, bi);
    _meshBuilderLeaves.addTriangle(ai, bi, ci);
}

void PlantMeshGenerator::generateMesh(SectionType sectionType, const iaVector3f& dir)
{
    switch (sectionType)
    {
    case SectionType::Trunk:
    case SectionType::Branch:
        generateTrunk(dir);
        break;

    case SectionType::Bud:
        generateBud(dir);
        break;

    case SectionType::Flower:
        generateFlower(dir);
        break;

    case SectionType::Leaf:
        generateLeaf(dir);
        break;
    }
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
        {
            _skeleton.addBone(rotationMatrix, _segmentLength * variation);
            iBone* bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setCustomData((void*)(int)SectionType::Trunk);
            }

            rotationMatrix.identity();
        }
        break;

        case 'x':
        {
            _skeleton.addBone(rotationMatrix, _segmentLength * variation);
            iBone* bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setCustomData((void*)(int)SectionType::Branch);
            }

            rotationMatrix.identity();
        }
        break;

        case '*':
        {
            iBone* bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData((void*)(int)SectionType::Bud);
            }
        }
        break;

        case 'o':
        {
            iBone* bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData((void*)(int)SectionType::Flower);
            }
        }
        break;

        case 'O':
        {
            iBone* bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData((void*)(int)SectionType::Leaf);
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