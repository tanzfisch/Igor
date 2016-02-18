// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iSystemSceneTransformation.h>
#include <iSystemPosition.h>
#include <iNodeFactory.h>
#include <iNodeTransform.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
    iSystemSceneTransformation::iSystemSceneTransformation(iSystemPosition* systemPosition, iScene* scene)
    {
        con_assert_sticky(scene != nullptr, "zero pointer");
        con_assert_sticky(systemPosition != nullptr, "zero pointer");
        _scene = scene;
        _systemPosition = systemPosition;
    }

    iSystemSceneTransformation::~iSystemSceneTransformation()
    {

    }

    void iSystemSceneTransformation::handle()
    {
        for (auto iter : _transformNodes)
        {
            uint64 entityID = iter.first;
            if(_systemPosition->hasEntity(entityID))
            {
                iNodeTransform* transformNode = static_cast<iNodeTransform*>(iNodeFactory::getInstance().getNode(iter.second));
                if (transformNode != nullptr)
                {
                    transformNode->setPosition(_systemPosition->getPosition(entityID));
                }
            }
        }
    }

    void iSystemSceneTransformation::registerEntity(uint64 entityID)
    {
        con_assert(_transformNodes.find(entityID) == _transformNodes.end(), "already registered");

        if (_transformNodes.find(entityID) == _transformNodes.end())
        {
            _transformNodes[entityID] = iNode::INVALID_NODE_ID;
        }
    }

    void iSystemSceneTransformation::unregisterEntity(uint64 entityID)
    {
        con_assert(_transformNodes.find(entityID) != _transformNodes.end(), "entity id not found");

        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            _transformNodes.erase(iter);
        }
    }

    bool iSystemSceneTransformation::hasEntity(uint64 entityID)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void iSystemSceneTransformation::setTransformID(uint64 entityID, uint32 transformNodeID)
    {
        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            (*iter).second = transformNodeID;
        }
    }

    uint32 iSystemSceneTransformation::getTransformID(uint64 entityID) const
    {
        uint32 result = iNode::INVALID_NODE_ID;

        auto iter = _transformNodes.find(entityID);
        if (iter != _transformNodes.end())
        {
            result = (*iter).second;
        }

        return result;
    }


}