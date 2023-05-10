// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Mica.h"

#include "WorkspaceLayer.h"
#include "overlay/OverlayLayer.h"
#include "UILayer.h"

/*! window title definition
*/
static const iaString MICA_TITLE_PREFIX = L"Mica";

Mica::Mica()
{
	_window = iApplication::getInstance().getWindow();
	_window->setTitle(MICA_TITLE_PREFIX);
	_window->setSize(1280, 720);
	_window->setCentered();
	_window->setDoubleClick(true);
	_window->open();

	_workspace = WorkspacePtr(new Workspace());

	iApplication::getInstance().addLayer(new WorkspaceLayer(_window, 0, _workspace));
	iApplication::getInstance().addLayer(new OverlayLayer(_window, 10, _workspace));
	iApplication::getInstance().addLayer(new UILayer(_window, 20, _workspace));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(_window, "Profiler", 100, iProfilerVerbosity::None));

	_taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(_window));
	_taskFlushModels = iTaskManager::getInstance().addTask(new iTaskFlushModels(_window));
}

void Mica::run(const iaString &filename)
{
	if (!filename.isEmpty())
	{
		_workspace->loadFile(filename);
	}

	iApplication::getInstance().run();
}