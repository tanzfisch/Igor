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

#ifndef __IAUX_RANDOMNUMBERGENERATOR_H__
#define __IAUX_RANDOMNUMBERGENERATOR_H__

#include <iaux/iaDefines.h>

namespace iaux
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
		void setSeed(T seed);

		/*! \returns next random number
        */
		T getNext();

	private:
		/*! the seed
        */
		T _seed;
	};

#include <iaux/math/iaRandomNumberGenerator.inl>

	/*! uint32 random number generator
	*/
	typedef iaRandomNumberGenerator<uint32> iaRandomNumberGeneratoru;

} // namespace iaux

#endif // __IAUX_RANDOMNUMBERGENERATOR_H__
