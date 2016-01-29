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

#ifndef __IGOR_AUX_FLUSHLIST__
#define __IGOR_AUX_FLUSHLIST__

#include <iDefines.h>

#include <list>
using namespace std;

namespace IgorAux
{

	/*! flush list

	every add or remove command will have no effect until flush

    \deprecated do we need this anymore?
	*/
	template<typename T> class IgorAux_API_Template iaFlushList
	{

	public:

		/*! ctor

		does nothing
		*/
		iaFlushList()
		{
		}

		/*! dtor

		clears lists
		*/
		virtual ~iaFlushList()
		{
			_flushList.clear();
			_data.clear();
		}

		/*! returns reference to _data

		\return reference to _data
		*/
		list<T>& getList()
		{
			return _data;
		}

		/*! add element to flush queue for insertion

		\param element new element
		*/
		void iaFlushList::add(T element)
		{
			_flushList.push_back(pair<T, bool>(element, true));
		}

		/*! add element to flush queue for removal

		\param element remove element
		*/
		void iaFlushList::remove(T element)
		{
			_flushList.push_back(pair<T, bool>(element, false));
		}

		/*! flush the queue

		inserts and removes elements according to _flushList in _data
		*/
		void iaFlushList::flush()
		{
			list<pair<T, bool>>::iterator element = _flushList.begin();
			list<pair<T, bool>>::iterator end = _flushList.end();

			while (element != end)
			{
				list<T>::iterator found = find(_data.begin(), _data.end(), (*element).first);

				if ((*element).second)
				{
					if (found == _data.end())
					{
						_data.push_back((*element).first);
					}
				}
				else
				{
					if (found != _data.end())
					{
						_data.erase(found);
					}
				}

				element++;
			}

			_flushList.clear();
		}

	private:

		/*! queue to flush
		*/
		list<pair<T, bool>> _flushList;

		/*! concrete _data
		*/
		list<T> _data;

	};
};

#endif