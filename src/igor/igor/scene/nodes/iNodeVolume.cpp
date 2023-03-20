// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/scene/nodes/iNodeVolume.h>
#include <igor/scene/iScene.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

    iNodeVolume::iNodeVolume()
        : iNodeRender()
    {
        _nodeKind = iNodeKind::Volume;
    }

    iNodeVolume::iNodeVolume(iNodeVolume *node)
        : iNodeRender(node)
    {
        con_assert(node != nullptr, "zero pointer");

        _bbox = node->_bbox;
        calcActualBBox();
    }

    iNodeVolume::~iNodeVolume()
    {
        setScene(nullptr);
    }

    const iAABoxd &iNodeVolume::getBoundingBox() const
    {
        return _bbox;
    }

    void iNodeVolume::setBoundingBox(const iAABoxd &bbox)
    {
        _bbox = bbox;
        calcActualBBox();
    }

    const iSphered &iNodeVolume::getBoundingSphere() const
    {
        return _sphere;
    }

    void iNodeVolume::onPreSetScene()
    {
        iNodeRender::onPreSetScene();

        if (getScene())
        {
            getScene()->unregisterVolume(this);
        }
    }

    void iNodeVolume::onPostSetScene()
    {
        iNodeRender::onPostSetScene();

        if (getScene())
        {
            getScene()->registerVolume(this);
        }
    }

    iaVector3d iNodeVolume::getCenter() const
    {
        return _worldMatrix._pos + _bbox._center;
    }

    const iaVector3d &iNodeVolume::getRelativeCenter() const
    {
        return _bbox._center;
    }

    void iNodeVolume::updateTree()
    {
        if (getScene())
        {
            getScene()->updateVolume(this);
        }
    }

    void iNodeVolume::onUpdateTransform(iaMatrixd &matrix)
    {
        if (_worldMatrix != matrix)
        {
            _worldMatrix = matrix;
            calcActualBBox();
            updateTree();
        }
    }

    void iNodeVolume::calcActualBBox()
    {
        _actualbbox._center = _bbox._center;
        _actualbbox._halfWidths._x = _bbox._halfWidths._x * _worldMatrix._right.length();
        _actualbbox._halfWidths._y = _bbox._halfWidths._y * _worldMatrix._top.length();
        _actualbbox._halfWidths._z = _bbox._halfWidths._z * _worldMatrix._depth.length();

        _sphere._center = _actualbbox._center;
        _sphere._radius = std::max(_actualbbox._halfWidths._x, std::max(_actualbbox._halfWidths._y, _actualbbox._halfWidths._z));
    }

} // namespace igor
