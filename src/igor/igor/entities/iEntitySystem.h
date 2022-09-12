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

#include <igor/entities/iEntityComponent.h>

namespace igor
{

	enum class iComponentFlag : uint32
	{
		Optional = 1,
	};

	/*! entity system base class
	 */
	class iEntitySystem
	{
	public:
		/*! does nothing
		 */
		iEntitySystem() = default;

		/*! updates components
		\param components the components to update
		*/
		virtual void updateComponents(BaseComponent **components) {}

		/*! \returns component types
		 */
		const std::vector<uint32> &getComponentTypes() const;

		/*! \returns component flags
		 */
		const std::vector<uint32> &getComponentFlags() const;

		/*! \returns true if valid
		 */
		bool isValid();

	protected:
		/*! add component type to system
		\param componentType the component type to add
		\param componentFlag the component flag to add
		*/
		void addComponentType(uint32 componentType, uint32 componentFlag = 0);

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
		inline bool addSystem(iEntitySystem &system)
		{
			if (!system.isValid())
			{
				con_err("invalid system");
				return false;
			}
			_systems.push_back(&system);
			return true;
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