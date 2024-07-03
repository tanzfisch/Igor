// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iMeshRenderSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/components/iMeshRenderComponent.h>
#include <igor/entities/components/iTransformComponent.h>
#include <igor/renderer/iRenderEngine.h>

#include <iaux/math/iaMatrix.h>
using namespace iaux;

namespace igor
{
	iMeshRenderSystem::iMeshRenderSystem()
	{
		_view = createView<iTransformComponent, iMeshRenderComponent>();
	}

	iEntitySystemStage iMeshRenderSystem::getStage() const
	{
		return iEntitySystemStage::Update;
	}

	void iMeshRenderSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		con_assert(context._renderEngine != nullptr, "zero pointer");
		iRenderEnginePtr renderEngine = context._renderEngine;

		// TODO get cam from render engine, query the octree (frustum culling) and pass the result to the render engine

		for (auto entity : _view->getEntities())
		{
			auto meshRender = entity->getComponent<iMeshRenderComponent>();
			auto transform = entity->getComponent<iTransformComponent>();

			renderEngine->addMesh(meshRender->getMesh(), meshRender->getMaterial(), transform->_worldMatrix);
		}
	}

} // igor