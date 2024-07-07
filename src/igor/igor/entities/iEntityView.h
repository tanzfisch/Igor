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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_ENTITY_VIEW_H
#define IGOR_ENTITY_VIEW_H

#include <igor/iDefines.h>

#include <igor/entities/iEntity.h>

namespace igor
{

	/*! entity pointer definition
	 */
	class iEntity;
	typedef iEntity *iEntityPtr;

	/*! entity set pointer definition
	 */
	class iEntityView;
	typedef iEntityView *iEntityViewPtr;	

	/*! set of entities
	 */
	class IGOR_API iEntityView
	{
		friend class iEntitySystem;

	public:

		/*! does nothing
		 */
		iEntityView() = default;

		/*! does nothing
		 */
		virtual ~iEntityView() = default;			

		/*! register type of component
		 */
		template <typename T>
		void registerType()
		{
			_supportedTypes.push_back(typeid(T));
			_componentMask = calcComponentMask(_supportedTypes);
		}		

		/*! checks if given type index of components is suported

		\param entity pointer of entity to check compatibility with
		*/
		bool checkCompatibility(iEntityPtr entity) const;

		/*! \returns list of entities registered to this system
		 */
		std::vector<iEntityPtr> &getEntities();

		/*! \returns list of entities registered to this system but are currently inactive
		 */
		std::vector<iEntityPtr> &getInactiveEntities();

	private:
		/*! supported types
		 */
		std::vector<std::type_index> _supportedTypes;

		/*! key of all supported types
		*/
		iEntityComponentMask _componentMask = 0;

		/*! entities registered with this system
		 */
		std::vector<iEntityPtr> _entities;

		/*! entities registered with this system but are currently inactive
		 */
		std::vector<iEntityPtr> _inactiveEntities;

		/*! \returns type hash for given types
		*/
		iEntityComponentMask calcComponentMask(const std::vector<std::type_index> &types);

		/*! called when something relevant to the given entity changed

		\param entity the given entity
		 */
		void onEntityChanged(iEntityPtr entity);

		/*! removes given entity from view

		\param entity the given entity
		*/
		void removeEntity(iEntityPtr entity);

	};

} // igor

#endif // IGOR_ENTITY_SET_H