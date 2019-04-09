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
//    (c) Copyright 2014-2015 by Martin Loga
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

#include <iWindow.h>
#include <iView.h>
#include <iKeyboard.h>
#include <iTextureFont.h>
#include <iTexture.h>
#include <iParticleSystem2D.h>
using namespace Igor;

#include <iaStateMachine.h>
#include <iaString.h>
using namespace IgorAux;

#include <vector>
using namespace std;

class StateMachineExample
{
    
private:

	iWindow _window;
	iView _view;

    iTextureFont* _font = 0;
    int32 _materialWithTextureAndBlending = -1;
    iParticleSystem2D _particleSystems[5];
    shared_ptr<iTexture> _particleTexture;

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

    float64 _time = 0.0;

    iaString _backgroundText = "";
    float32 _spinning = 0.0f;
    int32 _activeMenuEntry = 0;
    vector<iaString> _menuEntries;

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
