// Igor game engine
// (c) Copyright 2014-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Mica.h"

#include "MicaSceneLayer.h"

/*! window title definition
*/
static const iaString MICA_TITLE_PREFIX = L"Mica";

Mica::Mica()
{
	_window = iApplication::getInstance().createWindow();
	_window->setSize(1280, 720);
	_window->setCentered();
	_window->setTitle(MICA_TITLE_PREFIX);
	_window->setDoubleClick(true);
	_window->open();

	iApplication::getInstance().addLayer(new MicaSceneLayer(_window, 0));
}

Mica::~Mica()
{
	iApplication::getInstance().clearLayerStack(_window);
}

void Mica::run(const iaString &fileName)
{
	// TODO

	iApplication::getInstance().run();
}