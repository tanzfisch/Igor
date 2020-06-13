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
// contact: martinloga@gmx.de
//
// Originally this code was written by Markus Schulz for nexusPIL

#ifndef __IAUX_EVENT__
#define __IAUX_EVENT__

#include <iaux/system/iaMutex.h>
#include <iaux/system/iaDelegate.h>

#include <vector>
#include <algorithm>

namespace iaux
{

	/*! event
    */
#define iaEVENT(EventName, DelegateName, ReturnType, ParameterList, InnerParameterList) \
	iaDELEGATE(DelegateName, ReturnType, ParameterList, InnerParameterList);            \
                                                                                        \
	class EventName                                                                     \
	{                                                                                   \
                                                                                        \
	public:                                                                             \
		void append(const DelegateName &fpDelegate)                                     \
		{                                                                               \
			_mutex.lock();                                                              \
			std::vector<DelegateName>::iterator it;                                     \
			it = std::find(_delegates.begin(), _delegates.end(), fpDelegate);           \
			if (it == _delegates.end())                                                 \
			{                                                                           \
				_delegates.push_back(fpDelegate);                                       \
			}                                                                           \
			_mutex.unlock();                                                            \
		}                                                                               \
                                                                                        \
		void remove(const DelegateName &fpDelegate)                                     \
		{                                                                               \
			_mutex.lock();                                                              \
			std::vector<DelegateName>::iterator it;                                     \
			it = std::find(_delegates.begin(), _delegates.end(), fpDelegate);           \
			if (it != _delegates.end())                                                 \
			{                                                                           \
				_delegates.erase(it);                                                   \
			}                                                                           \
			_mutex.unlock();                                                            \
		}                                                                               \
                                                                                        \
		void block(bool blockEvent = true)                                              \
		{                                                                               \
			_blocked = blockEvent;                                                      \
		}                                                                               \
                                                                                        \
		void unblock()                                                                  \
		{                                                                               \
			_blocked = false;                                                           \
		}                                                                               \
                                                                                        \
		bool isBlocked()                                                                \
		{                                                                               \
			return _blocked;                                                            \
		}                                                                               \
                                                                                        \
		__inline ReturnType operator() ParameterList                                    \
		{                                                                               \
			if (_blocked)                                                               \
			{                                                                           \
				return ReturnType();                                                    \
			}                                                                           \
                                                                                        \
			_mutex.lock();                                                              \
			std::vector<DelegateName> delegates = _delegates;                           \
			_mutex.unlock();                                                            \
			for (auto dgate : delegates)                                                \
			{                                                                           \
				dgate InnerParameterList;                                               \
			}                                                                           \
		}                                                                               \
                                                                                        \
		void clear()                                                                    \
		{                                                                               \
			_mutex.lock();                                                              \
			_delegates.clear();                                                         \
			_mutex.unlock();                                                            \
		}                                                                               \
                                                                                        \
		bool hasDelegates()                                                             \
		{                                                                               \
			return _delegates.size() ? true : false;                                    \
		}                                                                               \
                                                                                        \
	protected:                                                                          \
		iaMutex _mutex;                                                                 \
		std::vector<DelegateName> _delegates;                                           \
		bool _blocked = false;                                                          \
	};

}; // namespace iaux

#endif
