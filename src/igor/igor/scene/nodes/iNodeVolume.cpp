// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
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

		_sphere = node->_sphere;
		_bbox = node->_bbox;
	}

	iNodeVolume::~iNodeVolume()
	{
		setScene(nullptr);
	}

	const iAABoxd &iNodeVolume::getBoundingBox() const
	{
		return _bbox;
	}

	const iSphered &iNodeVolume::getBoundingSphere() const
	{
		return _sphere;
	}

	void iNodeVolume::setBoundingBox(const iAABoxd &bbox)
	{
		_bbox = bbox;
	}

	void iNodeVolume::setBoundingSphere(const iSphered &sphere)
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

	void iNodeVolume::onUpdateTransform(iaMatrixd &matrix)
	{
		if (_worldMatrix != matrix)
		{
			_worldMatrix = matrix;
			updateTree();
		}
	}

} // namespace igor
