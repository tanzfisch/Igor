// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
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

    const iSpheref& iNodeVolume::getBoundingSphere() const
    {
        return _sphere;
    }

    void iNodeVolume::setBoundingSphere(const iSpheref& sphere)
    {
        _sphere._center = sphere._center;
        _sphere._radius = sphere._radius;
    }

	void iNodeVolume::onPreSetScene()
	{
		if (getScene())
		{
			getScene()->unregisterVolume(this);
		}
	}

	void iNodeVolume::onPostSetScene()
	{
		if (getScene())
		{
			getScene()->registerVolume(this);
		}
	}

	iaVector3f iNodeVolume::getCenter() const
	{
        return _worldMatrix._pos + _sphere._center;
	}

    iaVector3f iNodeVolume::getRelativeCenter() const
    {
        return _sphere._center;
    }

	void iNodeVolume::update()
	{
		if (getScene())
		{
			getScene()->updateVolume(this);
		}	
	}

    void iNodeVolume::onUpdateTransform(iaMatrixf& matrix)
    {
        if (_worldMatrix != matrix)
        {
            _worldMatrix = matrix;
            update();
        }
    }

}

