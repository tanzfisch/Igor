// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "Game.h"

#include <Igor.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iTimer.h>
#include <iKeyboard.h>
#include <iTextureResourceFactory.h>
#include <iResourceManager.h>
#include <iTaskManager.h>
#include <iMaterialResourceFactory.h>
#include <iTaskFlushModels.h>
#include <iTaskFlushTextures.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaState.h>
#include <iaTransition.h>
using namespace IgorAux;

#include "GameCore.h"

Game::Game()
{ 
	init();
}

Game::~Game()
{
	deinit();
}

void Game::init()
{
    // window and view
    _window.setTitle("Game Example");
    _window.addView(&_menuView);
    _window.open();

    _menuView.setOrthogonal(0, _window.getClientWidth(), _window.getClientHeight(), 0);
    _menuView.setClearColor(iaColor4f(0.5, 0.4, 0.3, 1));

    // init state machine
    uint32 initState = _stateMachine.createState();
    uint32 introState = _stateMachine.createState();
    uint32 menuState = _stateMachine.createState();
    uint32 helpState = _stateMachine.createState();
    uint32 settingsState = _stateMachine.createState();
    uint32 creditsState = _stateMachine.createState();
    uint32 gameState = _stateMachine.createState();
    uint32 winState = _stateMachine.createState();
    uint32 looseState = _stateMachine.createState();
    uint32 exitState = _stateMachine.createState();

    _stateMachine.setInitialState(initState);
    _stateMachine.registerEnterStateDelegate(initState, EnterStateDelegate(this, &Game::onEnterInitState));
    _stateMachine.registerLeaveStateDelegate(initState, LeaveStateDelegate(this, &Game::onLeaveInitState));

    _stateMachine.registerHandleStateDelegate(introState, HandleStateDelegate(this, &Game::onHandleIntroState));
    _stateMachine.registerEnterStateDelegate(introState, EnterStateDelegate(this, &Game::onEnterIntroState));
    _stateMachine.registerLeaveStateDelegate(introState, LeaveStateDelegate(this, &Game::onLeaveIntroState));
    _stateMachine.registerRenderStateDelegate(introState, RenderStateDelegate(this, &Game::onRenderIntroState));

    _stateMachine.registerHandleStateDelegate(menuState, HandleStateDelegate(this, &Game::onHandleMenuState));
    _stateMachine.registerRenderStateDelegate(menuState, RenderStateDelegate(this, &Game::onRenderMenuState));
    _stateMachine.registerEnterStateDelegate(menuState, EnterStateDelegate(this, &Game::onEnterMenuState));
    _stateMachine.registerLeaveStateDelegate(menuState, LeaveStateDelegate(this, &Game::onLeaveMenuState));

    _stateMachine.registerHandleStateDelegate(gameState, HandleStateDelegate(this, &Game::onHandleGameState));
    _stateMachine.registerRenderStateDelegate(gameState, RenderStateDelegate(this, &Game::onRenderGameState));
    _stateMachine.registerEnterStateDelegate(gameState, EnterStateDelegate(this, &Game::onEnterGameState));
    _stateMachine.registerLeaveStateDelegate(gameState, LeaveStateDelegate(this, &Game::onLeaveGameState));

    _stateMachine.registerHandleStateDelegate(winState, HandleStateDelegate(this, &Game::onHandleWinState));
    _stateMachine.registerRenderStateDelegate(winState, RenderStateDelegate(this, &Game::onRenderWinState));
    _stateMachine.registerEnterStateDelegate(winState, EnterStateDelegate(this, &Game::onEnterWinState));
    _stateMachine.registerLeaveStateDelegate(winState, LeaveStateDelegate(this, &Game::onLeaveWinState));

    _stateMachine.registerHandleStateDelegate(looseState, HandleStateDelegate(this, &Game::onHandleLooseState));
    _stateMachine.registerEnterStateDelegate(looseState, EnterStateDelegate(this, &Game::onEnterLooseState));

    _stateMachine.registerEnterStateDelegate(exitState, EnterStateDelegate(this, &Game::onEnterExitState));

    _initIntroTransition = _stateMachine.createTransition(initState, introState);
    _introMenuTransition = _stateMachine.createTransition(introState, menuState);

    _menuHelpTransition = _stateMachine.createTransition(menuState, helpState);
    _helpMenuTransition = _stateMachine.createTransition(helpState, menuState);
    _menuCreditsTransition = _stateMachine.createTransition(menuState, creditsState);
    _creditsMenuTransition = _stateMachine.createTransition(creditsState, menuState);
    _menuSettingsTransition = _stateMachine.createTransition(menuState, settingsState);
    _settingsMenuTransition = _stateMachine.createTransition(settingsState, menuState);

    _menuGameTransition = _stateMachine.createTransition(menuState, gameState);
    _gameMenuTransition = _stateMachine.createTransition(gameState, menuState);

    _gameWinTransition = _stateMachine.createTransition(gameState, winState);
    _gameLooseTransition = _stateMachine.createTransition(gameState, looseState);

    _winMenuTransition = _stateMachine.createTransition(winState, menuState);
    _looseMenuTransition = _stateMachine.createTransition(looseState, menuState);

    _menuExitTransition = _stateMachine.createTransition(menuState, exitState);

    _stateMachine.start();

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(&_stateMachine, &iaStateMachine::handle));
    _menuView.registerRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));

    // create some materials
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    // load a texture font
    _font = new iTextureFont("StandardFont.png");
}

void Game::deinit()
{
    _menuView.unregisterRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));
    iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(&_stateMachine, &iaStateMachine::handle));

    if (_font)
    {
        delete _font;
    }

    if (_taskFlushModels != nullptr)
    {
        _taskFlushModels->abort();
        _taskFlushModels = nullptr;
    }
    if (_taskFlushTextures != nullptr)
    {
        _taskFlushTextures->abort();
        _taskFlushTextures = nullptr;
    }

	_window.close();
	_window.removeView(&_menuView);
}

void Game::run()
{
    iApplication::getInstance().run();
}

void Game::onEnterLooseState()
{
    // TODO
}

void Game::onHandleLooseState()
{
    // TODO
    _stateMachine.doTransition(_looseMenuTransition);
}

void Game::onEnterWinState()
{
    // TODO
}

void Game::onLeaveWinState()
{
    // TODO
}

void Game::onHandleWinState()
{
    // TODO
    _stateMachine.doTransition(_winMenuTransition);
}

void Game::onRenderWinState()
{
    // TODO
}

void Game::onEnterInitState()
{ 
    // TODO skips init state for now
    _stateMachine.doTransition(_initIntroTransition);
}

void Game::onLeaveInitState()
{
    // TODO
}

void Game::onEnterIntroState()
{
     // TODO skips intro state for now
    _stateMachine.doTransition(_introMenuTransition);
}

void Game::onLeaveIntroState()
{
    // TODO
}

void Game::onHandleIntroState()
{
    // TODO
}

void Game::onRenderIntroState()
{
    // TODO
}

void Game::onKeyReleasedMenuState(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::ESC:
        _stateMachine.doTransition(_menuExitTransition);
        break;

    case iKeyCode::Down:
        _activeMenuEntry++;
        if (_activeMenuEntry >= _menuEntries.size())
        {
            _activeMenuEntry = 0;
        }
        break;

    case iKeyCode::Up:
        _activeMenuEntry--;
        if (_activeMenuEntry < 0)
        {
            _activeMenuEntry = _menuEntries.size() - 1;
        }
        break;

    case iKeyCode::Return:
    case iKeyCode::Enter:
    case iKeyCode::Space:
        switch (_activeMenuEntry)
        {
        case 0:
            _stateMachine.doTransition(_menuGameTransition);
            break;

        case 1:
            break;

        case 2:
            break;

        case 3:
            break;

        case 4:
            _stateMachine.doTransition(_menuExitTransition);
            break;
        }
        break;
    }
}

void Game::onEnterMenuState()
{
    // init some members
    _menuEntries = vector<iaString>{ "Play", "Settings", "Help", "Credits", "Exit" };

    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &Game::onKeyReleasedMenuState));

    // todo remove this later: bypassing the menu
    //_stateMachine.doTransition(_menuGameTransition);
}

void Game::onLeaveMenuState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &Game::onKeyReleasedMenuState));
}

void Game::onHandleMenuState()
{
    // TODO
}

void Game::onRenderMenuState()
{
    const float32 fontSize = 70.0f;
    const float32 lineHeight = 100.0f;
    const float32 offset = 200.0f;
    float32 scale = _window.getClientHeight() / 1000.0f;
    
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(fontSize * scale);
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);

    for (int i = 0; i < _menuEntries.size(); ++i)
    {
        if (i == _activeMenuEntry)
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
        }
        else if (i == 0 || i == 4)
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 0.4));
        }

        iRenderer::getInstance().drawString(_window.getClientWidth() * 0.5, offset * scale + i * lineHeight * scale, _menuEntries[i], iHorrizontalAlign::Center, iVerticalAlign::Center, 0);
    }

    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
}

void Game::onEnterExitState()
{
    iApplication::getInstance().stop();
}

void Game::onCloseWindow()
{
	iApplication::getInstance().stop();
}

void Game::onEnterGameState()
{
    _window.removeView(&_menuView);

    con_assert(_gameCore == nullptr, "unexpected");
    _gameCore = new GameCore(&_window);
}

void Game::onLeaveGameState()
{
    con_assert(_gameCore != nullptr, "zero pointer");
    if (_gameCore != nullptr)
    {
        delete _gameCore;
        _gameCore = nullptr;
    }

    _window.addView(&_menuView);
}

void Game::onHandleGameState()
{
    // TODO
    _stateMachine.doTransition(_gameMenuTransition);
}

void Game::onRenderGameState()
{
    // nothing to do
}
