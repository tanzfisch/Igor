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

#ifndef __IGOR_AUX_FLUSHVECTOR__
#define __IGOR_AUX_FLUSHVECTOR__

#include <iaConsole.h>

#include <vector>
using namespace std;

namespace IgorAux
{

	/*! flush list

	every add or remove command will have no effect until flush
    interface is not thread save

    \deprecated do we need this anymore?
	*/
	template<class T> class IgorAux_API_Template iaFlushVector
	{

	public:

		/*! ctor

		does nothing
		*/
        iaFlushVector();

		/*! dtor

		clears lists
		*/
        virtual ~iaFlushVector();

		/*! returns reference to _data

		\return reference to _data
		*/
        vector<T>& getList();

		/*! add element to flush queue for insertion

		\param element new element
		*/
        void add(T element);

		/*! add element to flush queue for removal

		\param element remove element
		*/
        void remove(T element);

		/*! flush the queue

		inserts and removes elements according to _queue in _data
		*/
        void iaFlushVector::flush();

	private:

		/*! queue to flush
		*/
        vector<pair<T, bool>> _queue;

		/*! concrete _data
		*/
		vector<T> _data;

	};

#include <iaFlushVector.inl>

};

#endif