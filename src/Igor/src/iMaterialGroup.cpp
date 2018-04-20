// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialGroup.h>

#include <iNodeRender.h>
#include <iNodeMesh.h>
#include <iMesh.h>
#include <iNodeFactory.h>
#include <iMaterialResourceFactory.h>

namespace Igor
{
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

    vector<uint64> iMaterialGroup::getRenderNodes() const
    {
        return _renderNodeIDs;
    }

    map<shared_ptr<iMeshBuffers>, iInstancedNodes> iMaterialGroup::getInstancedRenderNodes() const
    {
        return _instancedRenderNodes;
    }

    void iMaterialGroup::addRenderNode(uint64 renderNodeID, bool instancing)
    {
        iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
        if (renderNode != nullptr)
        {
            if (instancing && 
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

    void iMaterialGroup::removeRenderNode(uint64 renderNodeID, bool instancing)    {        iNodeRender* renderNode = static_cast<iNodeRender*>(iNodeFactory::getInstance().getNode(renderNodeID));
        if (renderNode != nullptr)        {            if (instancing &&                iNodeType::iNodeMesh == renderNode->getType())            {                auto meshBuffers = static_cast<iNodeMesh*>(renderNode)->getMeshBuffers();                auto iter = find(_instancedRenderNodes[meshBuffers]._renderNodeIDs.begin(), _instancedRenderNodes[meshBuffers]._renderNodeIDs.end(), renderNodeID);                if (iter != _instancedRenderNodes[meshBuffers]._renderNodeIDs.end())                {                    _instancedRenderNodes[meshBuffers]._renderNodeIDs.erase(iter);                }            }            else            {                auto iter = find(_renderNodeIDs.begin(), _renderNodeIDs.end(), renderNodeID);                if (iter != _renderNodeIDs.end())                {                    _renderNodeIDs.erase(iter);                }            }        }    }
}


