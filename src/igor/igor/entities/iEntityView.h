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
// (c) Copyright 2012-2023 by Martin Loga
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

#include <iaux/system/iaTime.h>

#include <typeindex>
#include <vector>

namespace igor
{

	/*! entity pointer definition
	 */
	class iEntity;
	typedef iEntity *iEntityPtr;

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
		}

		/*! checks if given type index of components is suported

		\param entity pointer of entity to check compatibility with
		*/
		bool checkCompatibility(iEntityPtr entity) const;

		/*! \returns list of entities registered to this system
		 */
		const std::vector<iEntityPtr> &getEntities() const;

	private:
		/*! supported types
		 */
		std::vector<std::type_index> _supportedTypes;

		/*! entities registered with this system
		 */
		std::vector<iEntityPtr> _entities;

		/*!
		 */
		void onComponentsChanged(iEntityPtr entity);
	};

	/*! entity set pointer definition
	 */
	typedef iEntityView *iEntityViewPtr;

} // igor

#endif // IGOR_ENTITY_SET_H