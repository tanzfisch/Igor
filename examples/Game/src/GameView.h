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

#ifndef __GAMEFRONTEND__
#define __GAMEFRONTEND__

#include "GameControl.h"
#include <iView.h>
#include <iWindow.h>
using namespace Igor;

#include <iaVector2.h>
using namespace IgorAux;

class GameScene;

class GameView
{
private:

    GameControl* _gameControl = nullptr;
    iWindow* _window = nullptr;
    iView view;

    GameScene* _gameScene = nullptr;
    
    /*! camera position in tile coordinates
    */
    iaVector2f _cameraPosition;

    const uint32 _scaleX = 20;
    const uint32 _scaleY = 20;

public:

    GameView(GameControl* gameControl, iWindow* window);
    ~GameView();
};

#endif