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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iaRANDOMNUMBERGENERATOR__
#define __iaRANDOMNUMBERGENERATOR__

#include <iaDefines.h>

namespace IgorAux
{

    /*! pseudo random number generator

    the only reason to use this one is to keep seperate instances of seeds so multithreaded tasks don't interfear with each other using rand()
    */
	template <class T>
    class IgorAux_API_Template iaRandomNumberGenerator
    {

    public:

        /*! init seed
        */
        iaRandomNumberGenerator(T seed = 0);

        /*! does nothing
        */
        ~iaRandomNumberGenerator();

        /*! sets the seed

        \param seed the seed
        */
		__IGOR_INLINE__ void setSeed(T seed);

        /*! \returns next random number
        */
		__IGOR_INLINE__ T getNext();

    private:

        /*! the seed
        */
        T _seed;

    };

	template <class T>
	iaRandomNumberGenerator<T>::iaRandomNumberGenerator(T seed)
	{
		_seed = seed;
	}

	template <class T>
	iaRandomNumberGenerator<T>::~iaRandomNumberGenerator()
	{

	}

	template <class T>
	void iaRandomNumberGenerator<T>::setSeed(T seed)
	{
		_seed = seed;
	}

	// is supposed to do the same as C++11 minstd_rand but maybe I got it wrong

	template <class T>
	__IGOR_INLINE__ T iaRandomNumberGenerator<T>::getNext()
	{
		const uint64 _a = 48271;
		const uint64 _c = 0;
		const uint64 _m = 0x7FFFFFFF;

		_seed = (static_cast<uint64>(_seed) * _a + _c) % _m;
		return _seed;
	}

#include <iaRandomNumberGenerator.inl>

	/*! uint32 random number generator
	*/
	typedef iaRandomNumberGenerator<uint32> iaRandomNumberGeneratoru;

}

#endif
