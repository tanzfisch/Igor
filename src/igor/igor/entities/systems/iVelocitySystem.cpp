// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iVelocitySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iVelocityComponent.h>
#include <igor/entities/components/iGlobalBoundaryComponent.h>

namespace igor
{
	iVelocitySystem::iVelocitySystem()
	{
		_noBoundsView = createView<iVelocityComponent, iTransformComponent>();
		_boundsView = createView<iVelocityComponent, iTransformComponent, iGlobalBoundaryComponent>();
	}

	iEntitySystemStage iVelocitySystem::getStage() const
	{
		return iEntitySystemStage::Update;
	}

	void iVelocitySystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		for (auto entity : _noBoundsView->getEntities())
		{
			auto velocityComp = entity->getComponent<iVelocityComponent>();
			auto transform = entity->getComponent<iTransformComponent>();

			transform->translate(velocityComp->getVelocity());
			transform->rotate(velocityComp->getAngularVelocity());
		}

		iaVector3d min;
		iaVector3d max;
		_bounds.getMinMax(min, max);
		const iaVector3d dimensions = _bounds._halfWidths * 2.0;

		for (auto entity : _boundsView->getEntities())
		{
			auto velocityComp = entity->getComponent<iVelocityComponent>();
			auto transform = entity->getComponent<iTransformComponent>();
			auto bounds = entity->getComponent<iGlobalBoundaryComponent>();

			auto position = transform->getPosition();

			transform->rotate(velocityComp->getAngularVelocity());

			switch (bounds->getType())
			{
			case iGlobalBoundaryType::Repeat:

				position += velocityComp->getVelocity();

				if (position._x > max._x)
				{
					position._x -= dimensions._x;
				}
				if (position._x < min._x)
				{
					position._x += dimensions._x;
				}

				if (position._y > max._y)
				{
					position._y -= dimensions._y;
				}
				if (position._y < min._y)
				{
					position._y += dimensions._y;
				}

				if (position._z > max._z)
				{
					position._z -= dimensions._z;
				}
				if (position._z < min._z)
				{
					position._z += dimensions._z;
				}
				break;

			case iGlobalBoundaryType::None:
			default:
				position += velocityComp->getVelocity();
				break;
			}

			transform->setPosition(position);
		}
	}

	void iVelocitySystem::setBounds(const iAABoxd &box)
	{
		_bounds = box;
	}

	const iAABoxd &iVelocitySystem::getBounds() const
	{
		return _bounds;
	}

} // igor