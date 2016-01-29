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

#ifndef __iKEYCODES__
#define __iKEYCODES__

#include <iaSingleton.h>
#include <iaString.h>
using namespace IgorAux;

#include <iDefines.h>

#include <map>
using namespace std;

namespace Igor
{

    /*! Igor key codes
    */
	enum class iKeyCode
	{
		Undefined,
		Backspace,
		Tab,
		Clear,
		Return,
		LShift,
		Shift,
		Control,
		LControl,
		LAlt,
		Break,
		CapsLock,
		ESC,
		Space,
		PgUp,
		PgDown,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Insert,
		Delete,
		Zero,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		WindowsLeft,
		WindowsRight,
		WindowsContext,
		NP_Zero,
		NP_One,
		NP_Two,
		NP_Three,
		NP_Four,
		NP_Five,
		NP_Six,
		NP_Seven,
		NP_Eight,
		NP_Nine,
		NP_Multiply,
		NP_Add,
		NP_Subtract,
		NP_Decimal,
		NP_Divide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		NumLock,
		ScrollLock,
		OEM1,
		OEMPlus,
		OEMComma,
		OEMMinus,
		OEMPeriod,
		OEM2,
		OEM3,
		OEM4,
		OEM5,
		OEM6,
		OEM7,
		OEM102,
		RControl,
		AltGr,
		Alt,
		Print,
		RShift,
		Enter,
		MouseLeft,
		MouseMiddle,
		MouseRight,
		MouseWheelUp,
		MouseWheelDown,
		MouseButton4,
		MouseButton5,
		KeyCodeCount
	};
	
	/*! key code map (Singleton)
	*/
	class Igor_API iKeyCodeMap : public iaSingleton<iKeyCodeMap>
	{
		friend class iaSingleton<iKeyCodeMap>;

	private:

		/*! map from code to iaString
		*/
		map<iKeyCode, iaString> _keyNames;

		/*! ctor

		initializes key maps
		*/
		iKeyCodeMap();

		/*! dtor

		does nothing
		*/
		~iKeyCodeMap();

	public:

		/*! returns key name

		\param keycode key code
		\return key name
		*/
		iaString getKeyName(iKeyCode keycode) const;

		/*! returns key code

		\param keycode key name
		\return key code
		*/
		iKeyCode getKeyCode(const iaString& keyname) const;

	};

};

#endif

