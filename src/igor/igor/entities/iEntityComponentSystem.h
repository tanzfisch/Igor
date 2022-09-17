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

#ifndef __iECS__
#define __iECS__

#include <igor/entities/iComponent.h>
#include <igor/entities/iEntitySystem.h>

namespace igor
{
	/*! entity component system

	based on https://github.com/BennyQBD/3DGameProgrammingTutorial/
	Copyright (c) 2018 Benny Bobaganoosh <3
	*/
	class iEntityComponentSystem
	{

	public:
		/*! does nothing
		 */
		iEntityComponentSystem() = default;
		~iEntityComponentSystem();

		// Entity methods
		iEntityPtr createEntity(iBaseComponent **components, const iComponentID *componentIDs, size_t numComponents);
		void destroyEntity(iEntityPtr entity);

		template <class A>
		iEntityPtr createEntity(A &c1)
		{
			iBaseComponent *components[] = {&c1};
			iComponentID componentIDs[] = {A::ID};
			return createEntity(components, componentIDs, 1);
		}

		template <class A, class B>
		iEntityPtr createEntity(A &c1, B &c2)
		{
			iBaseComponent *components[] = {&c1, &c2};
			iComponentID componentIDs[] = {A::ID, B::ID};
			return createEntity(components, componentIDs, 2);
		}

		template <class A, class B, class C>
		iEntityPtr createEntity(A &c1, B &c2, C &c3)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID};
			return createEntity(components, componentIDs, 3);
		}

		template <class A, class B, class C, class D>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID};
			return createEntity(components, componentIDs, 4);
		}

		template <class A, class B, class C, class D, class E>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID};
			return createEntity(components, componentIDs, 5);
		}

		template <class A, class B, class C, class D, class E, class F>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5, F &c6)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5, &c6};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID};
			return createEntity(components, componentIDs, 6);
		}

		template <class A, class B, class C, class D, class E, class F, class G>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5, F &c6, G &c7)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID};
			return createEntity(components, componentIDs, 7);
		}

		template <class A, class B, class C, class D, class E, class F, class G, class H>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5, F &c6, G &c7, H &c8)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID};
			return createEntity(components, componentIDs, 8);
		}

		template <class A, class B, class C, class D, class E, class F, class G, class H, class I>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5, F &c6, G &c7, H &c8, I &c9)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID};
			return createEntity(components, componentIDs, 9);
		}

		template <class A, class B, class C, class D, class E, class F, class G, class H, class I, class J>
		iEntityPtr createEntity(A &c1, B &c2, C &c3, D &c4, E &c5, F &c6, G &c7, H &c8, I &c9, J &c10)
		{
			iBaseComponent *components[] = {&c1, &c2, &c3, &c4, &c5, &c6, &c7, &c8, &c9, &c10};
			iComponentID componentIDs[] = {A::ID, B::ID, C::ID, D::ID, E::ID, F::ID, G::ID, H::ID, I::ID, J::ID};
			return createEntity(components, componentIDs, 10);
		}

		// Component methods
		template <class Component>
		inline void addComponent(iEntityPtr entity, Component *component)
		{
			addComponentInternal(entity, component, Component::ID);
		}

		template <class Component>
		bool removeComponent(iEntityPtr entity)
		{
			return removeComponentInternal(entity, Component::ID);
		}

		template <class Component>
		Component *getComponent(iEntityPtr entity)
		{
			return (Component *)getComponentInternal(getComponentData(entity), _components[Component::ID], Component::ID);
		}

		// System methods
		void updateSystems(iEntitySystemList &systems);

	private:
		std::vector<iEntitySystem *> _systems;

		/*! map of component IDs to the raw component data
		*/
		std::map<uint32, std::vector<uint8>> _components;

		/*! list of all entities
		*/
		std::vector<iEntityPtr> _entities;

		/*! \returns index for given entity
		*/
		__IGOR_INLINE__ uint32 getEntityIndex(iEntityPtr entity)
		{
			return entity->first;
		}

		/*! \returns component data for given entity
		*/
		__IGOR_INLINE__ iComponentData &getComponentData(iEntityPtr entity)
		{
			return entity->second;
		}

		void deleteComponent(uint32 componentID, uint32 index);
		bool removeComponentInternal(iEntityPtr entity, uint32 componentID);
		void addComponentInternal(iEntityPtr entity, iBaseComponent *component, uint32 componentID);
		iBaseComponent *getComponentInternal(iComponentData &entityComponents, std::vector<uint8> &componentRawData, uint32 componentID);

		void updateSystemWithMultipleComponents(uint32 index, iEntitySystemList &systems, const std::vector<uint32> &componentTypes,
												std::vector<iBaseComponent *> &componentParam, std::vector<std::vector<uint8> *> &componentArrays);
		uint32 findLeastCommonComponent(const std::vector<uint32> &componentTypes, const std::vector<uint32> &componentFlags);
	};

} // namespace igor

#endif // __iECS__