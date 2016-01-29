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

#ifndef __IGOR_AUX_DEFINES__
#define __IGOR_AUX_DEFINES__

#ifdef _WIN64
#define __IGOR_WIN__
#define __IGOR_WIN64__
#define __IGOR_X64__
#else
#ifdef _WIN32
#define __IGOR_WIN__
#define __IGOR_WIN32__
#define __IGOR_X32__
#endif
#endif

#ifdef __IGOR_WIN__

#ifdef _DEBUG
#define __IGOR_DEBUG__
#define __IGOR_INLINE__ __inline
#else
#define __IGOR_INLINE__ __forceinline
#endif

#define IGOR_DISABLE_WARNING(num) __pragma(warning(disable:num))
#define IGOR_ENABLE_WARNING(num) __pragma(warning(default:num))

    /*!
    Some Information arround __func__ __FUNCSIG__ and __FUNCTION__
    http://stackoverflow.com/questions/4384765/whats-the-difference-between-pretty-function-function-func
    */
#ifdef __FUNCTION__
#define __IGOR_FUNCTION__ __FUNCTION__ "(...)"
#else
#define __IGOR_FUNCTION__ "unknown(...)"
#endif

#define __IGOR_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __IGOR_LINE__ __LINE__
#define __IGOR_FILE_LINE__ __IGOR_FILE__ << " (" << __IGOR_LINE__ << ")"

#ifdef _MSC_VER // msvc
#define __IGOR_MSCOMPILER__

#ifdef __IGOR_AUX_BUILDING_DLL__
#define IgorAux_API __declspec(dllexport)
#define IgorAux_API_Template __declspec(dllexport)
#else
#define IgorAux_API __declspec(dllimport)
#define IgorAux_API_Template
#endif

#define __IGOR_FUNCTION_POINTER__(name,callconvention,returntype,parameters) typedef returntype (callconvention *name)parameters
#define __IGOR_MEMBERFUNCTION_POINTER__(classname,name,returntype,parameters) typedef returntype (classname::*name)parameters

#define __IGOR_STDCALL__ __stdcall
#define __IGOR_CDECL__ __cdecl
#define __IGOR_DEFAULTCALL__ __IGOR_CDECL__

#ifndef int8
typedef __int8 int8;
#endif

#ifndef int16
typedef __int16 int16;
#endif

#ifndef int32
typedef __int32 int32;
#endif

#ifndef int64
typedef __int64 int64;
#endif

#ifndef uint8
typedef	unsigned  __int8 uint8;
#endif

#ifndef uint16
typedef	unsigned __int16 uint16;
#endif

#ifndef uint32
typedef	unsigned __int32 uint32;
#endif

#ifndef uint64
typedef	unsigned __int64 uint64;
#endif	

#ifndef uint
typedef	unsigned int uint;
#endif	

#ifndef float32
typedef float float32;
#endif

#ifndef float64
typedef double float64;
#endif
#endif

#else
#error missing definitions for this environment
#endif

#ifndef _USE_MATH_DEFINES

    /* Define _USE_MATH_DEFINES before including math.h to expose these macro
    * definitions for common math constants.  These are placed under an #ifdef
    * since these commonly-defined names are not part of the C/C++ standards.
    */

    /* Definitions of useful mathematical constants
    * M_E        - e
    * M_LOG2E    - log2(e)
    * M_LOG10E   - log10(e)
    * M_LN2      - ln(2)
    * M_LN10     - ln(10)
    * M_PI       - pi
    * M_PI_2     - pi/2
    * M_PI_4     - pi/4
    * M_1_PI     - 1/pi
    * M_2_PI     - 2/pi
    * M_2_SQRTPI - 2/sqrt(pi)
    * M_SQRT2    - sqrt(2)
    * M_SQRT1_2  - 1/sqrt(2)
    */

#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401

#endif

namespace IgorAux
{
    /*! axis definition
    */
    enum class iaAxis
    {
        X,
        Y,
        Z
    };
}

#ifdef __IGOR_DEBUG__
#define __IGOR_AUX_CONFIG_STR__ debug
#else
#define __IGOR_AUX_CONFIG_STR__ release
#endif

#ifdef __IGOR_X64__
#define __IGOR_AUX_BIT_STR__ x64
#endif

#ifdef __IGOR_X32__
#define __IGOR_AUX_BIT_STR__ x32
#endif

#define __IGOR_TAB__ L"    "

//! helper macro to handle scrings in macros
#define STR2(x) #x
//! an other helper macro to handle scrings in macros
#define STR(x) STR2(x)

#define IGOR_GRAMM 0.001
#define IGOR_KILO 1.0
#define IGOR_TON 1000.0

/*! base weight
*/
#define IGOR_BASE_WEIGHT IGOR_KILO          

#define IGOR_MILIMETER 0.001
#define IGOR_CENTIMETER 0.01
#define IGOR_DEZIMETER 0.1
#define IGOR_METER 1.0
#define IGOR_KILOMETER 1000.0

/*! base lenght
*/
#define IGOR_BASE_LENGHT IGOR_METER

#define IGOR_MILLISECOND 1.0
#define IGOR_SECOND 1000.0

/*! base time
*/
#define IGOR_BASE_TIME IGOR_MILLISECOND

/*! default gravity in m/s²
*/
#define IGOR_GRAVITY 9.81

#ifndef _UNICODE
#error Igor needs unicode!
#endif

#endif