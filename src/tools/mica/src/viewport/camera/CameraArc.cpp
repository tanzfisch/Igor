// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "CameraArc.h"

CameraArc::CameraArc(const iEntitySceneID &entitySceneID, const iEntityID &entityID)
    : _entitySceneID(entitySceneID), _entityID(entityID)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");

    auto anchor = scene->getEntity(_entityID);
    con_assert(anchor != nullptr, "anchor not found");

    auto coi = scene->createEntity("mica_camera_arc_coi");
    coi->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    _cameraCOI = coi->getID();

    auto heading = scene->createEntity("mica_camera_arc_heading");
    heading->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    _cameraHeading = heading->getID();

    auto pitch = scene->createEntity("mica_camera_arc_pitch");
    pitch->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    _cameraPitch = pitch->getID();

    auto distance = scene->createEntity("mica_camera_arc_distance");
    distance->addComponent(new iTransformComponent(iaVector3d(0, 0, 10)));
    _cameraDistance = distance->getID();

    auto camera = scene->createEntity("mica_camera_arc_camera");
    camera->addComponent(new iTransformComponent(iaVector3d(0, 0, 0)));
    auto cameraComponent = camera->addComponent(new iCameraComponent());
    cameraComponent->setPerspective(45.0);
    cameraComponent->setClipPlanes(0.01, 100.0);
    cameraComponent->setClearColorActive(true);
    cameraComponent->setClearDepthActive(true);
    _camera = camera->getID();

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
    
    scene->destroyEntity(_cameraCOI);
}

void CameraArc::getWorldTransformation(iaMatrixd &matrix) const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto camera = scene->getEntity(_camera);
    con_assert(camera != nullptr, "no cam found");
    iTransformComponentPtr transform = camera->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    matrix = transform->getWorldMatrix();
}

float64 CameraArc::getHeading() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraHeading);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getOrientation().toEuler()._y;
}

void CameraArc::setHeading(float64 heading)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraHeading);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setOrientation(iaQuaterniond::fromEuler(0, heading, 0));
}

float64 CameraArc::getPitch() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraPitch);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getOrientation().toEuler()._x;
}

void CameraArc::setPitch(float64 pitch)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraPitch);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setOrientation(iaQuaterniond::fromEuler(pitch, 0, 0));
}

float64 CameraArc::getDistance() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraDistance);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getPosition()._z;
}

void CameraArc::setDistance(float64 distance)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraDistance);
    con_assert(entity != nullptr, "no heading found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setPosition(iaVector3d(0, 0, distance));
}

void CameraArc::setCenterOfInterest(const iaVector3d &coi)
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraCOI);
    con_assert(entity != nullptr, "no coi found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    transform->setPosition(coi);    
}

iaVector3d CameraArc::getCenterOfInterest() const
{
    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    con_assert(scene != nullptr, "no scene found");
    auto entity = scene->getEntity(_cameraCOI);
    con_assert(entity != nullptr, "no coi found");
    iTransformComponentPtr transform = entity->getComponent<iTransformComponent>();
    con_assert(transform != nullptr, "no transform found");

    return transform->getPosition();
}