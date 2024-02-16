// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystem.h>

#include <igor/entities/iEntityScene.h>

#include <algorithm>

namespace igor
{

	void iEntitySystem::onComponentsChanged(iEntityPtr entity)
	{
		for (auto set : _sets)
		{
			set->onComponentsChanged(entity);
		}
	}

}