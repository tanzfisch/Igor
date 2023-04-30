// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iGlobalBoundarySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
	void iGlobalBoundarySystem::update(iEntityScenePtr scene)
	{
		// TODO
	}

	void iGlobalBoundarySystem::setBounds(const iAABoxd &box)
	{
		_bounds = box;
	}

	const iAABoxd &iGlobalBoundarySystem::getBounds() const
	{
		return _bounds;
	}

} // igor