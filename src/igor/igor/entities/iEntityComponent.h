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

#ifndef IGOR_ENTITY_COMPONENT_H
#define IGOR_ENTITY_COMPONENT_H

#include <igor/iDefines.h>

#include <igor/utils/iJson.h>
#include <iaux/data/iaUUID.h>
using namespace iaux;

namespace igor
{

    enum class iEntityComponentState
    {
        Unloaded,   //! initial state unloaded
        Loaded,     //! loaded but inactive
        LoadFailed, //! load failed, stays inactive
        Active,     //! active
        Inactive,   //! inactive
    };

    /*! entity component id
     */
    typedef iaUUID iEntityComponentID;

    /*! entity id
     */
    typedef iaUUID iEntityID;

    /*! entity pointer definition
     */
    class iEntity;
    typedef iEntity *iEntityPtr;    

    /*! entity component base class
     */
    class IGOR_API iEntityComponent
    {
        friend class iEntity;
        friend class iTransformComponent;

    public:
        /*! ctor
        */
        iEntityComponent();

        /*! does nothing
         */
        virtual ~iEntityComponent() = default;

        /*! \returns entity component id
         */
        const iEntityComponentID &getID() const;

    protected:
        /*! callback for loading component

        \param entity the entity this component relates to
        \param[out] asyncLoad if true try again if unsuccessful
        \returns true when loading was successful
        */
        virtual bool onLoad(iEntityPtr entity, bool &asyncLoad);

        /*! callback to activate component
         */
        virtual void onActivate(iEntityPtr entity);

        /*! callback to deactivate component
         */
        virtual void onDeactivate(iEntityPtr entity);

        /*! callback for unload component
         */
        virtual void onUnLoad(iEntityPtr entity);

    private:
        /*! entity component id
         */
        iEntityComponentID _id;

        /*! component state
         */
        iEntityComponentState _state = iEntityComponentState::Unloaded;

        /*! reference to entity
        */
        iEntityPtr _entity = nullptr;
    };

    /*! pointer definition of entity component
     */
    typedef iEntityComponent *iEntityComponentPtr;

    /*! iEntityComponent to json
     */
    void to_json(json &j, const iEntityComponent &component);

    /*! json to iEntityComponent
     */
    void from_json(const json &j, iEntityComponent &component);    

}

#endif //  IGOR_ENTITY_COMPONENT_H