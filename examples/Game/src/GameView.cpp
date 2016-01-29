// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "GameView.h"

#include "GameScene.h"

#include <iRenderer.h>
#include <iMaterial.h>
using namespace Igor;

GameView::GameView(GameControl* gameControl, iWindow* window)
: _gameControl(gameControl)
, _window(window)
{
    // init essentials
    view.setClearColor(iaColor4f(0, 0, 0, 1));
    view.setPerspective(45);
    _window->addView(&view);

    _gameScene = new GameScene(gameControl);
    view.setScene(_gameScene->getScene());
}

GameView::~GameView()
{
    _window->removeView(&view);

    if (nullptr != _gameScene)
    {
        delete _gameScene;
    }
}
