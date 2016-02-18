// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemSceneTransformationUpdate.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iEntityManager.h>
#include <iEntityDataPosition.h>
#include <iEntityDataTransformation.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    uint64 iSystemSceneTransformationUpdate::getDataMask()
    {
        return static_cast<uint64>(iEntityDataMask::Position) | static_cast<uint64>(iEntityDataMask::TransformNode);
    }

    void iSystemSceneTransformationUpdate::handle(uint64 entityID)
    {
        iaVector3f position = _entityDataPosition->getPosition(entityID);
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_entityDataTransformation->getTransformID(entityID)));

        transformNode->setPosition(position);
    }

    void iSystemSceneTransformationUpdate::onRegistration()
    {
        _entityDataPosition = static_cast<iEntityDataPosition*>(iEntityManager::getInstance().getEntityData(static_cast<uint64>(iEntityDataMask::Position)));
        _entityDataTransformation = static_cast<iEntityDataTransformation*>(iEntityManager::getInstance().getEntityData(static_cast<uint64>(iEntityDataMask::TransformNode)));

        con_assert(_entityDataPosition != nullptr, "zero pointer");
        con_assert(_entityDataTransformation != nullptr, "zero pointer");
    }


}