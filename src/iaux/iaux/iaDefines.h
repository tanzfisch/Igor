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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __IAUX_DEFINES__
#define __IAUX_DEFINES__

#include <string.h>
#include <stdint.h>

// detect environment
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#ifdef _WIN64
#define __IGOR_WINDOWS__
#ifndef _UNICODE
#error Igor needs unicode!
#endif
#else
#error Igor currently does not support win32
#endif
#elif __linux__
#define __IGOR_LINUX__
#else
#error unsupported environment
#endif

#ifdef __IGOR_WINDOWS__
#define __IGOR_PATHSEPARATOR__ '\\'
#endif
#ifdef __IGOR_LINUX__
#define __IGOR_PATHSEPARATOR__ '/'
#endif

#define S1(x) #x
#define S2(x) S1(x)
#define __IGOR_FILE_LINE__ __FILE__ ":" S2(__LINE__)

#define __IGOR_STDCALL__ stdcall
#define __IGOR_CDECL__ cdecl
#define __IGOR_DEFAULTCALL__ __IGOR_CDECL__

// configure windows environment
#ifdef __IGOR_WINDOWS__

// Some Information arround __func__ __FUNCSIG__ and __FUNCTION__
// http://stackoverflow.com/questions/4384765/whats-the-difference-between-pretty-function-function-func
#define __IGOR_FUNCTION__ __FUNCTION__ "(...)"

#ifdef _DEBUG
#define __IGOR_DEBUG__
#define __IGOR_INLINE__ __inline
#else
#define __IGOR_INLINE__ __forceinline
#endif

#define __IGOR_DISABLE_WARNING__(num) __pragma(warning(disable \
                                                       : num))
#define __IGOR_ENABLE_WARNING__(num) __pragma(warning(default \
                                                      : num))
#ifdef _MSC_VER // msvc
#define __IGOR_MSCOMPILER__

#ifdef __IAUX_BUILDING_DLL__
#define IgorAux_API __declspec(dllexport)
#define IgorAux_API_Template __declspec(dllexport)
#else
#define IgorAux_API __declspec(dllimport)
#define IgorAux_API_Template
#endif

#define __IGOR_FUNCTION_POINTER__(name, returntype, parameters) typedef returntype(__CLRCALL_OR_CDECL *name) parameters
#define __IGOR_MEMBERFUNCTION_POINTER__(classname, name, returntype, parameters) typedef returntype(classname::*name) parameters
#endif

#endif // __IGOR_WINDOWS__

// configure linux environment
#ifdef __IGOR_LINUX__

#define __IGOR_FUNCTION__ __PRETTY_FUNCTION__

#if defined(DEBUG) || defined(_DEBUG)
#define __IGOR_DEBUG__
#define __IGOR_INLINE__ inline
#else
#define __IGOR_INLINE__ inline
#endif

#define __IGOR_DISABLE_WARNING__(num) // TODO
#define __IGOR_ENABLE_WARNING__(num)  // TODO

#define IgorAux_API
#define IgorAux_API_Template

#define __IGOR_FUNCTION_POINTER__(name, returntype, parameters) typedef returntype(*name) parameters
#define __IGOR_MEMBERFUNCTION_POINTER__(classname, name, returntype, parameters) typedef returntype(classname::*name) parameters

#define __IGOR_SEPARATOR__ '/'

#endif // __IGOR_LINUX__

#ifndef int8
typedef int8_t int8;
#endif

#ifndef int16
typedef int16_t int16;
#endif

#ifndef int32
typedef int32_t int32;
#endif

#ifndef int64
typedef int64_t int64;
#endif

#ifndef uint8
typedef uint8_t uint8;
#endif

#ifndef uint16
typedef uint16_t uint16;
#endif

#ifndef uint32
typedef uint32_t uint32;
#endif

#ifndef uint64
typedef uint64_t uint64;
#endif

#ifndef float32
typedef float float32;
#endif

#ifndef float64
typedef double float64;
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
#define M_E 2.71828182845904523536
#define M_LOG2E 1.44269504088896340736
#define M_LOG10E 0.434294481903251827651
#define M_LN2 0.693147180559945309417
#define M_LN10 2.30258509299404568402
#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923
#define M_PI_4 0.785398163397448309616
#define M_1_PI 0.318309886183790671538
#define M_2_PI 0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2 1.41421356237309504880
#define M_SQRT1_2 0.707106781186547524401

#endif

namespace iaux
{
    /*! axis definition
    */
    enum class iaAxis
    {
        X,
        Y,
        Z
    };
} // namespace iaux

#ifdef __IGOR_DEBUG__
#define __IAUX_CONFIG_STR__ debug
#else
#define __IAUX_CONFIG_STR__ release
#endif

/*! igor tab definition
*/
#define __IGOR_TAB__ L"    "

//! helper macro to handle scrings in macros
#define STR2(x) #x
//! an other helper macro to handle scrings in macros
#define STR(x) STR2(x)

/*! definitions of gramm
*/
#define __IGOR_GRAMM__ 0.001

/*! definitions of kilo gramm
*/
#define __IGOR_KILO__ 1.0

/*! definitions of ton
*/
#define __IGOR_TON__ 1000.0

/*! base weight
*/
#define __IGOR_BASE_WEIGHT__ __IGOR_KILO__

/*! definition of milimeter
*/
#define __IGOR_MILIMETER__ 0.001

/*! definition of centimeter
*/
#define __IGOR_CENTIMETER__ 0.01

/*! definition of decimeter
*/
#define __IGOR_DECIMETER__ 0.1

/*! definition of one meter
*/
#define __IGOR_METER__ 1.0

/*! definition of kilometer
*/
#define __IGOR_KILOMETER__ 1000.0

/*! base lenght
*/
#define __IGOR_BASE_LENGHT__ __IGOR_METER__

/*! definition of millisecond
*/
#define __IGOR_MILLISECOND__ 1.0

/*! definition of second
*/
#define __IGOR_SECOND__ 1000.0

/*! definition of minute
*/
#define __IGOR_MINUTE__ 60000.0

/*! base time
*/
#define __IGOR_BASE_TIME__ __IGOR_MILLISECOND__

/*! default gravity in m/s�
*/
#define __IGOR_GRAVITY__ 9.81

#define __IGOR_BIT_0__ 0x00000001
#define __IGOR_BIT_1__ 0x00000002
#define __IGOR_BIT_2__ 0x00000004
#define __IGOR_BIT_3__ 0x00000008
#define __IGOR_BIT_4__ 0x00000010
#define __IGOR_BIT_5__ 0x00000020
#define __IGOR_BIT_6__ 0x00000040
#define __IGOR_BIT_7__ 0x00000080

#define __IGOR_BIT_8__ 0x00000100
#define __IGOR_BIT_9__ 0x00000200
#define __IGOR_BIT_10__ 0x00000400
#define __IGOR_BIT_11__ 0x00000800
#define __IGOR_BIT_12__ 0x00001000
#define __IGOR_BIT_13__ 0x00002000
#define __IGOR_BIT_14__ 0x00004000
#define __IGOR_BIT_15__ 0x00008000

#define __IGOR_BIT_16__ 0x00010000
#define __IGOR_BIT_17__ 0x00020000
#define __IGOR_BIT_18__ 0x00040000
#define __IGOR_BIT_19__ 0x00080000
#define __IGOR_BIT_20__ 0x00100000
#define __IGOR_BIT_21__ 0x00200000
#define __IGOR_BIT_22__ 0x00400000
#define __IGOR_BIT_23__ 0x00800000

#define __IGOR_BIT_24__ 0x01000000
#define __IGOR_BIT_25__ 0x02000000
#define __IGOR_BIT_26__ 0x04000000
#define __IGOR_BIT_27__ 0x08000000
#define __IGOR_BIT_28__ 0x10000000
#define __IGOR_BIT_29__ 0x20000000
#define __IGOR_BIT_30__ 0x40000000
#define __IGOR_BIT_31__ 0x80000000

#define IGOR_INVALID_ID 0

// id types
typedef uint64 iaID64;
typedef uint32 iaID32;
typedef uint16 iaID16;
typedef uint8 iaID8;

#endif
