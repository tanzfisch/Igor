// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "GameMenu.h"

#include <Igor.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iTimer.h>
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

#include "GameControl.h"
#include "GameView.h"
#include "GameInput.h"

GameMenu::GameMenu()
{ 
	init();
}

GameMenu::~GameMenu()
{
	deinit();
}

void GameMenu::init()
{
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
    _stateMachine.registerEnterStateDelegate(initState, EnterStateDelegate(this, &GameMenu::onEnterInitState));
    _stateMachine.registerLeaveStateDelegate(initState, LeaveStateDelegate(this, &GameMenu::onLeaveInitState));

    _stateMachine.registerHandleStateDelegate(introState, HandleStateDelegate(this, &GameMenu::onHandleIntroState));
    _stateMachine.registerEnterStateDelegate(introState, EnterStateDelegate(this, &GameMenu::onEnterIntroState));
    _stateMachine.registerLeaveStateDelegate(introState, LeaveStateDelegate(this, &GameMenu::onLeaveIntroState));
    _stateMachine.registerRenderStateDelegate(introState, RenderStateDelegate(this, &GameMenu::onRenderIntroState));

    _stateMachine.registerHandleStateDelegate(menuState, HandleStateDelegate(this, &GameMenu::onHandleMenuState));
    _stateMachine.registerRenderStateDelegate(menuState, RenderStateDelegate(this, &GameMenu::onRenderMenuState));
    _stateMachine.registerEnterStateDelegate(menuState, EnterStateDelegate(this, &GameMenu::onEnterMenuState));
    _stateMachine.registerLeaveStateDelegate(menuState, LeaveStateDelegate(this, &GameMenu::onLeaveMenuState));

    _stateMachine.registerHandleStateDelegate(gameState, HandleStateDelegate(this, &GameMenu::onHandleGameState));
    _stateMachine.registerRenderStateDelegate(gameState, RenderStateDelegate(this, &GameMenu::onRenderGameState));
    _stateMachine.registerEnterStateDelegate(gameState, EnterStateDelegate(this, &GameMenu::onEnterGameState));
    _stateMachine.registerLeaveStateDelegate(gameState, LeaveStateDelegate(this, &GameMenu::onLeaveGameState));

    _stateMachine.registerHandleStateDelegate(winState, HandleStateDelegate(this, &GameMenu::onHandleWinState));
    _stateMachine.registerRenderStateDelegate(winState, RenderStateDelegate(this, &GameMenu::onRenderWinState));
    _stateMachine.registerEnterStateDelegate(winState, EnterStateDelegate(this, &GameMenu::onEnterWinState));
    _stateMachine.registerLeaveStateDelegate(winState, LeaveStateDelegate(this, &GameMenu::onLeaveWinState));

    _stateMachine.registerHandleStateDelegate(looseState, HandleStateDelegate(this, &GameMenu::onHandleLooseState));
    _stateMachine.registerEnterStateDelegate(looseState, EnterStateDelegate(this, &GameMenu::onEnterLooseState));

    _stateMachine.registerEnterStateDelegate(exitState, EnterStateDelegate(this, &GameMenu::onEnterExitState));

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
}

void GameMenu::deinit()
{
    view.unregisterRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));

    if (_font)
    {
        delete _font;
    }

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    _materialWithTextureAndBlending = 0;

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

	window.close();
	window.removeView(&view);
}

void GameMenu::run()
{
    iApplication::getInstance().run();
}

void GameMenu::onEnterLooseState()
{
    // TODO
}

void GameMenu::onHandleLooseState()
{
    // TODO
    _stateMachine.doTransition(_looseMenuTransition);
}

void GameMenu::onEnterWinState()
{
    // TODO
}

void GameMenu::onLeaveWinState()
{
    // TODO
}

void GameMenu::onHandleWinState()
{
    // TODO
    _stateMachine.doTransition(_winMenuTransition);
}

void GameMenu::onRenderWinState()
{
    // TODO
}

void GameMenu::onEnterInitState()
{
    // TODO skips init state for now
    _stateMachine.doTransition(_initIntroTransition);
}

void GameMenu::onLeaveInitState()
{
    // TODO
}

void GameMenu::onEnterIntroState()
{
    // init visual
    view.setClearColor(iaColor4f(0.2, 0.2, 0.2, 1));
    view.setOrthogonal(0, 1024, 768, 0);

    window.setTitle("State Machine Example");
    window.addView(&view);
    window.setSize(1024, 768);
    window.open();
    window.registerWindowCloseDelegate(WindowCloseDelegate(this, &GameMenu::onCloseWindow));

    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(&_stateMachine, &iaStateMachine::handle));
    view.registerRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));

    // init rendering stuff
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextOut");
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _font = new iTextureFont("StandardFont.png");

    _taskFlushModels = new iTaskFlushModels(&window);
    iTaskManager::getInstance().addTask(_taskFlushModels);
    _taskFlushTextures = new iTaskFlushTextures(&window);
    iTaskManager::getInstance().addTask(_taskFlushTextures);

    // TODO skips intro state for now
    _stateMachine.doTransition(_introMenuTransition);
}

void GameMenu::onLeaveIntroState()
{
    // TODO
}

void GameMenu::onHandleIntroState()
{
    // TODO
}

void GameMenu::onRenderIntroState()
{
    // TODO
}

void GameMenu::onKeyReleasedMenuState(iKeyCode key)
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

void GameMenu::onEnterMenuState()
{
    // init some members
    _menuEntries = vector<iaString>{ "Play", "Settings", "Help", "Credits", "Exit" };

    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegateExt(this, &GameMenu::onKeyReleasedMenuState));

    // todo remove this later: bypassing the menu
    //_stateMachine.doTransition(_menuGameTransition);
}

void GameMenu::onLeaveMenuState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegateExt(this, &GameMenu::onKeyReleasedMenuState));
}

void GameMenu::onHandleMenuState()
{
    // TODO
}

void GameMenu::onRenderMenuState()
{
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(50.0f);
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

        iRenderer::getInstance().drawString(500, 200.0f + i * 70.0f, _menuEntries[i], iHorrizontalAlign::Center, iVerticalAlign::Center, 0);
    }
}

void GameMenu::onEnterExitState()
{
    iApplication::getInstance().stop();
}

void GameMenu::onCloseWindow()
{
	iApplication::getInstance().stop();
}

void GameMenu::onEnterGameState()
{
    window.removeView(&view);

    if (nullptr == _gameControl)
    {
        _gameControl = new GameControl();
    }

    if (nullptr == _gameView)
    {
        _gameView = new GameView(_gameControl, &window);
    }

    if (nullptr == _gameInput)
    {
        _gameInput = new GameInput(_gameControl);
    }
}

void GameMenu::onLeaveGameState()
{
    if (nullptr != _gameInput)
    {
        delete _gameInput;
        _gameInput = nullptr;
    }
    
    if (nullptr != _gameView)
    {
        delete _gameView;
        _gameView = nullptr;
    }

    if (nullptr != _gameControl)
    {
        delete _gameControl;
        _gameControl = nullptr;
    }

    window.addView(&view);
}

void GameMenu::onHandleGameState()
{
    if (!_gameControl->onHandle())
    {
        _stateMachine.doTransition(_gameMenuTransition);
    }
}

void GameMenu::onRenderGameState()
{
    // TODO
}
