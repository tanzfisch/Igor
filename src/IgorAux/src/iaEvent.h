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
//
// Originally this code was written by Markus Schulz for nexusPIL

#ifndef __IGOR_AUX_EVENT__
#define __IGOR_AUX_EVENT__

#include <iaDefines.h>
#include <iaDelegate.h>
#include <iaConsole.h>

#include <vector>
#include <mutex>
using namespace std;

namespace IgorAux
{

    /*! event
    */
#define iaEVENT(EventName,DelegateName,ReturnValue,ParameterList,InnerParameterList)				    \
	iaDELEGATE(DelegateName, ReturnValue, ParameterList, InnerParameterList);							\
																										\
	class EventName                 																	\
	{																									\
																										\
		public:																							\
																										\
			void append(const DelegateName &fpDelegate)													\
			{																							\
                _mutex.lock();                                                                          \
				_delegates.push_back(fpDelegate);													    \
                _mutex.unlock();                                                                        \
			}																							\
																										\
			void remove(const DelegateName &fpDelegate)													\
			{																							\
                _mutex.lock();                                                                          \
				vector<DelegateName>::iterator it;														\
				for(it = _delegates.begin();it != _delegates.end();it++)						        \
				{																						\
					if((*it) == fpDelegate)																\
					{																					\
						_delegates.erase(it);														    \
						break;																			\
					}																					\
				}																						\
                _mutex.unlock();                                                                        \
			}																							\
																										\
			__inline void operator() ParameterList												        \
			{																							\
                _mutex.lock();                                                                          \
                vector<DelegateName> delegates = _delegates;                                            \
                _mutex.unlock();                                                                        \
				for (unsigned int i = 0; i<delegates.size(); ++i)									    \
				{																						\
                    delegates[i] InnerParameterList;												    \
				}																						\
			}																							\
																										\
			void clear()																				\
			{																							\
                _mutex.lock();                                                                          \
				_delegates.clear();																        \
                _mutex.unlock();                                                                        \
			}																							\
																										\
			bool hasDelegates()																			\
			{																							\
				return _delegates.size() ? true : false;											    \
			}																							\
																										\
		protected:																						\
																										\
            mutex _mutex;                                                                               \
			vector<DelegateName> _delegates;														    \
    };

};

#endif
