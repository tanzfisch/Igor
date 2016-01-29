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
    _menuEntries = vector<iaString>{ "Play", "Settings", "Help", "Credits", "Exit" };
    
    // init essentials
    view.setClearColor(iaColor4f(0.5f, 0.5f, 0.5f, 1.0f));
    view.setOrthogonal(0, 1024, 768, 0);
    view.registerRenderDelegate(RenderDelegate(this, &StateMachineExample::onRender));

	window.setTitle("State Machine Example");
	window.addView(&view);
	window.open();
	window.registerWindowCloseDelegate(WindowCloseDelegate(this, &StateMachineExample::onCloseWindow));

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
   
    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(&_stateMachine, &iaStateMachine::handle));
    view.registerRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));

    _stateMachine.start();

    // init rendering stuff
    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial();
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::Blend, iRenderStateValue::On);
    iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending)->getRenderStateSet().setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _font = new iTextureFont("StandardFont.png");

    _particleTexture = iTextureResourceFactory::getInstance().loadFile("simpleParticle.png");
    _particleSystem.setParticleSize(10.0f, 50.0f);
    _particleSystem.setInitialVelocity(iaVector2f(0, -10));
    _particleSystem.setMaxParticleCount(100);
    _particleSystem.setEmitRate(1);
    _particleSystem.setSpreadFactor(2.0f);
    _particleSystem.setAirDrag(0.995f);
    _particleSystem.setGravitation(iaVector2f(0, 0.2f));
}

void StateMachineExample::deinit()
{
    if (_font)
    {
        delete _font;
    }

    iMaterialResourceFactory::getInstance().destroyMaterial(_materialWithTextureAndBlending);
    _materialWithTextureAndBlending = 0;

	window.close();
	window.removeView(&view);

    view.unregisterRenderDelegate(RenderDelegate(this, &StateMachineExample::onRender));
    view.unregisterRenderDelegate(RenderDelegate(&_stateMachine, &iaStateMachine::render));
}

void StateMachineExample::onEnterLooseState()
{
    _time = iTimer::getInstance().getTime();
    _backgroundText = ")-: YOU LOOSE :-(";
}

void StateMachineExample::onHandleLooseState()
{
    float64 currentTime = iTimer::getInstance().getTime();

    if (currentTime > _time + 2 * IGOR_SECOND)
    {
        _stateMachine.doTransition(_looseMenuTransition);
    }
}

void StateMachineExample::onEnterWinState()
{
    _backgroundText = "!!! YOU WIN !!!";
    _time = iTimer::getInstance().getTime();
}

void StateMachineExample::onLeaveWinState()
{
}

void StateMachineExample::onHandleWinState()
{
    float64 currentTime = iTimer::getInstance().getTime();

    if (currentTime > _time + 2 * IGOR_SECOND)
    {
        _stateMachine.doTransition(_winMenuTransition);
    }

    _particleSystem.handle();
}

void StateMachineExample::onRenderWinState()
{
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1,0,0,1));

    iRenderer::getInstance().bindTexture(_particleTexture, 0);
    iRenderer::getInstance().drawParticles(500, 700, 0, _particleSystem.getParticles(), _particleSystem.getParticleCount());
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
    _time = iTimer::getInstance().getTime();
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegateExt(this, &StateMachineExample::onKeyReleasedGameState));
    iKeyboard::getInstance().registerKeyDownDelegate(KeyDownDelegateExt(this, &StateMachineExample::onKeyPressedGameState));
}

void StateMachineExample::onLeaveGameState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegateExt(this, &StateMachineExample::onKeyReleasedGameState));
    iKeyboard::getInstance().unregisterKeyDownDelegate(KeyDownDelegateExt(this, &StateMachineExample::onKeyPressedGameState));
}

void StateMachineExample::onHandleGameState()
{
    float64 currentTime = iTimer::getInstance().getTime();

    if (currentTime > _time + 5 * IGOR_SECOND)
    {
        _stateMachine.doTransition(_gameLooseTransition);
    }
}

void StateMachineExample::onRenderGameState()
{
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);

    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
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
    _time = iTimer::getInstance().getTime();
    _backgroundText = "3";
}

void StateMachineExample::onHandleInitState()
{
    float64 currentTime = iTimer::getInstance().getTime();

    if (currentTime > _time + 1 * IGOR_SECOND)
    {
        _backgroundText = "2";
    }

    if (currentTime > _time + 2 * IGOR_SECOND)
    {
        _backgroundText = "1";
    }

    if (currentTime > _time + 3 * IGOR_SECOND)
    {
        _stateMachine.doTransition(_initMenuTransition);
    }
}

void StateMachineExample::onLeaveInitState()
{
    // start spinning background after init
    iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &StateMachineExample::onHandle));
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
    iKeyboard::getInstance().registerKeyUpDelegate(KeyUpDelegateExt(this, &StateMachineExample::onKeyReleasedMenuState));
}

void StateMachineExample::onLeaveMenuState()
{
    iKeyboard::getInstance().unregisterKeyUpDelegate(KeyUpDelegateExt(this, &StateMachineExample::onKeyReleasedMenuState));
}

void StateMachineExample::onHandleMenuState()
{    
}

void StateMachineExample::onRenderMenuState()
{
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
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

        iRenderer::getInstance().drawString(500, 200.0f + i * 70.0f, _menuEntries[i], iHorrizontalAlign::Center, iVerticalAlign::Center, 0);
    }
}

void StateMachineExample::onRender()
{
    iaMatrixf modelMatrix;
    modelMatrix.translate(iaVector3f(0, 0, -30));
    iRenderer::getInstance().setModelMatrix(modelMatrix);
    iMaterialResourceFactory::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(50.0f);
    iRenderer::getInstance().setColor(iaColor4f(0, 0, 0, 0.3));
    iRenderer::getInstance().drawString(500, 400, _backgroundText, iHorrizontalAlign::Center, iVerticalAlign::Center, _spinning);
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
