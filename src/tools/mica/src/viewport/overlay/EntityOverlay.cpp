// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "EntityOverlay.h"

EntityOverlay::EntityOverlay(iViewPtr view)
    : _view(view)
{
}

void EntityOverlay::setOverlayMode(OverlayMode mode)
{
    _overlayMode = mode;

    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    if(scene == nullptr)
    {
        return;
    }

    auto entity = scene->getEntity(_entityID);
    if(entity == nullptr)
    {
        return;
    }

    setActive(accepts(_overlayMode, entity));
}

OverlayMode EntityOverlay::getOverlayMode() const
{
    return _overlayMode;
}

void EntityOverlay::setActive(bool active)
{
    _active = active;
}

bool EntityOverlay::isActive() const
{
    return _active;
}

bool EntityOverlay::onMouseKeyDownEvent(iEventMouseKeyDown &event)
{
    return false;
}

bool EntityOverlay::onMouseKeyUpEvent(iEventMouseKeyUp &event)
{
    return false;
}

bool EntityOverlay::onMouseMoveEvent(iEventMouseMove &event)
{
    return false;
}

bool EntityOverlay::onKeyDown(iEventKeyDown &event)
{
    return false;
}

iViewPtr EntityOverlay::getView() const
{
    return _view;
}

void EntityOverlay::setEntity(const iEntitySceneID &entitySceneID, const iEntityID &entityID)
{
    _entityID = entityID;
    _entitySceneID = entitySceneID;

    auto scene = iEntitySystemModule::getInstance().getScene(_entitySceneID);
    if(scene == nullptr)
    {
        return;
    }

    auto entity = scene->getEntity(_entityID);
    if(entity == nullptr)
    {
        return;
    }

    setActive(accepts(_overlayMode, entity));
}

void EntityOverlay::resetEntity()
{
    _entityID = iEntityID::getInvalid();
    _entitySceneID = iEntitySceneID::getInvalid();
    setActive(false);
}

const iEntityID &EntityOverlay::getEntityID() const
{
    return _entityID;
}

const iEntityID &EntityOverlay::getSceneID() const
{
    return _entitySceneID;
}

iMeshPtr EntityOverlay::createTranslateMesh()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertices(false);

    iaMatrixf matrix;

    matrix.translate(0, 1.5, 0);
    matrix.scale(0.1, 0.5, 0.1);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCone(meshBuilder, 1, 1, 6);

    matrix.identity();
    matrix.scale(0.025, 1.5, 0.025);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

    meshBuilder.calcNormals(true);

    return meshBuilder.createMesh();
}

iMeshPtr EntityOverlay::createCylinder()
{
    iMeshBuilder meshBuilder;
    meshBuilder.setJoinVertices(true);

    iaMatrixf matrix;
    matrix.scale(0.025, 1.5, 0.025);
    meshBuilder.setMatrix(matrix);
    iMeshBuilderUtils::addCylinder(meshBuilder, 1, 1, 6);

    meshBuilder.calcNormals(true);

    return meshBuilder.createMesh();
}

iMeshPtr EntityOverlay::createRingMesh()
{
    iMeshBuilder meshBuilder;
    iMeshBuilderUtils::addCylinder(meshBuilder, 2.0, 0.13, 64, false);
    meshBuilder.calcNormals(true);
    return meshBuilder.createMesh();
}