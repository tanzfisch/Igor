// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeRender.h>

#include <igor/scene/iScene.h>
#include <igor/resources/iResourceManager.h>


#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{

    iNodeRender::iNodeRender()
        : iNode()
    {
        _material = iResourceManager::getInstance().createResource<iMaterial>();

        setName(L"iNodeRender");
        _nodeType = iNodeType::iNodeRender;
        _nodeKind = iNodeKind::Renderable;
    }

    iNodeRender::iNodeRender(iNodeRender *node)
        : iNode()
    {
        _material = node->_material;
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

    void iNodeRender::setMaterial(const iMaterialPtr &material)
    {
        _material = material;
    }

    bool iNodeRender::isVisible() const
    {
        return _visible;
    }

    iMaterialPtr iNodeRender::getMaterial() const
    {
        return _material;
    }

    const iaMatrixd &iNodeRender::getWorldMatrix() const
    {
        return _worldMatrix;
    }

}; // namespace igor
