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

struct BaseComponent;
typedef void* iEntityHandle;
typedef uint32 (*ComponentCreateFunction)(std::vector<uint8>& memory, iEntityHandle entity, BaseComponent* comp);
typedef void (*ComponentFreeFunction)(BaseComponent* comp);
typedef size_t iEntityComponentID;

struct ComponentMetrics
{
	/*! component create function
	*/
	ComponentCreateFunction _createFunction;

	/*! component free function
	*/
	ComponentFreeFunction _freeFunction;

	/*! component size in bytes
	*/
	size_t _size;
};

struct BaseComponent
{
public:
	static iEntityComponentID registerComponentType(ComponentCreateFunction createfn, ComponentFreeFunction freefn, size_t size);
	iEntityHandle _entity = nullptr;

	inline static ComponentCreateFunction getTypeCreateFunction(uint32 id)
	{
		return (*_componentTypes)[id]._createFunction;
	}

	inline static ComponentFreeFunction getTypeFreeFunction(uint32 id)
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

	static std::vector<ComponentMetrics>* _componentTypes;
};

template<typename T>
struct iEntityComponent : public BaseComponent
{
	static const ComponentCreateFunction CREATE_FUNCTION;
	static const ComponentFreeFunction FREE_FUNCTION;
	static const iEntityComponentID ID;
	static const size_t SIZE; 
};

template<typename Component>
uint32 ComponentCreate(std::vector<uint8>& memory, iEntityHandle entity, BaseComponent* comp)
{
	iEntityComponentID index = memory.size();
	memory.resize(index+Component::SIZE);
	Component* component = new(&memory[index])Component(*(Component*)comp);
	component->_entity = entity;
	return index;
}

template<typename Component>
void ComponentFree(BaseComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const iEntityComponentID iEntityComponent<T>::ID(BaseComponent::registerComponentType(ComponentCreate<T>, ComponentFree<T>, sizeof(T)));

template<typename T>
const size_t iEntityComponent<T>::SIZE(sizeof(T));

template<typename T>
const ComponentCreateFunction iEntityComponent<T>::CREATE_FUNCTION(ComponentCreate<T>);

template<typename T>
const ComponentFreeFunction iEntityComponent<T>::FREE_FUNCTION(ComponentFree<T>);

} // namespace igor

#endif // __iENTITYCOMPONENT__