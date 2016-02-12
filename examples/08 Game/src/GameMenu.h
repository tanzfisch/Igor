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

#ifndef __GAMEMENU__
#define __GAMEMENU__

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

class GameControl;
class GameView;
class GameInput;

namespace Igor
{
    class iTaskFlushModels;
    class iTaskFlushTextures;
}

class GameMenu
{
    
private:

    // game
    GameControl* _gameControl = nullptr;
    GameView* _gameView = nullptr;
    GameInput* _gameInput = nullptr;

    // general
	iWindow window;
	iView view;
    iTaskFlushModels* _taskFlushModels = nullptr;
    iTaskFlushTextures* _taskFlushTextures = nullptr;

    iTextureFont* _font = 0;
    int32 _materialWithTextureAndBlending = -1;

    float64 _time = 0.0;

    // state machine
    iaStateMachine _stateMachine;
    uint32 _initIntroTransition = 0;
    uint32 _introMenuTransition = 0; 
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

    // menu
    int32 _activeMenuEntry = 0;
    vector<iaString> _menuEntries;

    // general events
    void onCloseWindow();
    void onRender();
    void onHandle();

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

    GameMenu();
    ~GameMenu();

	void run();
};

#endif
