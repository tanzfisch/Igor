// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "PlantMeshGenerator.h"

#include <igor/generation/iLSystem.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeLODSwitch.h>

#include <igor/resources/material/iMaterial.h>
#include <igor/resources/iResourceManager.h>
#include <igor/scene/nodes/iNodeTransform.h>
using namespace igor;

PlantMeshGenerator::PlantMeshGenerator()
{
    _identifier = "pmg";
    _name = "Plant Mesh Generator";
    _meshBuilderTrunk.setJoinVertexes();
    _meshBuilderLeaves.setJoinVertexes();
    _meshBuilderBuds.setJoinVertexes();
    _meshBuilderFlowers.setJoinVertexes();
}

iModelDataIO *PlantMeshGenerator::createInstance()
{
    PlantMeshGenerator *result = new PlantMeshGenerator();
    return static_cast<iModelDataIO *>(result);
}

iNodePtr PlantMeshGenerator::importData(const iParameters &parameters)
{
    const iaString &sectionName = parameters.getParameter<iaString>("name", "");
    iLSystem *lSystem = parameters.getParameter<iLSystem *>("lsystem", nullptr);
    const iaString &axiom = parameters.getParameter<iaString>("axiom", "");
    const uint32 iterations = parameters.getParameter<uint32>("iterations", 0);
    iShaderPtr shader = parameters.getParameter<iShaderPtr>(IGOR_RESOURCE_SHADER, nullptr);
    const iaColor3f trunkColor = parameters.getParameter<iaColor3f>("trunkColor", iaColor3f());
    const iaColor3f branchColor = parameters.getParameter<iaColor3f>("branchColor", iaColor3f());
    const iaColor3f budColor = parameters.getParameter<iaColor3f>("budColor", iaColor3f());
    const iaColor3f flowerColor = parameters.getParameter<iaColor3f>("flowerColor", iaColor3f());
    const iaColor3f leafColor = parameters.getParameter<iaColor3f>("leafColor", iaColor3f());
    _segmentLength = parameters.getParameter<float32>("segmentLength", 0.25f);
    _segmentAngle = parameters.getParameter<float32>("segmentAngle", 0.25f);

    iNodePtr result = iNodeManager::getInstance().createNode<iNode>();

    _rand.setSeed(parameters.getParameter<uint64>(IGOR_RESOURCE_PARAM_SEED, 1234));
    iaString sentence = lSystem->generate(axiom, iterations, _rand.getNext());

    generateSkeleton(sentence);
    generateMesh(_skeleton.getRootJoint());

    iMeshPtr meshTrunk = _meshBuilderTrunk.createMesh();

    if (meshTrunk != nullptr)
    {
        iNodeMesh *meshNodeTrunk = iNodeManager::getInstance().createNode<iNodeMesh>();
        meshNodeTrunk->setMesh(meshTrunk);

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_AMBIENT, trunkColor},
                           {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
                           {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.1f, 0.1f, 0.1f)},
                           {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.01f, 0.01f, 0.01f)},
                           {IGOR_RESOURCE_PARAM_SHININESS, 100.0f},
                           {IGOR_RESOURCE_PARAM_SHADER, shader}});
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);

        meshNodeTrunk->setMaterial(material);
        result->insertNode(meshNodeTrunk);
    }

    iMeshPtr meshFlowers = _meshBuilderFlowers.createMesh();

    if (meshFlowers != nullptr)
    {
        iNodeMesh *meshNodeFlowers = iNodeManager::getInstance().createNode<iNodeMesh>();
        meshNodeFlowers->setMesh(meshFlowers);

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_AMBIENT, flowerColor},
                           {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
                           {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.1f, 0.1f, 0.1f)},
                           {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.01f, 0.01f, 0.01f)},
                           {IGOR_RESOURCE_PARAM_SHININESS, 100.0f},
                           {IGOR_RESOURCE_PARAM_SHADER, shader}});
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);

        meshNodeFlowers->setMaterial(material);
        result->insertNode(meshNodeFlowers);
    }

    iMeshPtr meshBuds = _meshBuilderBuds.createMesh();

    if (meshBuds != nullptr)
    {
        iNodeMesh *meshNodeBuds = iNodeManager::getInstance().createNode<iNodeMesh>();
        meshNodeBuds->setMesh(meshBuds);

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_AMBIENT, budColor},
                           {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
                           {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.1f, 0.1f, 0.1f)},
                           {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.01f, 0.01f, 0.01f)},
                           {IGOR_RESOURCE_PARAM_SHININESS, 100.0f},
                           {IGOR_RESOURCE_PARAM_SHADER, shader}});
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);
        meshNodeBuds->setMaterial(material);
        result->insertNode(meshNodeBuds);
    }

    _meshBuilderLeaves.calcNormals();
    iMeshPtr meshLeafs = _meshBuilderLeaves.createMesh();

    if (meshLeafs != nullptr)
    {
        iNodeMesh *meshNodeLeafs = iNodeManager::getInstance().createNode<iNodeMesh>();
        meshNodeLeafs->setMesh(meshLeafs);

        iParameters param({{IGOR_RESOURCE_PARAM_TYPE, IGOR_RESOURCE_MATERIAL},
                           {IGOR_RESOURCE_PARAM_GENERATE, true},
                           {IGOR_RESOURCE_PARAM_AMBIENT, leafColor},
                           {IGOR_RESOURCE_PARAM_DIFFUSE, iaColor3f(0.5f, 0.5f, 0.5f)},
                           {IGOR_RESOURCE_PARAM_SPECULAR, iaColor3f(0.1f, 0.1f, 0.1f)},
                           {IGOR_RESOURCE_PARAM_EMISSIVE, iaColor3f(0.01f, 0.01f, 0.01f)},
                           {IGOR_RESOURCE_PARAM_SHININESS, 100.0f},
                           {IGOR_RESOURCE_PARAM_SHADER, shader}});
        iMaterialPtr material = iResourceManager::getInstance().loadResource<iMaterial>(param);
        meshNodeLeafs->setMaterial(material);
        result->insertNode(meshNodeLeafs);
    }

    return result;
}

void PlantMeshGenerator::generateMesh(iJoint *joint)
{
    if (joint != nullptr)
    {
        iaMatrixf matrixRotate;
        iaMatrixf saveModelMatrix;

        std::vector<iBone *> children = joint->getChildren();
        for (auto bone : children)
        {
            saveModelMatrix = _modelMatrix;

            iaVector3f dir(0, bone->getLenght(), 0);
            bone->getMatrix(matrixRotate);

            _modelMatrix *= matrixRotate;

            // generate actual mesh now
            std::any userData = bone->getCustomData();
            generateMesh(std::any_cast<SectionType>(userData), dir);

            _modelMatrix.translate(dir);

            if (bone->getTopJoint() != nullptr)
            {
                generateMesh(bone->getTopJoint());
            }

            _modelMatrix = saveModelMatrix;
        }
    }
}

void PlantMeshGenerator::generateTrunk(const iaVector3f &dir)
{
    const iaVector3f vecs[4] = {{1, 0, 1}, {1, 0, -1}, {-1, 0, -1}, {-1, 0, 1}};

    _meshBuilderTrunk.setMatrix(_modelMatrix);

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a *= 0.03;
    b *= 0.03;
    c *= 0.03;
    d *= 0.03;

    uint32 ai = _meshBuilderTrunk.addVertex(a);
    uint32 bi = _meshBuilderTrunk.addVertex(b);
    uint32 ci = _meshBuilderTrunk.addVertex(c);
    uint32 di = _meshBuilderTrunk.addVertex(d);

    a += dir;
    b += dir;
    c += dir;
    d += dir;

    uint32 ati = _meshBuilderTrunk.addVertex(a);
    uint32 bti = _meshBuilderTrunk.addVertex(b);
    uint32 cti = _meshBuilderTrunk.addVertex(c);
    uint32 dti = _meshBuilderTrunk.addVertex(d);

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

void PlantMeshGenerator::generateFlower(const iaVector3f &dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.6, 0.6, 0.6);

    const iaVector3f vecs[4] = {{0, 0, 0}, {1, 0.2f, 0}, {0.75f, 0.4f, -0.2f}, {0.75f, 0.4f, 0.2f}};

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

        _meshBuilderFlowers.setMatrix(current);

        uint32 ai = _meshBuilderFlowers.addVertex(a);
        uint32 bi = _meshBuilderFlowers.addVertex(b);
        uint32 ci = _meshBuilderFlowers.addVertex(c);
        uint32 di = _meshBuilderFlowers.addVertex(d);

        _meshBuilderFlowers.setNormal(ai, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(bi, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(ci, iaVector3f(0, 1, 0));
        _meshBuilderFlowers.setNormal(di, iaVector3f(0, 1, 0));

        _meshBuilderFlowers.addTriangle(ai, di, bi);
        _meshBuilderFlowers.addTriangle(ai, bi, ci);
    }
}

void PlantMeshGenerator::generateBud(const iaVector3f &dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.2, 0.2, 0.2);

    const iaVector3f vecs[4] = {{0, 0, 0}, {1, 1.2f, 0}, {0, 1.2f, 1}, {0, 2, 0}};

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

        _meshBuilderBuds.setMatrix(current);

        uint32 ai = _meshBuilderBuds.addVertex(a);
        uint32 bi = _meshBuilderBuds.addVertex(b);
        uint32 ci = _meshBuilderBuds.addVertex(c);
        uint32 di = _meshBuilderBuds.addVertex(d);

        _meshBuilderBuds.setNormal(ai, iaVector3f(0, -1, 0));
        _meshBuilderBuds.setNormal(bi, iaVector3f(1, 0, 0));
        _meshBuilderBuds.setNormal(ci, iaVector3f(0, 0, 1));
        _meshBuilderBuds.setNormal(di, iaVector3f(0, 1, 0));

        _meshBuilderBuds.addTriangle(ai, bi, ci);
        _meshBuilderBuds.addTriangle(ci, bi, di);
    }
}

void PlantMeshGenerator::generateLeaf(const iaVector3f &dir)
{
    iaMatrixf rotate;
    iaMatrixf scale;
    iaMatrixf current;

    scale.scale(0.7, 0.7, 0.7);

    const iaVector3f vecs[4] = {{0, 0, 0}, {1, 0.2f, 0}, {0.75f, 0.4f, -0.2f}, {0.75f, 0.4f, 0.2f}};

    iaVector3f a = vecs[0];
    iaVector3f b = vecs[1];
    iaVector3f c = vecs[2];
    iaVector3f d = vecs[3];

    a = scale * a;
    b = scale * b;
    c = scale * c;
    d = scale * d;

    rotate.rotate(_rand.getNext() % 100 / 100.0 * M_PI * 2, iaAxis::Y);
    current = _modelMatrix;
    current *= rotate;

    _meshBuilderLeaves.setMatrix(current);

    uint32 ai = _meshBuilderLeaves.addVertex(a);
    uint32 bi = _meshBuilderLeaves.addVertex(b);
    uint32 ci = _meshBuilderLeaves.addVertex(c);
    uint32 di = _meshBuilderLeaves.addVertex(d);

    _meshBuilderLeaves.setNormal(ai, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(bi, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(ci, iaVector3f(0, 1, 0));
    _meshBuilderLeaves.setNormal(di, iaVector3f(0, 1, 0));

    _meshBuilderLeaves.addTriangle(ai, di, bi);
    _meshBuilderLeaves.addTriangle(ai, bi, ci);
}

void PlantMeshGenerator::generateMesh(SectionType sectionType, const iaVector3f &dir)
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

void PlantMeshGenerator::generateSkeleton(const iaString &sentence)
{
    iaMatrixf rotationMatrix;

    for (int64 i = 0; i < sentence.getLength(); ++i)
    {
        float32 variation = 1.5 - (((_rand.getNext() % 100) / 100.0f));

        switch (sentence[i])
        {
        case 'F':
        case 'X':
        {
            _skeleton.addBone(rotationMatrix, _segmentLength * variation);
            iBone *bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setCustomData(SectionType::Trunk);
            }

            rotationMatrix.identity();
        }
        break;

        case 'x':
        {
            _skeleton.addBone(rotationMatrix, _segmentLength * variation);
            iBone *bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setCustomData(SectionType::Branch);
            }

            rotationMatrix.identity();
        }
        break;

        case '*':
        {
            iBone *bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData(SectionType::Bud);
            }
        }
        break;

        case 'o':
        {
            iBone *bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData(SectionType::Flower);
            }
        }
        break;

        case 'O':
        {
            iBone *bone = _skeleton.getLastBone();
            if (bone != nullptr)
            {
                bone->setLenght(0);
                bone->setCustomData(SectionType::Leaf);
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