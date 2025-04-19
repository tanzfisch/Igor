// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "Example3D.h"

Example3D::Example3D(iWindowPtr window)
    : ExampleBase(window, "3D Scene", true, "igor_skybox_debug")
{
}

void Example3D::onInit()
{

#if 0
    iPrefabPtr scenePrefab = iResourceManager::getInstance().createResource<iPrefab>();
    _entityScene = iEntitySystemModule::getInstance().getScene(scenePrefab->getSceneID());
    _entityScene->initializeOctree(iAACubed(iaVector3d(), 10000));
    getView().setEntityScene(_entityScene);

    iEntityPtr cameraHeading = _entityScene->createEntity("cameraHeading");
    _cameraHeading = cameraHeading->getID();
    cameraHeading->addComponent(new iTransformComponent(iaVector3d(0, 0, 0.0)));

    iEntityPtr cameraPitch = _entityScene->createEntity("cameraPitch");
    _cameraPitch = cameraPitch->getID();
    cameraPitch->addComponent(new iTransformComponent(iaVector3d(0, 0, 0.0)));
    cameraPitch->setParent(cameraHeading);

    iEntityPtr camera = _entityScene->createEntity("camera");
    _camera = camera->getID();
    camera->addComponent(new iTransformComponent(iaVector3d(0, 0, 3.0)));
    auto cameraComponent = camera->addComponent(new iCameraComponent());
    cameraComponent->setPerspective(45.0);
    cameraComponent->setClipPlanes(0.01, 100.0);
    cameraComponent->setClearColorActive(true);
    cameraComponent->setClearDepthActive(true);
    camera->setParent(cameraPitch);

    iEntityPtr cat = _entityScene->createEntity("cat");
    cat->addComponent(new iTransformComponent(iaVector3d(0, -0.3, -0.25)));
    cat->addComponent(new iSphereComponent(1));
    cat->addComponent(new iOctreeComponent());

    iModelPtr modelCat = iResourceManager::getInstance().requestResource<iModel>("example_model_cat");
    cat->addComponent(new iMeshReferenceComponent(modelCat, "cat"));
    cat->addComponent(new iMeshRenderComponent());

    iResourceManager::getInstance().saveResource(scenePrefab, "/home/martin/dev/Igor/examples/04_3D/project/scenes/main.scene");
#else
    getView().setEntityScene(iProject::getInstance().getProjectScene());

    _cameraPitch = iEntityID(0x1cab7c99336dbea8);
    _cameraHeading = iEntityID(0x494714df579bf91e);
    _camera = iEntityID(0x55192542cbb0c27d);
#endif
}

void Example3D::onEvent(iEvent &event)
{
    auto scene = iProject::getInstance().getProjectScene();

    if(getView().getEntityScene() != scene)
    {
        getView().setEntityScene(scene);
    }

    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventMouseMove>(IGOR_BIND_EVENT_FUNCTION(Example3D::onMouseMoveEvent));
    event.dispatch<iEventMouseWheel>(IGOR_BIND_EVENT_FUNCTION(Example3D::onMouseWheelEvent));
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Example3D::onKeyDown));
}

bool Example3D::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
    {
        iEntityPrintTraverser print;
        auto scene = iProject::getInstance().getProjectScene();
        print.traverse(scene);
    }
        return true;
    }

    return false;
}

bool Example3D::onMouseMoveEvent(iEventMouseMove &event)
{
    auto scene = iProject::getInstance().getProjectScene();

    const auto from = event.getLastPosition();
    const auto to = event.getPosition();

    if (iMouse::getInstance().getLeftButton())
    {
        auto pitch = scene->getEntity(_cameraPitch)->getComponent<iTransformComponent>();
        auto heading = scene->getEntity(_cameraHeading)->getComponent<iTransformComponent>();

        pitch->rotate(iaVector3d((to._y - from._y) * 0.005f, 0.0, 0.0));
        heading->rotate(iaVector3d(0.0, (to._x - from._x) * 0.005f, 0.0));

        iMouse::getInstance().setCenter();
    }

    return true;
}

bool Example3D::onMouseWheelEvent(iEventMouseWheel &event)
{
    auto scene = iProject::getInstance().getProjectScene();
    auto translation = scene->getEntity(_camera)->getComponent<iTransformComponent>();

    if (event.getWheelDelta() < 0)
    {
        translation->translate(iaVector3d(0, 0, 1));
    }
    else
    {
        translation->translate(iaVector3d(0, 0, -1));
    }

    return true;
}

iaString Example3D::getHelpString()
{
    iaString help = "\n"
                    "[MouseWheel] Zoom\n"
                    "[LeftMouseDrag] Rotate cam\n";

    iaString result = ExampleBase::getHelpString();
    result += help;

    return result;
}