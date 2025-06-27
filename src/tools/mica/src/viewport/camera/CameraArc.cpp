// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "CameraArc.h"

CameraArc::CameraArc(const iEntitySceneID &entitySceneID, const iEntityID &entityID)
    : _entitySceneID(entitySceneID)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");

    if(!entityID.isValid())
    {
        _entityID = scene->getRootEntity()->getID();
    }

    auto anchor = scene->getEntity(_entityID);
    con_assert(anchor != nullptr, "anchor not found");

    auto coi = scene->createEntity("mica_camera_arc_coi");
    coi->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    _cameraCOIID = coi->getID();

    auto heading = scene->createEntity("mica_camera_arc_heading");
    heading->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(iaVector3d(0, _cameraHeading, 0))));
    _cameraHeadingID = heading->getID();

    auto pitch = scene->createEntity("mica_camera_arc_pitch");
    pitch->addComponent(new iTransformComponent(iaVector3d(), iaQuaterniond::fromEuler(iaVector3d(_cameraPitch, 0, 0))));
    _cameraPitchID = pitch->getID();

    auto distance = scene->createEntity("mica_camera_arc_distance");
    distance->addComponent(new iTransformComponent(iaVector3d(0, 0, 50)));
    _cameraDistanceID = distance->getID();

    auto camera = scene->createEntity("mica_camera_arc_camera");
    camera->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    auto cameraComponent = camera->addComponent(new iCameraComponent());
    cameraComponent->setPerspective(45.0);
    cameraComponent->setClipPlanes(0.01, 100.0);
    cameraComponent->setClearColorActive(true);
    cameraComponent->setClearDepthActive(true);
    _cameraID = camera->getID();

    coi->setParent(anchor);
    heading->setParent(coi);
    pitch->setParent(heading);
    distance->setParent(pitch);
    camera->setParent(distance);
}

CameraArc::~CameraArc()
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    if(scene == nullptr)
    {
        return;
    }
    
    scene->destroyEntity(_cameraCOIID);
}

void CameraArc::getWorldTransformation(iaMatrixd &matrix) const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto camera = scene->getEntity(_cameraID);
    con_assert(camera != nullptr, "no cam found");
    iTransformComponentPtr transform = camera->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    matrix = transform->getWorldMatrix();
}

float64 CameraArc::getHeading() const
{
    return _cameraHeading;
}

void CameraArc::setHeading(float64 heading)
{
    _cameraHeading = heading;

    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraHeadingID);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setOrientation(iaQuaterniond::fromEuler(0, _cameraHeading, 0));
}

float64 CameraArc::getPitch() const
{
    return _cameraPitch;
}

void CameraArc::setPitch(float64 pitch)
{
    _cameraPitch = pitch;
    
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraPitchID);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setOrientation(iaQuaterniond::fromEuler(_cameraPitch, 0, 0));
}

float64 CameraArc::getDistance() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraDistanceID);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getPosition()._z;
}

void CameraArc::setDistance(float64 distance)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entityTransform = scene->getEntity(_cameraDistanceID);
    con_assert(entityTransform != nullptr, "no heading found");
    iTransformComponentPtr transform = entityTransform->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setPosition(iaVector3d(0, 0, distance));

    auto entityCamera = scene->getEntity(_cameraID);
    iCameraComponentPtr cameraComp = entityCamera->getComponent<iCameraComponent>();
    con_assert(cameraComp != nullptr, "no transform found");

    cameraComp->setClipPlanes(distance / 100.0, distance * 2);
}

void CameraArc::setCenterOfInterest(const iaVector3d &coi)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraCOIID);
    con_assert(entity != nullptr, "no coi found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setPosition(coi);    
}

iaVector3d CameraArc::getCenterOfInterest() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraCOIID);
    con_assert(entity != nullptr, "no coi found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getPosition();
}

    const iEntitySceneID CameraArc::getEntitySceneID() const
    {
        return _entitySceneID;
    }

    const iEntityID CameraArc::getCameraID() const
    {
        return _cameraID;
    }
