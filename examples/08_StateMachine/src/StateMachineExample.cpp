#include "StateMachineExample.h"

#include <Igor.h>
#include <iApplication.h>
#include <iRenderer.h>
#include <iMaterial.h>
#include <iTimer.h>
#include <iTextureResourceFactory.h>
using namespace Igor;

#include <iaConsole.h>
#include <iaState.h>
#include <iaTransition.h>
#include <iMaterialResourceFactory.h>
using namespace IgorAux;

StateMachineExample::StateMachineExample()
{ 
	init();
}

StateMachineExample::~StateMachineExample()
{
	deinit();
}

void StateMachineExample::init()
{
	con_endl("--- State Machine Example --- ");
    con_endl("");

    // init some members
    _menuEntries = std::vector<iaString>{ "Play", "Settings", "Help", "Credits", "Exit" };
    
    // init essentials
    _view.setClearColor(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    _view.setOrthogonal(0, 1024, 768, 0);
    _view.registerRenderDelegate(RenderDelegate(this, &StateMachineExample::onRender));

	_window.setTitle("State Machine Example");
	_window.addView(&_view);
	_window.setCentered();
	_window.open();
	_window.registerWindowCloseDelegate(WindowCloseDelegate(this, &StateMachineExample::onCloseWindow));

    // init state machine
    uint32 initState = _stateMachine.createState();
    uint32 menuState = _stateMachine.createState();
    uint32 helpState = _stateMachine.createState();
    uint32 settingsState = _stateMachine.createState();
    uint32 creditsState = _stateMachine.createState();
    uint32 gameState = _stateMachine.createState();
    uint32 winState = _stateMachine.createState();
    uint32 looseState = _stateMachine.createState();
    uint32 exitState = _stateMachine.createState();

    _stateMachine.setInitialState(initState);
    _stateMachine.registerHandleStateDelegate(initState, HandleStateDelegate(this, &StateMachineExample::onHandleInitState));
    _stateMachine.registerEnterStateDelegate(initState, EnterStateDelegate(this, &StateMachineExample::onEnterInitState));
    _stateMachine.registerLeaveStateDelegate(initState, LeaveStateDelegate(this, &StateMachineExample::onLeaveInitState));

    _stateMachine.registerHandleStateDelegate(menuState, HandleStateDelegate(this, &StateMachineExample::onHandleMenuState));
    _stateMachine.registerRenderStateDelegate(menuState, RenderStateDelegate(this, &StateMachineExample::onRenderMenuState));
    _stateMachine.registerEnterStateDelegate(menuState, EnterStateDelegate(this, &StateMachineExample::onEnterMenuState));
    _stateMachine.registerLeaveStateDelegate(menuState, LeaveStateDelegate(this, &StateMachineExample::onLeaveMenuState));

    _stateMachine.registerHandleStateDelegate(gameState, HandleStateDelegate(this, &StateMachineExample::onHandleGameState));
    _stateMachine.registerRenderStateDelegate(gameState, RenderStateDelegate(this, &StateMachineExample::onRenderGameState));
    _stateMachine.registerEnterStateDelegate(gameState, EnterStateDelegate(this, &StateMachineExample::onEnterGameState));
    _stateMachine.registerLeaveStateDelegate(gameState, LeaveStateDelegate(this, &StateMachineExample::onLeaveGameState));

    _stateMachine.registerHandleStateDelegate(winState, HandleStateDelegate(this, &StateMachineExample::onHandleWinState));
    _stateMachine.registerRenderStateDelegate(winState, RenderStateDelegate(this, &StateMachineExample::onRenderWinState));
    _stateMachine.registerEnterStateDelegate(winState, EnterStateDelegate(this, &StateMachineExample::onEnterWinState));
    _stateMachine.registerLeaveStateDelegate(winState, LeaveStateDelegate(this, &StateMachineExample::onLeaveWinState));

    _stateMachine.registerHandleStateDelegate(looseState, HandleStateDelegate(this, &StateMachineExample::onHandleLooseState));
    _stateMachine.registerEnterStateDelegate(looseState, EnterStateDelegate(this, &StateMachineExample::onEnterLooseState));

    _stateMachine.registerEnterStateDelegate(exitState, EnterStateDelegate(this, &StateMachineExample::onEnterExitState));

    _initMenuTransition = _stateMachine.createTransition(initState, menuState);

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

    _gameGateA = _stateMachine.createGate(_gameWinTransition);
    _gameGateB = _stateMachine.createGate(_gameWinTransition);
    _gameGateC = _stateMachine.createGate(_gameWinTransition);
   
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(&_stateMachine, &iaStateMachine::handle));
    _view.registerRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));

    _stateMachine.start();

    // init rendering stuff
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _font = new iTextureFont("StandardFont.png");

    _particleTexture = iTextureResourceFactory::getInstance().loadFile("particleDot.png");

    for (int i = 0; i < 5; ++i)
    {
        _particleSystems[i].setParticleSize(10.0f, 50.0f);
        _particleSystems[i].setInitialVelocity(iaVector2f(0, -(rand() % 10 + 2)));
        _particleSystems[i].setMaxParticleCount(100);
        _particleSystems[i].setEmitRate(20);
        _particleSystems[i].setParticleLifetime(60);
        _particleSystems[i].setSpreadFactor(1.0f);
        _particleSystems[i].setAirDrag(0.05f);
        _particleSystems[i].setExternalForce(iaVector2f(0.0f, 0.05f));
        _particleSystems[i].setLoopable(false);
    }
}

void StateMachineExample::deinit()
{
    if (_font)
    {
        delete _font;
        _font = nullptr;
    }

    _particleTexture = nullptr;

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    _materialWithTextureAndBlending = 0;

	_window.close();
	_window.removeView(&_view);

    _view.unregisterRenderDelegate(RenderDelegate(this, &StateMachineExample::onRender));
    _view.unregisterRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(&_stateMachine, &iaStateMachine::handle));
    iApplication::getInstance().unregisterApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &StateMachineExample::onHandle));
}

void StateMachineExample::onEnterLooseState()
{
    _time = iTimer::getInstance().getApplicationTime();
    _backgroundText = ")-: YOU LOOSE :-(";
}

void StateMachineExample::onHandleLooseState()
{
    float64 currentTime = iTimer::getInstance().getApplicationTime();

    if (currentTime > _time + 2 * __IGOR_SECOND__)
    {
        _stateMachine.doTransition(_looseMenuTransition);
    }
}

void StateMachineExample::onEnterWinState()
{
    _backgroundText = "!!! YOU WIN !!!";
    _time = iTimer::getInstance().getApplicationTime();

    for (int i = 0; i < 5; ++i)
    {
      _particleSystems[i].restart();
    }
}

void StateMachineExample::onLeaveWinState()
{
}

void StateMachineExample::onHandleWinState()
{
    float64 currentTime = iTimer::getInstance().getApplicationTime();

    if (currentTime > _time + 2 * __IGOR_SECOND__)
    {
        _stateMachine.doTransition(_winMenuTransition);
    }

    for (int i = 0; i < 5; ++i)
    {
        _particleSystems[i].handle();
    }
}

void StateMachineExample::onRenderWinState()
{
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1,0,0,1));

    iRenderer::getInstance().bindTexture(_particleTexture, 0);
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));
    iRenderer::getInstance().drawParticles(_window.getClientWidth() * 0.5, _window.getClientHeight() * 0.5, 0, _particleSystems[0].getParticles(), _particleSystems[0].getParticleCount());
    iRenderer::getInstance().setColor(iaColor4f(0, 1, 0, 1));
    iRenderer::getInstance().drawParticles(_window.getClientWidth() * 0.4, _window.getClientHeight() * 0.7, 0, _particleSystems[1].getParticles(), _particleSystems[1].getParticleCount());
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().drawParticles(_window.getClientWidth() * 0.7, _window.getClientHeight() * 0.4, 0, _particleSystems[2].getParticles(), _particleSystems[2].getParticleCount());
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 1, 1));
    iRenderer::getInstance().drawParticles(_window.getClientWidth() * 0.6, _window.getClientHeight() * 0.55, 0, _particleSystems[3].getParticles(), _particleSystems[3].getParticleCount()); 
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 1, 1));
    iRenderer::getInstance().drawParticles(_window.getClientWidth() * 0.45, _window.getClientHeight() * 0.6, 0, _particleSystems[4].getParticles(), _particleSystems[4].getParticleCount());
}

void StateMachineExample::onKeyPressedGameState(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::A:
        _stateMachine.openGate(_gameWinTransition, _gameGateA);
        _drawA = true;
        break;

    case iKeyCode::B:
        _stateMachine.openGate(_gameWinTransition, _gameGateB);
        _drawB = true;
        break;

    case iKeyCode::C:
        _stateMachine.openGate(_gameWinTransition, _gameGateC);
        _drawC = true;
        break;
    }
}

void StateMachineExample::onKeyReleasedGameState(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::A:
        _stateMachine.closeGate(_gameWinTransition, _gameGateA);
        _drawA = false;
        break;

    case iKeyCode::B:
        _stateMachine.closeGate(_gameWinTransition, _gameGateB);
        _drawB = false;
        break;

    case iKeyCode::C:
        _stateMachine.closeGate(_gameWinTransition, _gameGateC);
        _drawC = false;
        break;
    }
}

void StateMachineExample::onEnterGameState()
{
    _drawA = false;
    _drawB = false;
    _drawC = false;

    _backgroundText = "press A,B and C";
    _time = iTimer::getInstance().getApplicationTime();
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &StateMachineExample::onKeyReleasedGameState));
    iKeyboard::getInstance().registerKeyDownDelegate(iKeyDownDelegate(this, &StateMachineExample::onKeyPressedGameState));
}

void StateMachineExample::onLeaveGameState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &StateMachineExample::onKeyReleasedGameState));
    iKeyboard::getInstance().unregisterKeyDownDelegate(iKeyDownDelegate(this, &StateMachineExample::onKeyPressedGameState));
}

void StateMachineExample::onHandleGameState()
{
    float64 currentTime = iTimer::getInstance().getApplicationTime();

    if (currentTime > _time + 5 * __IGOR_SECOND__)
    {
        _stateMachine.doTransition(_gameLooseTransition);
    }
}

void StateMachineExample::onRenderGameState()
{
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setFontSize(100.0f);
    iRenderer::getInstance().setColor(iaColor4f(1, 0, 0, 1));

    if (_drawA)
    {
        iRenderer::getInstance().drawString(800, 500, "A", _spinning);
    }

    if (_drawB)
    {
        iRenderer::getInstance().drawString(100, 200, "B", _spinning + 100);
    }

    if (_drawC)
    {
        iRenderer::getInstance().drawString(300, 400, "C", _spinning - 50);
    }
}

void StateMachineExample::onEnterInitState()
{
    _time = iTimer::getInstance().getApplicationTime();
    _backgroundText = "3";
}

void StateMachineExample::onHandleInitState()
{
    float64 currentTime = iTimer::getInstance().getApplicationTime();

    if (currentTime > _time + 1 * __IGOR_SECOND__)
    {
        _backgroundText = "2";
    }

    if (currentTime > _time + 2 * __IGOR_SECOND__)
    {
        _backgroundText = "1";
    }

    if (currentTime > _time + 3 * __IGOR_SECOND__)
    {
        _stateMachine.doTransition(_initMenuTransition);
    }
}

void StateMachineExample::onLeaveInitState()
{
    // start spinning background after init
    iApplication::getInstance().registerApplicationPreDrawHandleDelegate(iApplicationPreDrawHandleDelegate(this, &StateMachineExample::onHandle));
}

void StateMachineExample::onKeyReleasedMenuState(iKeyCode key)
{
    switch (key)
    {
    case iKeyCode::ESC:
        _stateMachine.doTransition(_menuExitTransition);
        break;

    case iKeyCode::Down:
    case iKeyCode::Up:
        if (_activeMenuEntry == 0)
        {
            _activeMenuEntry = 4;
        }
        else
        {
            _activeMenuEntry = 0;
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

void StateMachineExample::onEnterMenuState()
{
    _backgroundText = "Main Menu";
    iKeyboard::getInstance().registerKeyUpDelegate(iKeyUpDelegate(this, &StateMachineExample::onKeyReleasedMenuState));
}

void StateMachineExample::onLeaveMenuState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(iKeyUpDelegate(this, &StateMachineExample::onKeyReleasedMenuState));
}

void StateMachineExample::onHandleMenuState()
{    
}

void StateMachineExample::onRenderMenuState()
{
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setFontSize(50.0f);

    for (int i = 0; i < 5; ++i)
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
            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 0.5));
        }

        iRenderer::getInstance().drawString(500, 200.0f + i * 70.0f, _menuEntries[i], iHorizontalAlignment::Center, iVerticalAlignment::Center, 0);
    }
}

void StateMachineExample::onRender()
{
    iaMatrixd modelMatrix;
    modelMatrix.translate(0, 0, -30);
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(50.0f);
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 0.3));
    iRenderer::getInstance().drawString(500, 400, _backgroundText, iHorizontalAlignment::Center, iVerticalAlignment::Center, _spinning);
}

void StateMachineExample::onHandle()
{
    _spinning += 0.05f;
}

void StateMachineExample::onEnterExitState()
{
    con_endl("onEnterExitState");
    iApplication::getInstance().stop();
}

void StateMachineExample::run()
{
	iApplication::getInstance().run();
}

void StateMachineExample::onCloseWindow()
{
	con_endl("windows was closed");
	iApplication::getInstance().stop();
}
