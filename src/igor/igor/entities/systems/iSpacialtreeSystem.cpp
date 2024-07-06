// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iSpacialtreeSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iQuadtreeComponent.h>
#include <igor/entities/components/iOctreeComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/entities/components/iCircleCollision2DComponent.h>
#include <igor/entities/components/iSphereCollision3DComponent.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iSpacialtreeSystem::iSpacialtreeSystem()
	{
		_quadtreePositionView = createView<iTransformComponent, iQuadtreeComponent>();
		_quadtreeCircleView = createView<iTransformComponent, iQuadtreeComponent, iCircleCollision2DComponent>();
		_octreePositionView = createView<iTransformComponent, iOctreeComponent>();
		_octreeSphereView = createView<iTransformComponent, iOctreeComponent, iSphereCollision3DComponent>();
	}

	iEntitySystemStage iSpacialtreeSystem::getStage() const
	{
		return iEntitySystemStage::Update;
	}

	void iSpacialtreeSystem::onUpdateQuadtree(iQuadtreed &quadtree)
	{
		for (auto entity : _quadtreePositionView->getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iQuadtreeComponent *body = entity->getComponent<iQuadtreeComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			const iaVector2d position(transform->_position._x, transform->_position._y);
			quadtree.update(body->_object, position);
		}

		for (auto entity : _quadtreeCircleView->getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iQuadtreeComponent *body = entity->getComponent<iQuadtreeComponent>();
			iCircleCollision2DComponent *circleCollision = entity->getComponent<iCircleCollision2DComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			const iaCircled circle(transform->_position._x + circleCollision->_offset._x,
								   transform->_position._y + circleCollision->_offset._y,
								   circleCollision->_radius);
			quadtree.update(body->_object, circle);
		}
	}

	void iSpacialtreeSystem::onUpdateOctree(iOctreed &octree)
	{
		for (auto entity : _octreePositionView->getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iOctreeComponent *body = entity->getComponent<iOctreeComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			octree.update(body->_object, transform->_position);
		}
		
		for (auto entity : _quadtreeCircleView->getEntities())
		{
			iTransformComponent *transform = entity->getComponent<iTransformComponent>();
			iOctreeComponent *body = entity->getComponent<iOctreeComponent>();
			iSphereCollision3DComponent *collision = entity->getComponent<iSphereCollision3DComponent>();

			if (body->_object == nullptr ||
				body->_object->_parent.expired())
			{
				continue;
			}

			const iaSphered sphere(transform->_position + collision->_offset, collision->_radius);
			octree.update(body->_object, sphere);
		}
	}

	void iSpacialtreeSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		iEntityScenePtr scene = context._scene;

		if (scene->hasQuadtree())
		{
			onUpdateQuadtree(scene->getQuadtree());
		}

		if (scene->hasOctree())
		{
			onUpdateOctree(scene->getOctree());
		}

	}

} // igor
