// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeRender.h>

#include <iMaterialResourceFactory.h>
#include <iMaterialGroup.h>
#include <iScene.h>

#include <iaConsole.h>
using namespace IgorAux;

namespace Igor
{
	
	iNodeRender::iNodeRender()
		: iNode()
	{
        _materialID = iMaterialResourceFactory::getInstance().getDefaultMaterialID();

		setName(L"iNodeRender");
		_nodeType = iNodeType::iNodeRender;
		_nodeKind = iNodeKind::Renderable;
	}

    iNodeRender::iNodeRender(iNodeRender* node)
        : iNode()
    {
        _materialID = node->_materialID;
    }

    iNodeRender::~iNodeRender()
    {
		setScene(nullptr);

        // TODO bad design iRenderEngine should do that
        iMaterialGroup* materialGroup = iMaterialResourceFactory::getInstance().getMaterialGroup(_materialID);
        if (materialGroup != nullptr)
        {
            materialGroup->removeRenderNode(this);
        }
    }

    bool iNodeRender::onUpdateData()
    {
        // does nothing
        return true;
    }

    bool iNodeRender::wasReached()
    {
        return _reached;
    }

    void iNodeRender::onPreSetScene()
    {
        if (_scene)
        {
            _scene->unregisterRenderable(this);
        }
    }

    void iNodeRender::onPostSetScene()
    {
        if (_scene)
        {
            _scene->registerRenderable(this);
        }
    }

    iaMatrixf& iNodeRender::getWorldMatrix()
	{
		return _worldMatrix;
	}

    void iNodeRender::onUpdateTransform(iaMatrixf& matrix)
	{
        _worldMatrix = matrix;
	}
	
    void iNodeRender::setMaterial(uint32 materialID)
	{
        if (_materialID != materialID)
        {
            // TODO bad design iRenderEngine should do that
            iMaterialGroup* materialGroup = iMaterialResourceFactory::getInstance().getMaterialGroup(_materialID);
            if (materialGroup != nullptr)
            {
                materialGroup->removeRenderNode(this);
            }

            _materialID = materialID;
        }
	}

	uint32 iNodeRender::getMaterial()
	{
        return _materialID;
	}


};
