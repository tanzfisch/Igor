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

#ifndef __IGOR_DEFINES__
#define __IGOR_DEFINES__

#include <ostream>

#include <iaux/data/iaString.h>
using namespace iaux;

#ifdef IGOR_MSCOMPILER
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

#ifdef IGOR_GCC
#define IGOR_API
#define IGOR_API_EXPORT_ONLY
#endif

extern const iaString IGOR_BUILD_CONFIGURATION; //! Igor build configuration short
extern const std::vector<iaString> IGOR_SUPPORTED_SPRITE_EXTENSIONS; //! supported file extensions for sprites
extern const std::vector<iaString> IGOR_SUPPORTED_TEXTURE_EXTENSIONS; //! supported file extensions for textures
extern const std::vector<iaString> IGOR_SUPPORTED_SHADER_EXTENSIONS; //! supported file extensions for shaders
extern const std::vector<iaString> IGOR_SUPPORTED_MATERIAL_EXTENSIONS; //! supported file extensions for materials
extern const std::vector<iaString> IGOR_SUPPORTED_ANIMATION_EXTENSIONS; //! supported file extensions for animations
extern const std::vector<iaString> IGOR_SUPPORTED_MODEL_EXTENSIONS; //! supported file extensions for models
extern const std::vector<iaString> IGOR_SUPPORTED_SOUND_EXTENSIONS; //! supported file extensions for sounds

// resources
extern const iaString IGOR_RESOURCE_SHADER; //! shader resource name
extern const iaString IGOR_RESOURCE_MATERIAL; //! material resource name
extern const iaString IGOR_RESOURCE_ANIMATION; //! animation resource name
extern const iaString IGOR_RESOURCE_TEXTURE; //! texture resource name
extern const iaString IGOR_RESOURCE_SOUND; //! sound resource name
extern const iaString IGOR_RESOURCE_SPRITE; //! sprite resource name
extern const iaString IGOR_RESOURCE_MODEL; //! model resource name

// resource parameters
extern const iaString IGOR_RESOURCE_PARAM_ID; //! resource parameters id
extern const iaString IGOR_RESOURCE_PARAM_ALIAS; //! resource parameters alias
extern const iaString IGOR_RESOURCE_PARAM_TYPE; //! resource parameters type
extern const iaString IGOR_RESOURCE_PARAM_CACHE_MODE; //! resource parameters cache mode
extern const iaString IGOR_RESOURCE_PARAM_SOURCE; //! resource parameters filename
extern const iaString IGOR_RESOURCE_PARAM_PIXMAP; //! resource parameters pixmap
extern const iaString IGOR_RESOURCE_PARAM_NODE; //! resource parameters node
extern const iaString IGOR_RESOURCE_PARAM_EXPORT_MODE; //! resource parameters export mode
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE_BUILD_MODE; //! resource parameters texture build mode
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE_WRAP_MODE; //! resource parameters texture build mode
extern const iaString IGOR_RESOURCE_PARAM_JOIN_VERTICES; //! resource parameters join vertices
extern const iaString IGOR_RESOURCE_PARAM_SUB_TYPE; //! resource parameters sub type
extern const iaString IGOR_RESOURCE_PARAM_QUIET; //! resource parameters quiet
extern const iaString IGOR_RESOURCE_PARAM_SHADER; //! resource parameters shader material
extern const iaString IGOR_RESOURCE_PARAM_MATERIAL; //! resource parameters material
extern const iaString IGOR_RESOURCE_PARAM_PHYSICS_MATERIAL; //! resource parameters physics material
extern const iaString IGOR_RESOURCE_PARAM_LOD; //! resource parameters level of detail
extern const iaString IGOR_RESOURCE_PARAM_KEEP_MESH; //! resource parameters keep mesh
extern const iaString IGOR_RESOURCE_PARAM_GENERATE; //! resource parameters generate
extern const iaString IGOR_RESOURCE_PARAM_SEED; //! resource parameters seed
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE; //! resource parameters texture
extern const iaString IGOR_RESOURCE_PARAM_EMISSIVE; //! resource parameters emissive
extern const iaString IGOR_RESOURCE_PARAM_AMBIENT; //! resource parameters ambient
extern const iaString IGOR_RESOURCE_PARAM_DIFFUSE; //! resource parameters diffuse
extern const iaString IGOR_RESOURCE_PARAM_SPECULAR; //! resource parameters specular
extern const iaString IGOR_RESOURCE_PARAM_SHININESS; //! resource parameters shininess
extern const iaString IGOR_RESOURCE_PARAM_ALPHA; //! resource parameters alpha
extern const iaString IGOR_RESOURCE_PARAM_TILING; //! resource parameters tiling

extern const iaString IGOR_RESOURCE_PARAM_TEXTURE0; //! resource parameters texture unit 0
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE1; //! resource parameters texture unit 1
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE2; //! resource parameters texture unit 2
extern const iaString IGOR_RESOURCE_PARAM_TEXTURE3; //! resource parameters texture unit 3

namespace igor
{

    /*! save mode for exporting node graphs
     */
    enum class iSaveMode
    {
        /*! keeps external references unchanged
         */
        KeepExternals,

        /*! converts external data and embeds it in to one file
         */
        EmbedExternals
    };

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

    /*! A flag specifying the character of the material

    The application is free to interpret it any way it likes.

    In Mica for example we only display public materials in the material list
    */
    enum class iMaterialVisibility
    {
        Private,
        Public
    };

    /*! prints the material visibility to a stream

    \param stream the stream to log to
    \param visibility the value to log
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iMaterialVisibility &visibility);

    /*! color format of textures pixmaps etc.
     */
    enum class iColorFormat
    {
        Undefined,
        RGB,
        RGBA
    };

    /*! prints the color format to the console

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

    /*! prints the texture build mode to the console

    \param stream the stream to log to
    \param buildMode the texture build mode
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iTextureBuildMode &buildMode);

    /*! texture wrap mode
     */
    enum class iTextureWrapMode
    {
        Repeat,
        Clamp,
        MirrorRepeat
    };

    /*! prints the texture wrap mode to the console

    \param stream the stream to log to
    \param wrapMode the texture wrap mode
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, const iTextureWrapMode &wrapMode);

    /*! texture patterns
     */
    enum class iTexturePattern
    {
        SolidColor,
        CheckerBoard
    };

    /*! resource manager load mode
     */
    enum class iResourceManagerLoadMode
    {
        Application,
        Synchronized
    };

    /*! prints the resource manager load mode in the console

    \param stream the stream to log to
    \param mode the resource manager load mode mode
    \returns the stream
    */
    IAUX_API std::wostream &operator<<(std::wostream &stream, iResourceManagerLoadMode mode);

    /*! cache mode for resources
     */
    enum class iResourceCacheMode
    {
        DontCache, //! don't put in cache and forget about it
        Free,      //! free immediately after not in use
        Cache,     //! free when cache is flushed
        Keep,      //! keep until corresponding resource manager was destroyed
    };

    /*! prints the resource cache mode to the console

    \param stream the stream to log to
    \param cacheMode the resource cache mode
    \returns the stream
    */
    IGOR_API std::wostream &operator<<(std::wostream &stream, iResourceCacheMode cacheMode);

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

#endif // __IGOR_DEFINES__
