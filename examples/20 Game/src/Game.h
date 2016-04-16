//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __GAME__
#define __GAME__

#include <iWindow.h>
#include <iView.h>
#include <iKeyCodeMap.h>
#include <iTextureFont.h>
using namespace Igor;

#include <iaStateMachine.h>
#include <iaString.h>
#include <iaTransition.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{
    class iTaskFlushModels;
    class iTaskFlushTextures;
}

class GameCore;

class Game
{
    
private:

    GameCore* _gameCore = nullptr;

    // general
	iWindow _window;
	iView _menuView;
    iTaskFlushModels* _taskFlushModels = nullptr;
    iTaskFlushTextures* _taskFlushTextures = nullptr;

    iTextureFont* _font = 0;
    int32 _materialWithTextureAndBlending = -1;

    float64 _time = 0.0;

    // state machine
    iaStateMachine _stateMachine;
    uint32 _initIntroTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _introMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _menuExitTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _menuHelpTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _helpMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _menuCreditsTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _creditsMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _menuSettingsTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _settingsMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _menuGameTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _gameMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _gameWinTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _gameLooseTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _winMenuTransition = iaTransition::INVALID_TRANSITION_ID;
    uint32 _looseMenuTransition = iaTransition::INVALID_TRANSITION_ID;

    // menu
    int32 _activeMenuEntry = 0;
    vector<iaString> _menuEntries;

    // general events
    void onCloseWindow();

    // state machine events
    void onKeyReleasedMenuState(iKeyCode key);
    void onEnterMenuState();
    void onLeaveMenuState();
    void onHandleMenuState();
    void onRenderMenuState();

    void onEnterInitState();
    void onLeaveInitState();

    void onEnterIntroState();
    void onLeaveIntroState();
    void onHandleIntroState();
    void onRenderIntroState();

    void onEnterWinState();
    void onLeaveWinState();
    void onHandleWinState();
    void onRenderWinState();

    void onEnterLooseState();
    void onHandleLooseState();

    void onEnterGameState();
    void onLeaveGameState();
    void onHandleGameState();
    void onRenderGameState();

    void onEnterExitState();

    // init deinit
	void init();
	void deinit();

public:

    Game();
    ~Game();

	void run();
};

#endif
