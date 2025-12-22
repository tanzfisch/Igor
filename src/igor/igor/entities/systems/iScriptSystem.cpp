// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/systems/iScriptSystem.h>

#include <igor/entities/iEntityScene.h>
#include <igor/entities/iEntity.h>

namespace igor
{
	iScriptSystem::iScriptSystem()
	{
		_view = createView<iScriptComponent>();
	}

	iEntitySystemPtr iScriptSystem::createInstance()
	{
		return new iScriptSystem();
	}

	const iaString &iScriptSystem::getTypeName()
	{
		static const iaString typeName("igor_script_system");
		return typeName;
	}

	iEntitySystemStage iScriptSystem::getStage() const
	{
		return iEntitySystemStage::Update;
	}

	void iScriptSystem::onUpdate(const iEntitySceneUpdateContext &context)
	{
		const auto &entities = _view->getEntities();
		for (const auto entity : entities)
		{
			auto script = entity->getComponent<iScriptComponent>();
			script->execute();
		}
	}

} // igor