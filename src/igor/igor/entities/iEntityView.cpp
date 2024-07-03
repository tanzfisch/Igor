// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityView.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace igor
{
	void iEntityView::removeEntity(iEntityPtr entity)
	{
		auto iter = std::find(_entities.begin(), _entities.end(), entity);
		if (iter != _entities.end())
		{
			_entities.erase(iter);
			return;
		}

		iter = std::find(_inactiveEntities.begin(), _inactiveEntities.end(), entity);
		if (iter != _inactiveEntities.end())
		{
			_inactiveEntities.erase(iter);
		}
	}

	void iEntityView::onEntityChanged(iEntityPtr entity)
	{
		const bool compatible = checkCompatibility(entity);		

		// need to remove?
		if (!compatible)
		{
			removeEntity(entity);
			return;
		}

		const bool isActive = entity->isActive();

		if(isActive)
		{
			auto activeIter = std::find(_entities.begin(), _entities.end(), entity);
			if (activeIter != _entities.end())
			{
				return;
			}

			auto inactiveIter = std::find(_inactiveEntities.begin(), _inactiveEntities.end(), entity);
			if (inactiveIter != _inactiveEntities.end())
			{
				_inactiveEntities.erase(inactiveIter);
			}

			_entities.emplace_back(entity);
		}
		else
		{
			auto inactiveIter = std::find(_inactiveEntities.begin(), _inactiveEntities.end(), entity);
			if (inactiveIter != _inactiveEntities.end())
			{
				return;
			}

			auto activeIter = std::find(_entities.begin(), _entities.end(), entity);
			if (activeIter != _entities.end())
			{
				_entities.erase(activeIter);
			}

			_inactiveEntities.emplace_back(entity);
		}

	}

	iEntityComponentMask iEntityView::calcComponentMask(const std::vector<std::type_index> &types)
	{
		return iEntity::calcComponentMask(types);
	}

	bool iEntityView::checkCompatibility(iEntityPtr entity) const
	{
		const auto hash = _componentMask & entity->getTypeHash();
		return hash == _componentMask;
	}

	std::vector<iEntityPtr> &iEntityView::getEntities()
	{
		return _entities;
	}

	std::vector<iEntityPtr> &iEntityView::getInactiveEntities()
	{
		return _inactiveEntities;
	}
}