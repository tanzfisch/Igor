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
// (c) Copyright 2014-2016 by Martin Loga
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

#ifndef __iCONTROL__
#define __iCONTROL__

#include <iDefines.h>
#include <iKeyCodeMap.h>

#include <map>
using namespace std;

namespace Igor
{

    class iWindow;

    /*! transformation directions
    */
    enum class iTransformDirection
    {
        Foreward,
        Backward,
        Left,
        Right,
        Up,
        Down,
        RotateLeft,
        RotateRight,
        RotateUp,
        RotateDown,
        RollLeft,
        RollRight,
        MouseHV
    };

    /*! abstract base class to control "something" with keyboard and mouse
    */
    class Igor_API iControl
    {

    public:

        /*! initializes member variables

        \param initDefaultLayout if true a default keyboard layout will ne initialized
        */
        iControl(bool initDefaultLayout = true);

        /*! release handles
        */
        virtual ~iControl();

        /*! start moving in specified direction

        \param direction the direction specified
        */
        void start(iTransformDirection direction);

        /*! stop moving in specified direction

        \param direction the direction specified
        */
        void stop(iTransformDirection direction);

        /*! sets factor of specified direction

        could be speed, acceleration, force or what ever and depends in implementation of deriving class

        \param direction the direction specified
        \param speed the speed to move in specified direction
        */
        void setFactor(iTransformDirection direction, float32 speed);

        /*! \returns the factor of specified direction

        \param direction the direction specified
        */
        float32 getFactor(iTransformDirection direction);

        /*! binds a key to a direction

        \param key the key to bind to a direction
        \param dir the direction to bind the key to
        */
        void setKeyBinding(iKeyCode key, iTransformDirection dir);

        /*! removes a key direction binding

        \param key the key binding to remove
        */
        void removeKeyBinding(iKeyCode key);

    protected:

        /*! map of directions and which are active
        */
        map<iTransformDirection, bool> _activeDir;

        /*! map of directions and what speed if configured for them
        */
        map<iTransformDirection, float32> _factorDir;

        /*! map of key bindings to directions
        */
        map<iKeyCode, iTransformDirection> _keyMapping;

        /*! called every frame
        */
        virtual void onHandle() = 0;

        /*! called when mouse moves
        \param x1 last horrizontal position of mouse
        \param y1 last vertical position of mouse
        \param x2 current horrizontal position of mouse
        \param y2 current vertical position of mouse
        */
        //void onMouseMove(int32 x1, int32 y1, int32 x2, int32 y2, iWindow* window);

        /*! called on mouse key down

        \param key key id of mouse key pressed
        */
        void onKeyDown(iKeyCode key);

        /*! called on mouse key up

        \param key key id of mouse key released
        */
        void onKeyUp(iKeyCode key);

    };

}
#endif
