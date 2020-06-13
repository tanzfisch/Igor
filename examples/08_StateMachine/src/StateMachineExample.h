//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/                     ) )     ((
//                                           (_(       \)
//    (c) Copyright 2014-2020 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
//
// contact: martinloga@gmx.de

#ifndef __STATEMACHINEEXAMPLE__
#define __STATEMACHINEEXAMPLE__

#include <igor/system/iWindow.h>
#include <igor/graphics/iView.h>
#include <igor/system/iKeyboard.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/resources/texture/iTexture.h>
#include <igor/simulation/iParticleSystem2D.h>
using namespace igor;

#include <iaux/statemachine/iaStateMachine.h>
#include <iaux/data/iaString.h>
#include <iaux/system/iaTime.h>
using namespace iaux;

#include <vector>

class StateMachineExample
{

private:
    iWindow _window;
    iView _view;

    iTextureFont *_font = 0;
    int32 _materialWithTextureAndBlending = -1;
    iParticleSystem2D _particleSystems[5];
    iTexturePtr _particleTexture;

    iaStateMachine _stateMachine;
    uint32 _initMenuTransition = 0;
    uint32 _menuExitTransition = 0;
    uint32 _menuHelpTransition = 0;
    uint32 _helpMenuTransition = 0;
    uint32 _menuCreditsTransition = 0;
    uint32 _creditsMenuTransition = 0;
    uint32 _menuSettingsTransition = 0;
    uint32 _settingsMenuTransition = 0;
    uint32 _menuGameTransition = 0;
    uint32 _gameMenuTransition = 0;
    uint32 _gameWinTransition = 0;
    uint32 _gameLooseTransition = 0;
    uint32 _winMenuTransition = 0;
    uint32 _looseMenuTransition = 0;

    uint32 _gameGateA;
    uint32 _gameGateB;
    uint32 _gameGateC;

    bool _drawA = false;
    bool _drawB = false;
    bool _drawC = false;

    iaTime _time;

    iaString _backgroundText = "";
    float32 _spinning = 0.0f;
    int32 _activeMenuEntry = 0;
    std::vector<iaString> _menuEntries;

    void onCloseWindow();

    void onKeyReleasedMenuState(iKeyCode key);
    void onEnterMenuState();
    void onLeaveMenuState();
    void onHandleMenuState();
    void onRenderMenuState();

    void onEnterInitState();
    void onLeaveInitState();
    void onHandleInitState();

    void onEnterWinState();
    void onLeaveWinState();
    void onHandleWinState();
    void onRenderWinState();

    void onEnterLooseState();
    void onHandleLooseState();

    void onKeyReleasedGameState(iKeyCode key);
    void onKeyPressedGameState(iKeyCode key);
    void onEnterGameState();
    void onLeaveGameState();
    void onHandleGameState();
    void onRenderGameState();

    void onRender();
    void onHandle();

    void onEnterExitState();

    void init();
    void deinit();

public:
    StateMachineExample();
    ~StateMachineExample();

    void run();
};

#endif
