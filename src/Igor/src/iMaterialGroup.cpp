// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iMaterialGroup.h>

#include <iNodeRender.h>
#include <iNodeMesh.h>
#include <iMesh.h>

namespace Igor
{

	uint32 iMaterialGroup::_nextId = 1;

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

    void iMaterialGroup::removeRenderNode(iNodeRender* renderNode)
    {
        if (iRenderStateValue::On == _material.getRenderStateSet().getRenderStateValue(iRenderState::Instanced) &&
            iNodeType::iNodeMesh == renderNode->getType())
        {
            auto meshBuffers = static_cast<iNodeMesh*>(renderNode)->getMeshBuffers();
            auto iter = find(_instancedRenderNodes[meshBuffers]._nodes.begin(), _instancedRenderNodes[meshBuffers]._nodes.end(), renderNode);
            if (iter != _instancedRenderNodes[meshBuffers]._nodes.end())
            {
                _instancedRenderNodes[meshBuffers]._nodes.erase(iter);
            }
        }
        else
        {
            auto iter = find(_renderNodes.begin(), _renderNodes.end(), renderNode);
            if (iter != _renderNodes.end())
            {
                _renderNodes.erase(iter);
            }
        }
    }

    void iMaterialGroup::addRenderNode(iNodeRender* renderNode)
    {
        if (iRenderStateValue::On == _material.getRenderStateSet().getRenderStateValue(iRenderState::Instanced) &&
            iNodeType::iNodeMesh == renderNode->getType())
        {
            auto meshBuffers = static_cast<iNodeMesh*>(renderNode)->getMeshBuffers();
            if (_instancedRenderNodes[meshBuffers]._instancer == nullptr)
            {
                _instancedRenderNodes[meshBuffers]._instancer = new iInstancer(64, 10000);
            }

            auto iter = find(_instancedRenderNodes[meshBuffers]._nodes.begin(), _instancedRenderNodes[meshBuffers]._nodes.end(), renderNode);
            if (_instancedRenderNodes[meshBuffers]._nodes.end() == iter)
            {
                _instancedRenderNodes[meshBuffers]._nodes.push_back(static_cast<iNodeMesh*>(renderNode));
            }
        }
        else
        {
            auto iter = find(_renderNodes.begin(), _renderNodes.end(), renderNode);
            if (_renderNodes.end() == iter)
            {
                _renderNodes.push_back(renderNode);
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

	uint32 iMaterialGroup::getID()
	{
		return _id;
	}

}


