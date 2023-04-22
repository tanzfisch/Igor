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
// (c) Copyright 2012-2023 by Martin Loga
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
// contact: igorgameengine@protonmail.com

#ifndef __IGOR_DEFINES_H__
#define __IGOR_DEFINES_H__

#include <ostream>

#include <iaux/data/iaString.h>
using namespace iaux;

#ifdef __IGOR_MSCOMPILER__
#ifdef __IGOR_BUILDING_DLL__
#define IGOR_API __declspec(dllexport)
#define IGOR_API_EXPORT_ONLY __declspec(dllexport)
#define IGOR_API_IMPORT_ONLY
#else
#define IGOR_API __declspec(dllimport)
#define IGOR_API_EXPORT_ONLY
#define IGOR_API_IMPORT_ONLY __declspec(dllimport)
#endif
#endif

#ifdef __IGOR_GCC__
#define IGOR_API
#define IGOR_API_EXPORT_ONLY
#endif

//! Igor configuration short
extern const iaString __IGOR_CONFIGURATION__;

#define IGOR_MAX_BEHAVIORS_PER_COMPONENT 10

namespace igor
{
    /*! horizontal alignment relative to parent widget
     */
    enum class iHorizontalAlignment
    {
        Left,
        Center,
        Right,
        Absolute,
        Stretch, // currently only supported by dialogs
        Undefined
    };

    /*! vertical alignment relative to parent widget
     */
    enum class iVerticalAlignment
    {
        Top,
        Center,
        Bottom,
        Absolute,
        Stretch, // currently only supported by dialogs
        Undefined
    };

    /*! grid/cell selection mode
     */
    enum class iSelectionMode
    {
        Cell,
        Row,
        Column,
        NoSelection
    };

    /*! color format of textures pixmaps etc.
     */
    enum class iColorFormat
    {
        Undefined,
        RGB,
        RGBA
    };

    /*! prints the color format in the console

    \param stream the stream to log to
    \param colorFormat the color format
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iColorFormat &colorFormat);    

    /*! how to generate a texture
     */
    enum class iTextureBuildMode
    {
        Normal,
        Mipmapped
    };

    /*! texture wrap mode
     */
    enum class iTextureWrapMode
    {
        Repeat,
        Clamp,
        MirrorRepeat
    };

    /*! cache mode for resources
     */
    enum class iResourceCacheMode
    {
        Free = 0,  //! free immidiatly after not in use
        Cache = 1, //! free when cache is flushed
        Keep = 2   //! keep until corresponding resource manager was destroyed
    };

    /*! Igor key codes
     */
    enum class iKeyCode
    {
        Undefined,
        Backspace,
        Tab,
        Return,
        LShift,
        LControl,
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
        MouseButton4,
        MouseButton5,
        MouseWheelUp,
        MouseWheelDown,
        KeyCodeCount
    };
    
    /*! user data type
     */
    typedef void *iUserData;

}; // namespace igor

#endif // __IGOR_DEFINES_H__
