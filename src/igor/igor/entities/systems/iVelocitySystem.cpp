// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iVelocitySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
	iVelocitySystem::iVelocitySystem()
	{
		_interactionResolverView = createView<iVelocityComponent, iBody2DComponent, iMotionInteractionResolverComponent>();
		_noBoundsView = createView<iVelocityComponent, iTransformComponent>();
		_boundsView = createView<iVelocityComponent, iTransformComponent, iGlobalBoundaryComponent>();
	}

    iEntitySystemStage iVelocitySystem::getStage() const
    {
        return iEntitySystemStage::Update;
    }	

	void iVelocitySystem::update(const iaTime &time, iEntityScenePtr scene)
	{
		if (scene->hasQuadtree())
		{
			auto &quadtree = scene->getQuadtree();

			for (auto entity : _interactionResolverView->getEntities())
			{
				auto velocity = entity->getComponent<iVelocityComponent>();
				auto quadComp = entity->getComponent<iBody2DComponent>();
				auto motionResolver = entity->getComponent<iMotionInteractionResolverComponent>();

				switch (motionResolver->_type)
				{
				case iMotionInteractionType::Divert:
				{
					iaCircled circle = quadComp->_object->_circle;
					circle._radius *= 1.1;
					iQuadtreed::Objects objects;
					quadtree.query(circle, objects);

					float64 speed = velocity->_velocity.length();
					iaVector2d diversion;

					for (const auto &object : objects)
					{
						iEntityID otherEntityID = std::any_cast<iEntityID>(object->_userData);

						// skip self
						if (otherEntityID == entity->getID())
						{
							continue;
						}

						/*auto *hasMotionInteraction = registry->try_get<iMotionInteractionResolverComponent>(otherEntityID);
						if (hasMotionInteraction == nullptr)
						{
							continue;
						}*/

						// calc diversion
						diversion += circle._center - object->_circle._center;
					}

					diversion.normalize();
					diversion *= speed;

					velocity->_velocity._x += diversion._x;
					velocity->_velocity._y += diversion._y;
				}
				break;

				case iMotionInteractionType::None:
				default:
					break;
				}
			}
		}

		for (auto entity : _noBoundsView->getEntities())
		{
			auto velocity = entity->getComponent<iVelocityComponent>();
			auto transform = entity->getComponent<iTransformComponent>();

			transform->_position += velocity->_velocity;
			transform->_orientation += velocity->_angularVelocity;
		}

		iaVector3d min;
		iaVector3d max;
		_bounds.getMinMax(min, max);
		const iaVector3d dimensions = _bounds._halfWidths * 2.0;

		for (auto entity : _boundsView->getEntities())
		{
			auto velocity = entity->getComponent<iVelocityComponent>();
			auto transform = entity->getComponent<iTransformComponent>();
			auto bounds = entity->getComponent<iGlobalBoundaryComponent>();

			auto &position = transform->_position;

			transform->_orientation += velocity->_angularVelocity;

			switch (bounds->_type)
			{
			case iGlobalBoundaryType::Repeat:

				position += velocity->_velocity;

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
				position += velocity->_velocity;
				break;
			}
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