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

#ifndef __iENTITYSYSTEM__
#define __iENTITYSYSTEM__

#include <igor/entities/iComponent.h>

namespace igor
{

	enum class iComponentFlag : uint32
	{
		None = 0,
		Optional = 1,
	};

	/*! entity system base class
	 */
	class iEntitySystem
	{
	public:
		/*! init system
		 */
		iEntitySystem(const iaString &name);

		/*! updates components
		\param components the components to update
		*/
		virtual void updateComponents(iBaseComponent **components) {}

		/*! \returns component types
		 */
		const std::vector<uint32> &getComponentTypes() const;

		/*! \returns component flags
		 */
		const std::vector<uint32> &getComponentFlags() const;

		/*! \returns name of this system
		 */
		const iaString &getName() const;

	protected:
		/*! name of this system
		 */
		iaString _name;

		/*! add component type to system
		\param componentType the component type to add
		\param componentFlag the component flag to add
		*/
		void addComponentType(uint32 componentType, uint32 componentFlags = static_cast<uint32>(iComponentFlag::None));

	private:
		/*! component types
		 */
		std::vector<uint32> _componentTypes;

		/*! component flags
		 */
		std::vector<uint32> _componentFlags;
	};

	/*! list of entity systems
	 */
	class iEntitySystemList
	{
	public:
		inline void addSystem(iEntitySystem &system)
		{
			_systems.push_back(&system);
		}

		inline size_t size()
		{
			return _systems.size();
		}

		inline iEntitySystem *operator[](uint32 index)
		{
			return _systems[index];
		}

		bool removeSystem(iEntitySystem &system);

	private:
		std::vector<iEntitySystem *> _systems;
	};

} // igor

#endif // __iENTITYSYSTEM__