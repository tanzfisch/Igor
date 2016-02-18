// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemScenePositionUpdate.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>
#include <iEntityManager.h>
#include <iEntityDataPosition.h>
#include <iEntityDataTransformation.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    uint64 iSystemScenePositionUpdate::getDataMask()
    {
        return static_cast<uint64>(iEntityDataMask::Position) | static_cast<uint64>(iEntityDataMask::TransformNode);
    }

    void iSystemScenePositionUpdate::handle(uint64 entityID)
    {
        iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(_entityDataTransformation->getTransformID(entityID)));

        if (transformNode != nullptr)
        {
            iaMatrixf matrix;
            transformNode->getMatrix(matrix);

            _entityDataPosition->updatePosition(entityID, matrix._pos);
        }
    }

    void iSystemScenePositionUpdate::onRegistration()
    {
        _entityDataPosition = static_cast<iEntityDataPosition*>(iEntityManager::getInstance().getEntityData(static_cast<uint64>(iEntityDataMask::Position)));
        _entityDataTransformation = static_cast<iEntityDataTransformation*>(iEntityManager::getInstance().getEntityData(static_cast<uint64>(iEntityDataMask::TransformNode)));

        con_assert(_entityDataPosition != nullptr, "zero pointer");
        con_assert(_entityDataTransformation != nullptr, "zero pointer");
    }


}