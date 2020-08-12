// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeRender.h>

#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/scene/iScene.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeRender::iNodeRender()
        : iNode()
    {
        _materialID = iMaterialResourceFactory::getInstance().getDefaultMaterialID();

        setName(L"iNodeRender");
        _nodeType = iNodeType::iNodeRender;
        _nodeKind = iNodeKind::Renderable;
    }

    iNodeRender::iNodeRender(iNodeRender *node)
        : iNode()
    {
        _materialID = node->_materialID;
        _visible = node->_visible;
    }

    iNodeRender::~iNodeRender()
    {
        setScene(nullptr);
    }

    void iNodeRender::setVisible(bool visible)
    {
        _visible = visible;
    }

    void iNodeRender::onPreSetScene()
    {
        if (_nodeKind == iNodeKind::Renderable &&
            _scene)
        {
            _scene->unregisterRenderable(this);
        }
    }

    void iNodeRender::onPostSetScene()
    {
        if (_nodeKind == iNodeKind::Renderable &&
            _scene)
        {
            _scene->registerRenderable(this);
        }
    }

    void iNodeRender::onUpdateTransform(iaMatrixd &matrix)
    {
        _worldMatrix = matrix;
    }

    void iNodeRender::setMaterial(uint64 materialID)
    {
        if (_materialID != materialID)
        {
            _materialID = materialID;
        }
    }

}; // namespace igor
