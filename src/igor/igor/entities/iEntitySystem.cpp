// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntitySystem.h>

#include <igor/entities/iEntityScene.h>

#include <algorithm>

namespace igor
{

	bool iEntitySystem::checkCompatibility(iEntityPtr entity) const
	{
		con_assert(!_supportedTypes.empty(), "no supported types");
		
		const auto &components = entity->_components;

		for (const auto &supportedType : _supportedTypes)
		{
			const auto iter = components.find(supportedType);
			if (iter == components.end())
			{
				return false;
			}
		}

		return true;
	}

	const std::vector<iEntityPtr>& iEntitySystem::getEntities() const
	{
		return _entities;
	}

	void iEntitySystem::onComponentsChanged(iEntityPtr entity)
	{
		bool compatible = checkCompatibility(entity);

		auto iter = std::find(_entities.begin(), _entities.end(), entity);

		if(iter != _entities.end() && !compatible)
		{
			_entities.erase(iter);
			return;
		}

		if(iter == _entities.end() && compatible)
		{
			_entities.push_back(entity);
		}
	}

}