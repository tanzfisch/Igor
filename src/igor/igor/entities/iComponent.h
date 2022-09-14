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

#ifndef __iENTITYCOMPONENT__
#define __iENTITYCOMPONENT__

#include <igor/iDefines.h>

#include <vector>

namespace igor
{

typedef std::vector<std::pair<uint32, uint32>> iComponentData;
typedef std::pair<uint32, iComponentData> iEntity;
typedef iEntity* iEntityPtr;

struct iBaseComponent;
typedef uint32 (*iComponentCreateFunction)(std::vector<uint8>& memory, iEntityPtr entity, iBaseComponent* comp);
typedef void (*iComponentFreeFunction)(iBaseComponent* comp);
typedef size_t iComponentID;

struct iComponentMetrics
{
	/*! component create function
	*/
	iComponentCreateFunction _createFunction;

	/*! component free function
	*/
	iComponentFreeFunction _freeFunction;

	/*! component size in bytes
	*/
	size_t _size;
};

struct iBaseComponent
{
public:
	static iComponentID registerComponentType(iComponentCreateFunction createfn, iComponentFreeFunction freefn, size_t size);
	iEntityPtr _entity = nullptr;

	inline static iComponentCreateFunction getTypeCreateFunction(uint32 id)
	{
		return (*_componentTypes)[id]._createFunction;
	}

	inline static iComponentFreeFunction getTypeFreeFunction(uint32 id)
	{
		return (*_componentTypes)[id]._freeFunction;
	}

	inline static size_t getTypeSize(uint32 id)
	{
		return (*_componentTypes)[id]._size;
	}

	inline static bool isTypeValid(uint32 id)
	{
		return id < _componentTypes->size();
	}
private:

	static std::vector<iComponentMetrics>* _componentTypes;
};

template<typename T>
struct iComponent : public iBaseComponent
{
	static const iComponentCreateFunction CREATE_FUNCTION;
	static const iComponentFreeFunction FREE_FUNCTION;
	static const iComponentID ID;
	static const size_t SIZE; 
};

template<typename Component>
uint32 ComponentCreate(std::vector<uint8>& memory, iEntityPtr entity, iBaseComponent* comp)
{
	iComponentID index = memory.size();
	memory.resize(index+Component::SIZE);
	Component* component = new(&memory[index])Component(*(Component*)comp);
	component->_entity = entity;
	return index;
}

template<typename Component>
void ComponentFree(iBaseComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const iComponentID iComponent<T>::ID(iBaseComponent::registerComponentType(ComponentCreate<T>, ComponentFree<T>, sizeof(T)));

template<typename T>
const size_t iComponent<T>::SIZE(sizeof(T));

template<typename T>
const iComponentCreateFunction iComponent<T>::CREATE_FUNCTION(ComponentCreate<T>);

template<typename T>
const iComponentFreeFunction iComponent<T>::FREE_FUNCTION(ComponentFree<T>);

} // namespace igor

#endif // __iENTITYCOMPONENT__