// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iKeyCodeMap.h>
#include <iaConsole.h>

namespace Igor
{
    iKeyCodeMap::iKeyCodeMap()
    {
        _keyNames = map<iKeyCode, iaString>{
            { iKeyCode::Undefined, L"Undefined" },
            { iKeyCode::Backspace, L"Backspace" },
            { iKeyCode::Tab, L"Tab" },
            { iKeyCode::Clear, L"Clear" },
            { iKeyCode::Return, L"Return" },
            { iKeyCode::LShift, L"LShift" },
            { iKeyCode::Shift, L"Shift" },
            { iKeyCode::Control, L"Control" },
            { iKeyCode::LControl, L"LControl" },
            { iKeyCode::LAlt, L"LAlt" },
            { iKeyCode::Break, L"Break" },
            { iKeyCode::CapsLock, L"CapsLock" },
            { iKeyCode::ESC, L"ESC" },
            { iKeyCode::Space, L"Space" },
            { iKeyCode::PgUp, L"PgUp" },
            { iKeyCode::PgDown, L"PgDown" },
            { iKeyCode::End, L"End" },
            { iKeyCode::Home, L"Home" },
            { iKeyCode::Left, L"Left" },
            { iKeyCode::Up, L"Up" },
            { iKeyCode::Right, L"Right" },
            { iKeyCode::Down, L"Down" },
            { iKeyCode::Insert, L"Insert" },
            { iKeyCode::Delete, L"Delete" },
            { iKeyCode::Zero, L"0" },
            { iKeyCode::One, L"1" },
            { iKeyCode::Two, L"2" },
            { iKeyCode::Three, L"3" },
            { iKeyCode::Four, L"4" },
            { iKeyCode::Five, L"5" },
            { iKeyCode::Six, L"6" },
            { iKeyCode::Seven, L"7" },
            { iKeyCode::Eight, L"8" },
            { iKeyCode::Nine, L"9" },
            { iKeyCode::A, L"A" },
            { iKeyCode::B, L"B" },
            { iKeyCode::C, L"C" },
            { iKeyCode::D, L"D" },
            { iKeyCode::E, L"E" },
            { iKeyCode::F, L"F" },
            { iKeyCode::G, L"G" },
            { iKeyCode::H, L"H" },
            { iKeyCode::I, L"I" },
            { iKeyCode::J, L"J" },
            { iKeyCode::K, L"K" },
            { iKeyCode::L, L"L" },
            { iKeyCode::M, L"M" },
            { iKeyCode::N, L"N" },
            { iKeyCode::O, L"O" },
            { iKeyCode::P, L"P" },
            { iKeyCode::Q, L"Q" },
            { iKeyCode::R, L"R" },
            { iKeyCode::S, L"S" },
            { iKeyCode::T, L"T" },
            { iKeyCode::U, L"U" },
            { iKeyCode::V, L"V" },
            { iKeyCode::W, L"W" },
            { iKeyCode::X, L"X" },
            { iKeyCode::Y, L"Y" },
            { iKeyCode::Z, L"Z" },
            { iKeyCode::WindowsLeft, L"WindowsLeft" },
            { iKeyCode::WindowsRight, L"WindowsRight" },
            { iKeyCode::WindowsContext, L"WindowsContext" },
            { iKeyCode::NP_Zero, L"NP_0" },
            { iKeyCode::NP_One, L"NP_1" },
            { iKeyCode::NP_Two, L"NP_2" },
            { iKeyCode::NP_Three, L"NP_3" },
            { iKeyCode::NP_Four, L"NP_4" },
            { iKeyCode::NP_Five, L"NP_5" },
            { iKeyCode::NP_Six, L"NP_6" },
            { iKeyCode::NP_Seven, L"NP_7" },
            { iKeyCode::NP_Eight, L"NP_8" },
            { iKeyCode::NP_Nine, L"NP_9" },
            { iKeyCode::NP_Multiply, L"NP_Multiply" },
            { iKeyCode::NP_Add, L"NP_Add" },
            { iKeyCode::NP_Subtract, L"NP_Subtract" },
            { iKeyCode::NP_Decimal, L"NP_Decimal" },
            { iKeyCode::NP_Divide, L"NP_Divide" },
            { iKeyCode::F1, L"F1" },
            { iKeyCode::F2, L"F2" },
            { iKeyCode::F3, L"F3" },
            { iKeyCode::F4, L"F4" },
            { iKeyCode::F5, L"F5" },
            { iKeyCode::F6, L"F6" },
            { iKeyCode::F7, L"F7" },
            { iKeyCode::F8, L"F8" },
            { iKeyCode::F9, L"F9" },
            { iKeyCode::F10, L"F10" },
            { iKeyCode::F11, L"F11" },
            { iKeyCode::F12, L"F12" },
            { iKeyCode::F13, L"F13" },
            { iKeyCode::F14, L"F14" },
            { iKeyCode::F15, L"F15" },
            { iKeyCode::F16, L"F16" },
            { iKeyCode::F17, L"F17" },
            { iKeyCode::F18, L"F18" },
            { iKeyCode::F19, L"F19" },
            { iKeyCode::F20, L"F20" },
            { iKeyCode::F21, L"F21" },
            { iKeyCode::F22, L"F22" },
            { iKeyCode::F23, L"F23" },
            { iKeyCode::F24, L"F24" },
            { iKeyCode::NumLock, L"NumLock" },
            { iKeyCode::ScrollLock, L"ScrollLock" },
            { iKeyCode::OEM1, L"OEM1" },
            { iKeyCode::OEMPlus, L"OEMPlus" },
            { iKeyCode::OEMComma, L"OEMComma" },
            { iKeyCode::OEMMinus, L"OEMMinus" },
            { iKeyCode::OEMPeriod, L"OEMPeriod" },
            { iKeyCode::OEM2, L"OEM2" },
            { iKeyCode::OEM3, L"OEM3" },
            { iKeyCode::OEM4, L"OEM4" },
            { iKeyCode::OEM5, L"OEM5" },
            { iKeyCode::OEM6, L"OEM6" },
            { iKeyCode::OEM7, L"OEM7" },
            { iKeyCode::OEM102, L"OEM102" },
            { iKeyCode::RControl, L"RControl" },
            { iKeyCode::AltGr, L"AltGr" },
            { iKeyCode::Alt, L"Alt" },
            { iKeyCode::Print, L"Print" },
            { iKeyCode::RShift, L"RShift" },
            { iKeyCode::Enter, L"Enter" },
            { iKeyCode::MouseLeft, L"MouseLeft" },
            { iKeyCode::MouseMiddle, L"MouseMiddle" },
            { iKeyCode::MouseRight, L"MouseRight" },
            { iKeyCode::MouseWheelUp, L"MouseWheelUp" },
            { iKeyCode::MouseButton4, L"MouseButton4" },
            { iKeyCode::MouseButton5, L"MouseButton5" },
            { iKeyCode::MouseWheelDown, L"MouseWheelDown" }
        };
    }

    iKeyCodeMap::~iKeyCodeMap()
    {
        _keyNames.clear();
    }

    iaString iKeyCodeMap::getKeyName(iKeyCode keyCode) const
    {
        return _keyNames.at(keyCode);
    }

    iKeyCode iKeyCodeMap::getKeyCode(const iaString& keyName) const
    {
        //! \todo maybe an oprtunity to try out lamda expression?!
        auto iter = _keyNames.begin();
        while (iter != _keyNames.end())
        {
            if ((*iter).second == keyName)
            {
                return (*iter).first;
            }

            ++iter;
        }

        return iKeyCode::Undefined;
    }

};