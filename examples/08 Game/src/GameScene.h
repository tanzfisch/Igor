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

#ifndef __GAMESCENE__
#define __GAMESCENE__

#include <iNodeCamera.h>
#include <iNodeTransform.h>
#include <iScene.h>
#include <iNodeModel.h>
#include <iNodeLight.h>
using namespace Igor;

#include <map>
using namespace std;

class GameControl;

class GameScene
{
private:

    const uint32 _directionIndex = 0;
    GameControl* _gameControl = nullptr;

    iScene* _scene = nullptr;

    iNodeCamera* _camera = nullptr;
    iNodeTransform* _cameraHeading = nullptr;
    iNodeTransform* _cameraPitch = nullptr;
    iNodeTransform* _cameraDistance = nullptr;

    map<uint32, iNodeTransform*> _moveables;

    iNodeTransform* _directionalLightTranslate = 0;
    iNodeTransform* _directionalLightRotate = 0;
    iNodeLight* _lightNode = 0;

    void init();
    void initTiles();
    void initMoveables();
    void updateMoveables();
    void deinit();

public:

    iScene* getScene();

    void onHandle();

    GameScene(GameControl* gameControl);
    ~GameScene();
};

#endif
