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

#ifndef __GAMECONTROL__
#define __GAMECONTROL__

#include <iPhysicsBody.h>
using namespace Igor;

#include "Tiles.h"

#include <map>
#include <memory>
using namespace std;

enum class GameAction
{
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    PrimaryAction,
    SecondaryAction
};

class GameControl
{
private:

    const uint32 _directionIndex = 0;
    
    iPhysicsBody* _floor = nullptr;
    Tiles* _tiles = nullptr;
    uint32 _player = 0;
    bool _isRunning = false;

public:

    Tiles* getTiles();
    uint32 getPlayer();

    void activateEntityAction(uint32 id, GameAction action);
    void deactivateEntityAction(uint32 id, GameAction action);

    void exitGame();

    bool onHandle();

    GameControl();
    ~GameControl();

};

#endif
