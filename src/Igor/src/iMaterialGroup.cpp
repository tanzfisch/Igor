// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialGroup.h>

#include <iNodeRender.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeFactory.h>

namespace Igor
{

    uint64 iMaterialGroup::_nextId = 1;

    iMaterialGroup::iMaterialGroup()
    {
        _id = _nextId++;
    }

    iMaterialGroup::~iMaterialGroup()
    {
        for (auto iter : _instancedRenderNodes)
        {
            if (iter.second._instancer != nullptr)
            {
                delete iter.second._instancer;
            }
        }
    }

    void iMaterialGroup::removeRenderNode(uint32 renderNodeID)
    {
        iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
        if (renderNode != nullptr)
        {
            if (iRenderStateValue::On == _material.getRenderStateSet().getRenderStateValue(iRenderState::Instanced) &&
                iNodeType::iNodeMesh == renderNode->getType())
            {
                auto meshBuffers = static_cast<iNodeMesh*>(renderNode)->getMeshBuffers();
                auto iter = find(_instancedRenderNodes[meshBuffers]._renderNodeIDs.begin(), _instancedRenderNodes[meshBuffers]._renderNodeIDs.end(), renderNodeID);
                if (iter != _instancedRenderNodes[meshBuffers]._renderNodeIDs.end())
                {
                    _instancedRenderNodes[meshBuffers]._renderNodeIDs.erase(iter);
                }
            }
            else
            {
                auto iter = find(_renderNodeIDs.begin(), _renderNodeIDs.end(), renderNodeID);
                if (iter != _renderNodeIDs.end())
                {
                    _renderNodeIDs.erase(iter);
                }
            }
        }
    }

    void iMaterialGroup::addRenderNode(uint32 renderNodeID)
    {
        iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
        if (renderNode != nullptr)
        {
            if (iRenderStateValue::On == _material.getRenderStateSet().getRenderStateValue(iRenderState::Instanced) &&
                iNodeType::iNodeMesh == renderNode->getType())
            {
                auto meshBuffers = static_cast<iNodeMesh*>(renderNode)->getMeshBuffers();
                if (_instancedRenderNodes[meshBuffers]._instancer == nullptr)
                {
                    _instancedRenderNodes[meshBuffers]._instancer = new iInstancer(64, 10000); // big TODO 
                }

                auto iter = find(_instancedRenderNodes[meshBuffers]._renderNodeIDs.begin(), _instancedRenderNodes[meshBuffers]._renderNodeIDs.end(), renderNodeID);
                if (_instancedRenderNodes[meshBuffers]._renderNodeIDs.end() == iter)
                {
                    _instancedRenderNodes[meshBuffers]._renderNodeIDs.push_back(renderNodeID);
                }
            }
            else
            {
                auto iter = find(_renderNodeIDs.begin(), _renderNodeIDs.end(), renderNodeID);
                if (_renderNodeIDs.end() == iter)
                {
                    _renderNodeIDs.push_back(renderNodeID);
                }
            }
        }
    }

    iMaterial* iMaterialGroup::getMaterial()
    {
        return &_material;
    }

    int32 iMaterialGroup::getOrder()
    {
        return _material.getOrder();
    }

    void iMaterialGroup::setOrder(int32 order)
    {
        _material.setOrder(order);
    }

    uint64 iMaterialGroup::getID()
    {
        return _id;
    }

}


