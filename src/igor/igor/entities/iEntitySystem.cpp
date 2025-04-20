// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystem.h>

#include <igor/entities/iEntityScene.h>

#include <algorithm>

namespace igor
{

	void iEntitySystem::onEntityChanged(iEntityPtr entity)
	{
		for (auto view : _views)
		{
			view->onEntityChanged(entity);
		}
	}

	void iEntitySystem::onComponentAdded(iEntityPtr entity, const std::type_index &typeID)
	{
		// nothing to do
	}

	void iEntitySystem::onComponentRemoved(iEntityPtr entity, const std::type_index &typeID)
	{
		// nothing to do
	}

	void iEntitySystem::onComponentToRemove(iEntityPtr entity, const std::type_index &typeID)
	{
		// nothing to do
	}

	void iEntitySystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		// nothing to do
	}

}