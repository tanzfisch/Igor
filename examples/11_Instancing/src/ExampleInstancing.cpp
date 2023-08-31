// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

#include <igor/resources/material/iMaterial.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/scene/iScene.h>
#include <igor/scene/nodes/iNodeLight.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/system/iMouse.h>
using namespace igor;

ExampleInstancing::ExampleInstancing(iWindowPtr window)
    : ExampleBase(window, "Instacing")
{
}

void ExampleInstancing::onInit()
{
    const float32 spacing = 5.0f;
    const int32 amountPerDimension = 60;

    // switching of vsync for maximum output
    getWindow()->setVSync(false);

    // setup camera
    // we want a camera which can be rotated arround the origin
    // we will acchive that with 3 transform nodes
    // one is for the heading
    iNodeTransform *cameraHeading = iNodeManager::getInstance().createNode<iNodeTransform>();
    // give the transform node a name. naming is optional and ist jus for helping to debug.
    // Names do not have to be unique but since it is possible to find nodes by name they better are
    cameraHeading->setName("camera heading");
    _cameraHeading = cameraHeading->getID();
    // one is for the pitch
    iNodeTransform *cameraPitch = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraPitch->setName("camera pitch");
    _cameraPitch = cameraPitch->getID();
    // and one is for translation or distance from the origin
    iNodeTransform *cameraTranslation = iNodeManager::getInstance().createNode<iNodeTransform>();
    cameraTranslation->setName("camera translation");
    // translate away from origin
    cameraTranslation->translate(0, 0, spacing * amountPerDimension * 0.5);
    _cameraTranslation = cameraTranslation->getID();
    // from all nodes that we want to control later we save the node ID
    // and last but not least we create a camera node
    iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
    camera->setName("camera");
    // and build everything together
    // first we add the heading to the root node
    getScene()->getRoot()->insertNode(cameraHeading);
    // than the pitch to the heading node
    cameraHeading->insertNode(cameraPitch);
    // then the translation to the pitch node
    cameraPitch->insertNode(cameraTranslation);
    // and than we add the camera to the translation
    cameraTranslation->insertNode(camera);
    // and finally we tell the view which camera shall be the current one. for this to work a camera must be part of a
    // scene assiciated with the view wich we achived by adding all those nodes on to an other starting with the root node
    getView().setCurrentCamera(camera->getID());

    // we have to override the material which is stored within the model
    // to do that we load a new material that is using instancing
    _materialWithInstancingA = iMaterialResourceFactory::getInstance().loadMaterial("instancing_textured.mat");
    _materialWithInstancingB = iMaterialResourceFactory::getInstance().loadMaterial("instancing_flat_shaded.mat");

    // now we can just put copies of that model in the scene
    iNodeTransform *transformGroup = iNodeManager::getInstance().createNode<iNodeTransform>();
    transformGroup->translate(-((amountPerDimension - 1) * spacing * 0.5), -((amountPerDimension - 1) * spacing * 0.5), -((amountPerDimension - 1) * spacing * 0.5));
    getScene()->getRoot()->insertNode(transformGroup);

    int count = 0;
    _perlinNoise.generateBase(42);

    iaRandomNumberGenerator random;

    // todo need to be able to load a mesh from file without all of this
    iMeshPtr catMesh;
    iTargetMaterialPtr catTargetMaterial;
    iModelPtr modelCat = iResourceManager::getInstance().loadResource<iModel>("cat.ompf");
    if(modelCat->getNode()->getType() == iNodeType::iNodeMesh)
    {
        iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(modelCat->getNode());
        catTargetMaterial = meshNode->getTargetMaterial();
        catMesh = meshNode->getMesh();
    }

    iMeshPtr createMesh;
    iTargetMaterialPtr crateTargetMaterial;
    iModelPtr modelCrate = iResourceManager::getInstance().loadResource<iModel>("crate.ompf");
    if(modelCrate->getNode()->getType() == iNodeType::iNodeMesh)
    {
        iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(modelCrate->getNode());
        crateTargetMaterial = meshNode->getTargetMaterial();
        createMesh = meshNode->getMesh();
    }    

    iMeshPtr cubeMesh;
    iTargetMaterialPtr cubeTargetMaterial;
    iModelPtr modelCube = iResourceManager::getInstance().loadResource<iModel>("cube_green.ompf");
    if(modelCube->getNode()->getType() == iNodeType::iNodeMesh)
    {
        iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(modelCube->getNode());
        cubeTargetMaterial = meshNode->getTargetMaterial();
        cubeMesh = meshNode->getMesh();
    }    

    iMeshPtr teapotMesh;
    iTargetMaterialPtr teapotTargetMaterial;
    iModelPtr modelTeapot = iResourceManager::getInstance().loadResource<iModel>("teapot.ompf");
    if(modelTeapot->getNode()->getType() == iNodeType::iNodeMesh)
    {
        iNodeMeshPtr meshNode = static_cast<iNodeMeshPtr>(modelTeapot->getNode());
        teapotTargetMaterial = meshNode->getTargetMaterial();
        teapotMesh = meshNode->getMesh();
    }    

    // create a bunch of models
    for (int z = 0; z < amountPerDimension; ++z)
    {
        for (int y = 0; y < amountPerDimension; ++y)
        {
            for (int x = 0; x < amountPerDimension; ++x)
            {
                float32 noise = _perlinNoise.getValue(iaVector3d(x * 0.1, y * 0.1, z * 0.1), 3);
                if (noise < 0.62)
                {
                    continue;
                }

                iNodeTransform *transform = iNodeManager::getInstance().createNode<iNodeTransform>();
                transform->translate(static_cast<float32>(x) * spacing + random.getNextFloatRange(0.0, spacing),
                                     static_cast<float32>(y) * spacing + random.getNextFloatRange(0.0, spacing),
                                     static_cast<float32>(z) * spacing + random.getNextFloatRange(0.0, spacing));

                transform->rotate(random.getNextFloat() * M_PI * 2, iaAxis::X);
                transform->rotate(random.getNextFloat() * M_PI * 2, iaAxis::Y);
                transform->rotate(random.getNextFloat() * M_PI * 2, iaAxis::Z);

                iNodeMeshPtr meshNode = iNodeManager::getInstance().createNode<iNodeMesh>();
                
                
                switch (count % 4)
                {
                case 0:
                    meshNode->setMesh(catMesh);
                    meshNode->setTargetMaterial(catTargetMaterial);
                    meshNode->setMaterial(_materialWithInstancingA);
                    break;

                case 1:
                    meshNode->setMesh(createMesh);
                    meshNode->setTargetMaterial(crateTargetMaterial);
                    meshNode->setMaterial(_materialWithInstancingA);
                    break;

                case 2:
                    meshNode->setMesh(cubeMesh);
                    meshNode->setTargetMaterial(cubeTargetMaterial);
                    meshNode->setMaterial(_materialWithInstancingB);
                    break;

                case 3:
                    meshNode->setMesh(teapotMesh);
                    meshNode->setTargetMaterial(teapotTargetMaterial);
                    meshNode->setMaterial(_materialWithInstancingB);
                    break;
                }

                // building the created nodes together and insert them in the scene
                transformGroup->insertNode(transform);
                transform->insertNode(meshNode);

                count++;
            }
        }
    }

    con_info("instance count: " << count);

    // setup light
    // transform node for the lights orientation
    iNodeTransform *directionalLightRotate = iNodeManager::getInstance().createNode<iNodeTransform>();
    // keep transform node id so we can manipulate the light's position later
    _directionalLightRotate = directionalLightRotate->getID();
    // transform node for the lights distance to the origin
    iNodeTransform *directionalLightTranslate = iNodeManager::getInstance().createNode<iNodeTransform>();
    directionalLightTranslate->translate(100, 100, 100);
    // the light node
    iNodeLight *lightNode = iNodeManager::getInstance().createNode<iNodeLight>();
    lightNode->setAmbient(iaColor3f(0.3f, 0.3f, 0.3f));
    lightNode->setDiffuse(iaColor3f(0.8f, 0.8f, 0.8f));
    lightNode->setSpecular(iaColor3f(1.0f, 1.0f, 1.0f));
    // insert light to scene
    getScene()->getRoot()->insertNode(directionalLightRotate);
    directionalLightRotate->insertNode(directionalLightTranslate);
    directionalLightTranslate->insertNode(lightNode);

    // animation
    _animationTimingHandle = new iTimerHandle(iTimerTickDelegate(this, &ExampleInstancing::onUpdate), iaTime::fromMilliseconds(10));
    _animationTimingHandle->start();
}

void ExampleInstancing::onDeinit()
{
    // stop light animation
    if (_animationTimingHandle)
    {
        delete _animationTimingHandle;
        _animationTimingHandle = nullptr;
    }

    _materialWithInstancingA = nullptr;
    _materialWithInstancingB = nullptr;
}

void ExampleInstancing::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(ExampleInstancing::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(ExampleInstancing::onMouseWheelEvent));
}

bool ExampleInstancing::onMouseWheelEvent(iEventMouseWheel &event)
{
    iNodeTransform *camTranslation = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraTranslation));
    if (camTranslation != nullptr)
    {
        if (event.getWheelDelta() < 0)
        {
            camTranslation->translate(0, 0, 2);
        }
        else
        {
            camTranslation->translate(0, 0, -2);
        }
    }

    return true;
}

bool ExampleInstancing::onMouseMoveEvent(iEventMouseMove &event)
{
    if (iMouse::getInstance().getLeftButton())
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        iNodeTransform *cameraPitch = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraPitch));
        iNodeTransform *cameraHeading = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_cameraHeading));

        if (cameraPitch != nullptr &&
            cameraHeading != nullptr)
        {
            cameraPitch->rotate((from._y - to._y) * 0.0005f, iaAxis::X);
            cameraHeading->rotate((from._x - to._x) * 0.0005f, iaAxis::Y);
            iMouse::getInstance().setCenter();
        }

        return true;
    }

    return false;
}

void ExampleInstancing::onUpdate(const iaTime &time)
{
    iNodeTransform *directionalLightRotate = static_cast<iNodeTransform *>(iNodeManager::getInstance().getNode(_directionalLightRotate));
    directionalLightRotate->rotate(0.005f, iaAxis::Y);
}
