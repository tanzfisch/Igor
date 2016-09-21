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
#include <iMeshBuilder.h>
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

    /*iContouringCubes contouringCubes;
    contouringCubes.setVoxelData(voxelData);
    shared_ptr<iMesh> mesh = contouringCubes.compile(iaVector3I(), iaVector3I(width, height, depth));

    if (mesh.get() != nullptr)
    {
        iNodeMesh* meshNode = static_cast<iNodeMesh*>(iNodeFactory::getInstance().createNode(iNodeType::iNodeMesh));
        meshNode->setMesh(mesh);
        meshNode->setMaterial(tileInformation->_materialID);

        iTargetMaterial* targetMaterial = meshNode->getTargetMaterial();
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("dirt.png"), 0);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("grass.png"), 1);
        targetMaterial->setTexture(iTextureResourceFactory::getInstance().requestFile("rock.png"), 2);
        targetMaterial->setAmbient(iaColor3f(0.7f, 0.7f, 0.7f));
        targetMaterial->setDiffuse(iaColor3f(0.9f, 0.9f, 0.9f));
        targetMaterial->setSpecular(iaColor3f(0.1f, 0.1f, 0.1f));
        targetMaterial->setEmissive(iaColor3f(0.01f, 0.01f, 0.01f));
        targetMaterial->setShininess(100.0f);

        result->insertNode(meshNode);
    }*/

    return result;
}

void PlantMeshGenerator::generateMesh(iJoint* joint)
{
 /*   if (joint != nullptr)
    {
        iaVector3f dir;

        vector<uint64> children = joint->getChildren();
        for (auto childBone : children)
        {
            iaMatrixf modelMatrix;
            iRenderer::getInstance().getModelMatrix(modelMatrix);
            iaMatrixf saveModelMatrix = modelMatrix;

            iBone* bone = iBoneFactory::getInstance().getBone(childBone);
            dir._y = bone->getLenght();

            iaMatrixf matrixRotate;
            bone->getMatrix(matrixRotate);

            iRenderer::getInstance().getModelMatrix(modelMatrix);
            modelMatrix *= matrixRotate;
            iRenderer::getInstance().setModelMatrix(modelMatrix);

            iRenderer::getInstance().setColor(_stemColor);
            iRenderer::getInstance().drawLine(iaVector3f(), dir);

            iaMatrixf matrixTranslate;
            matrixTranslate.translate(dir);

            modelMatrix *= matrixTranslate;
            iRenderer::getInstance().setModelMatrix(modelMatrix);

            int value = reinterpret_cast<int>(bone->getCustomData());
            if (value != 0)
            {
                switch (value)
                {
                case 1:
                    iRenderer::getInstance().setColor(_shotColor);
                    iRenderer::getInstance().setPointSize(6);
                    break;
                case 2:
                    iRenderer::getInstance().setColor(_budColor);
                    iRenderer::getInstance().setPointSize(10);
                    break;
                case 3:
                    iRenderer::getInstance().setColor(_blossomColor);
                    iRenderer::getInstance().setPointSize(15);
                    break;
                }

                iRenderer::getInstance().drawPoint(iaVector3f());
            }

            if (bone->getTopJoint() != iJoint::INVALID_JOINT_ID)
            {
                iJoint* joint = iBoneFactory::getInstance().getJoint(bone->getTopJoint());
                drawLSystem(joint);
            }

            iRenderer::getInstance().setModelMatrix(saveModelMatrix);
        }
    }*/
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