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

#ifndef __GAMECORE__
#define __GAMECORE__

#include <iView.h>
#include <iKeyCodeMap.h>

namespace Igor
{
    class iNodeCamera;
    class iNodeTransform;
    class iNodeLight;
    class iScene;
    class iWindow;
    class iPhysicsBody;
}

using namespace Igor;

class Player;
class Enemy;

class GameCore
{

public:

    GameCore(iWindow* window);
    ~GameCore();

    bool isRunning();

private:

    bool _isRunning = false;

    iWindow* _parentWindow = nullptr;
    iView _view;

    iScene* _scene = nullptr;

    iNodeCamera* _camera = nullptr;
    iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraDistance = nullptr;

    iNodeTransform* _directionalLightTranslate = nullptr;
    iNodeTransform* _directionalLightRotate = nullptr;
    iNodeLight* _lightNode = nullptr;

    Player* _player = nullptr;
    vector<Enemy*> _enemies;

    void init();
    void initPlayer();

    void deinit();
    void deinitPlayer();

    void onKeyPressed(iKeyCode key);
    void onKeyReleased(iKeyCode key);

    void onHandle();

};

#endif