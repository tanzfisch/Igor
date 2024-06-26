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

#ifndef IGOR_ENTITY_SYSTEM_H
#define IGOR_ENTITY_SYSTEM_H

#include <igor/entities/iEntityView.h>

#include <iaux/system/iaTime.h>

namespace igor
{

	/*! entity pointer definition
	 */
	class iEntity;
	typedef iEntity *iEntityPtr;

	/*! entity scene pointer definition
	 */
	class iEntityScene;
	typedef iEntityScene *iEntityScenePtr;

	/*! entity system processing stage
	*/
	enum class iEntitySystemStage
	{
		Update,
		PreRender,
		Render,
		StageCount
	};

	/*! entity system base class
	 */
	class IGOR_API iEntitySystem
	{
		friend class iEntity;
		friend class iEntityScene;

	public:
		/*! does nothing
		 */
		iEntitySystem() = default;

		/*! does nothing
		 */
		virtual ~iEntitySystem() = default;

		/*! updates system

		\param time the time of the update tick
		\param scene the scene used for this update
		 */
		virtual void update(const iaTime &time, iEntityScenePtr scene) = 0;

		/*! \returns processing stage this system want's to run in
		*/
		virtual iEntitySystemStage getStage() const = 0;

		/*! \returns scene this system operates in
		*/
		iEntityScenePtr getScene() const;

	protected:

		/*! callback to handle added component on entity

		\param entity pointer of entity
		\param typeID type of component that has been added
		*/
		virtual void onComponentAdded(iEntityPtr entity, const std::type_index &typeID);

		/*! callback to handle removed component on entity

		\param entity pointer of entity
		\param typeID type of component that has been removed
		*/
		virtual void onComponentRemoved(iEntityPtr entity, const std::type_index &typeID);

		/*! callback to handle component to be removed

		\param entity pointer of entity
		\param typeID type of component to be removed
		*/
		virtual void onComponentToRemove(iEntityPtr entity, const std::type_index &typeID);

		/*! update when entity changed in a way that is relevant to views

		\param entity the entity to update with this system
		*/
		void onEntityChanged(iEntityPtr entity);

		/*! create an entity view for given component types

		\returns entity view
		*/
		template <typename... Args>
		iEntityViewPtr createView()
		{
			iEntityViewPtr view = new iEntityView();
			(view->registerType<Args>(), ...);
			_views.push_back(view);
			return view;
		}		

	private:
		/*! entity views
		 */
		std::vector<iEntityViewPtr> _views;

		/*! scene this system operates in
		*/
		iEntityScenePtr _scene;
	};

	/*! entity system pointer definition
	 */
	typedef iEntitySystem *iEntitySystemPtr;

} // igor

#endif // IGOR_ENTITY_SYSTEM_H