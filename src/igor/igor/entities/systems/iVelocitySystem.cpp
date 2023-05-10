// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iVelocitySystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

#include <entt.h>

namespace igor
{
	void iVelocitySystem::update(iEntityScenePtr scene)
	{
		auto *registry = static_cast<entt::registry*>(scene->getRegistry());
		auto &quadtree = scene->getQuadtree();

		auto viewInteractionResolver = registry->view<iVelocityComponent, iBody2DComponent, iMotionInteractionResolverComponent>();

		for (auto entityID : viewInteractionResolver)
		{
			auto [velocity, quadComp, motionResolver] = viewInteractionResolver.get<iVelocityComponent, iBody2DComponent, iMotionInteractionResolverComponent>(entityID);

			switch (motionResolver._type)
			{
			case iMotionInteractionType::Divert:
			{
				iaCircled circle = quadComp._object->_circle;
				circle._radius *= 1.1;
				iQuadtreed::Objects objects;
				quadtree.query(circle, objects);

				float64 speed = velocity._velocity.length();
				iaVector2d diversion;

				for (const auto &object : objects)
				{
					const entt::entity otherEntityID = static_cast<entt::entity>(std::any_cast<iEntityID>(object->_userData));

					// skip self
					if (otherEntityID == entityID)
					{
						continue;
					}

					auto *hasMotionInteraction = registry->try_get<iMotionInteractionResolverComponent>(otherEntityID);
					if(hasMotionInteraction == nullptr)
					{
						continue;
					}

					// calc diversion
					diversion += circle._center - object->_circle._center;
				}

				diversion.normalize();
				diversion *= speed;

				velocity._velocity._x += diversion._x;
				velocity._velocity._y += diversion._y;
			}
			break;

			case iMotionInteractionType::None:
			default:
				break;
			}
		}

		auto viewNoBounds = registry->view<iVelocityComponent, iTransformComponent>(entt::exclude<iGlobalBoundaryComponent>);

		for (auto entityID : viewNoBounds)
		{
			auto [velocity, transform] = viewNoBounds.get<iVelocityComponent, iTransformComponent>(entityID);

			transform._position += velocity._velocity;
			transform._orientation += velocity._angularVelocity;
		}

		auto viewWithBounds = registry->view<iVelocityComponent, iTransformComponent, iGlobalBoundaryComponent>();
		iaVector3d min;
		iaVector3d max;
		_bounds.getMinMax(min, max);
		const iaVector3d dimensions = _bounds._halfWidths * 2.0;

		for (auto entityID : viewWithBounds)
		{
			auto [velocity, transform, bounds] = viewWithBounds.get<iVelocityComponent, iTransformComponent, iGlobalBoundaryComponent>(entityID);

			auto &position = transform._position;

			transform._orientation += velocity._angularVelocity;

			switch (bounds._type)
			{
			case iGlobalBoundaryType::Repeat:

				position += velocity._velocity;

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
				position += velocity._velocity;
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