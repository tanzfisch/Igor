// Igor game engine
// (c) Copyright 2012-2019 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeRender.h>

#include <iMaterialResourceFactory.h>
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

    bool iNodeRender::isVisible() const
    {
        return _visible;
    }

    bool iNodeRender::wasReached()
    {
        return _reached;
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

    iaMatrixd& iNodeRender::getWorldMatrix()
    {
        return _worldMatrix;
    }

    void iNodeRender::onUpdateTransform(iaMatrixd& matrix)
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

    uint64 iNodeRender::getMaterial()
    {
        return _materialID;
    }


};
