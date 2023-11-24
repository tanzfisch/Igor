// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Mica.h"

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

	// before we can load any resources make sure the resource dictionary is loaded
    iResourceManager::getInstance().loadResourceDictionary("dictionaries/example_resource_dictionary.xml");	

	// TODO so here we need to in future be able to create projects and have a dictionary per project. For convenience we simply use the example dict for now
	// see below saveResourceDictionary

	_workspace = WorkspacePtr(new Workspace());

	iApplication::getInstance().addLayer(new UILayer(_window, 20, _workspace));
	igor::iApplication::getInstance().addLayer(new iLayerProfiler(_window, "Profiler", 100, iProfilerVerbosity::None));
}

void Mica::run(const iaString &filename)
{
	if (!filename.isEmpty())
	{
		_workspace->loadFile(filename);
	}

	iApplication::getInstance().run();

	// TODO in future only when saving a project
	iResourceManager::getInstance().saveResourceDictionary("dictionaries/example_resource_dictionary.xml");	
}