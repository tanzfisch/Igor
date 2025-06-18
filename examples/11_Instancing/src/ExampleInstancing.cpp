// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "ExampleInstancing.h"

ExampleInstancing::ExampleInstancing(iWindowPtr window)
    : ExampleBase(window, "Instacing", true, "")
{
}

void ExampleInstancing::onInit()
{
    const float64 spacing = 5.0;
    const int32 amountPerDimension = 5;

    // switching of vsync for maximum output
    getWindow()->setVSync(false);

    // create entity scene
    iPrefabPtr scenePrefab = iResourceManager::getInstance().createResource<iPrefab>();
    _entityScene = iEntitySystemModule::getInstance().getScene(scenePrefab->getSceneID());
    // _entityScene = iEntitySystemModule::getInstance().createScene();
    // setup octree for culling iOctreeComponents
    _entityScene->initializeOctree(iAACubed(iaVector3d(), 10000));
    getView().setScene(_entityScene);

    // setup camera
    // we want a camera which can be rotated around the origin
    // we will achieve that with 3 entities
    // one is for the heading
    iEntityPtr cameraHeading = _entityScene->createEntity("camera_heading");
    _cameraHeading = cameraHeading->getID();
    cameraHeading->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    // one is for the pitch
    iEntityPtr cameraPitch = _entityScene->createEntity("camera_pitch");
    _cameraPitch = cameraPitch->getID();
    cameraPitch->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    // and the third is the camera including translation away from the origin
    iEntityPtr camera = _entityScene->createEntity("camera");
    _camera = camera->getID();
    camera->addComponent(new iTransformComponent(iaVector3d(0, 0, spacing * amountPerDimension * 0.5)));
    auto cameraComponent = camera->addComponent(new iCameraComponent());
    cameraComponent->setPerspective(45.0);
    cameraComponent->setClipPlanes(0.1, 10000.0);
    cameraComponent->setClearColorActive(true);
    cameraComponent->setClearDepthActive(true);
    // link them together
    cameraPitch->setParent(cameraHeading);
    camera->setParent(cameraPitch);

    iPerlinNoise perlinNoise;
    perlinNoise.generateBase(42);
    iaRandomNumberGenerator random;

    // load a bunch of models and put them in the scene
    iModelPtr modelCat = iResourceManager::getInstance().loadResource<iModel>("example_model_cat");
    iModelPtr modelCrate = iResourceManager::getInstance().loadResource<iModel>("example_model_crate");
    iModelPtr modelCube = iResourceManager::getInstance().loadResource<iModel>("example_model_cube_green");
    iModelPtr modelTeapot = iResourceManager::getInstance().loadResource<iModel>("example_model_teapot");

    const iaVector3d offset(-spacing * amountPerDimension * 0.5, -spacing * amountPerDimension * 0.5, -spacing * amountPerDimension * 0.5);

    for (int z = 0; z < amountPerDimension; ++z)
    {
        for (int y = 0; y < amountPerDimension; ++y)
        {
            for (int x = 0; x < amountPerDimension; ++x)
            {
                float32 noise = perlinNoise.getValue(iaVector3d(x * 0.1, y * 0.1, z * 0.1), 3);
                if (noise < 0.62)
                {
                    continue;
                }

                const iaVector3d position(static_cast<float64>(x) * spacing + random.getNextFloatRange(0.0, spacing),
                                          static_cast<float64>(y) * spacing + random.getNextFloatRange(0.0, spacing),
                                          static_cast<float64>(z) * spacing + random.getNextFloatRange(0.0, spacing));

                const iaVector3d orientation(random.getNextFloat() * M_PI * 2,
                                             random.getNextFloat() * M_PI * 2,
                                             random.getNextFloat() * M_PI * 2);

                iEntityPtr entity = _entityScene->createEntity();
                entity->addComponent(new iTransformComponent(position + offset, orientation));
                entity->addComponent(new iSphereComponent(1));
                entity->addComponent(new iOctreeComponent());

                switch (random.getNextRange(4))
                {
                case 0:
                    entity->addComponent(new iMeshReferenceComponent(modelCat));
                    break;

                case 1:
                    entity->addComponent(new iMeshReferenceComponent(modelCrate));
                    break;

                case 2:
                    entity->addComponent(new iMeshReferenceComponent(modelCube));
                    break;

                case 3:
                    entity->addComponent(new iMeshReferenceComponent(modelTeapot));
                    break;
                }

                entity->addComponent(new iMeshRenderComponent());
            }
        }
    }

    // add the sun
    iEntityPtr sun = _entityScene->createEntity("sun_light");
    sun->addComponent(new iTransformComponent(iaVector3d(1, 1, 1)));
    sun->addComponent(new iLightComponent());

    iResourceManager::getInstance().saveResource(scenePrefab, "/home/martin/dev/Igor/examples/11_Instancing/project/scenes/main.scene");
}

void ExampleInstancing::onDeinit()
{
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
    iEntityPtr camera = _entityScene->getEntity(_camera);
    auto transformComponent = camera->getComponent<iTransformComponent>();

    if (event.getWheelDelta() < 0)
    {
        transformComponent->translate(iaVector3d(0.0, 0.0, 2.0));
    }
    else
    {
        transformComponent->translate(iaVector3d(0.0, 0.0, -2.0));
    }

    return true;
}

bool ExampleInstancing::onMouseMoveEvent(iEventMouseMove &event)
{
    if (iMouse::getInstance().getLeftButton())
    {
        const auto from = event.getLastPosition();
        const auto to = event.getPosition();

        iEntityPtr cameraPitch = _entityScene->getEntity(_cameraPitch);
        iEntityPtr cameraHeading = _entityScene->getEntity(_cameraHeading);
        auto transformPitch = cameraPitch->getComponent<iTransformComponent>();
        auto transformHeading = cameraHeading->getComponent<iTransformComponent>();

        transformPitch->rotate(iaVector3d((from._y - to._y) * 0.0005f, 0.0, 0.0));
        transformHeading->rotate(iaVector3d(0.0, (from._x - to._x) * 0.0005f, 0.0));
        iMouse::getInstance().setCenter();

        return true;
    }

    return false;
}
