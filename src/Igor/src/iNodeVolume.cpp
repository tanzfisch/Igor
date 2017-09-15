// Igor game engine
// (c) Copyright 2012-2017 by Martin Loga
// see copyright notice in corresponding header file

#include <iNodeVolume.h>
#include <iScene.h>
#include <iaConsole.h>

namespace Igor
{

	iNodeVolume::iNodeVolume()
        :iNodeRender()
	{
		_nodeKind = iNodeKind::Volume;
	}

    iNodeVolume::iNodeVolume(iNodeVolume* node)
        : iNodeRender(node)
    {
        con_assert(node != nullptr, "zero pointer");

        _sphere = node->_sphere;
    }

	iNodeVolume::~iNodeVolume()
	{
		setScene(nullptr);
	}

    const iAABoxd& iNodeVolume::getBoundingBox() const
    {
        iAABoxd result;
        result._center = _sphere._center;
        result._halfWidths._x = _sphere._radius;
        result._halfWidths._y = _sphere._radius;
        result._halfWidths._z = _sphere._radius;

        return result;
    }

    const iSphered& iNodeVolume::getBoundingSphere() const
    {
        return _sphere;
    }

    void iNodeVolume::setBoundingSphere(const iSphered& sphere)
    {
		if (_sphere._center != sphere._center ||
			_sphere._radius != sphere._radius)
		{
			_sphere._center = sphere._center;
			_sphere._radius = sphere._radius;

			updateTree();
		}
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
        return _worldMatrix._pos + _sphere._center;
	}

    iaVector3d iNodeVolume::getRelativeCenter() const
    {
        return _sphere._center;
    }

	void iNodeVolume::updateTree()
	{
		if (getScene())
		{
			getScene()->updateVolume(this);
		}	
	}

    void iNodeVolume::onUpdateTransform(iaMatrixd& matrix)
    {
        if (_worldMatrix != matrix)
        {
            _worldMatrix = matrix;
			updateTree();
        }
    }

    bool iNodeVolume::onUpdateData()
    {
        // does nothing
        return true;
    }

}

