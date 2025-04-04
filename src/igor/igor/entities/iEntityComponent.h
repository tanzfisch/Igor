//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin Loga
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

#include <functional>
#include <bitset>

namespace igor
{

    /*! entity component state
     */
    enum class iEntityComponentState
    {
        Unloaded,         //! initial state unloaded
        UnloadedInactive, //! unloaded but inactive
        LoadFailed,       //! load failed, stays inactive
        Inactive,         //! inactive
        Active,           //! active
    };

    /*
    +---------------+                                                           +----------------+  
    |               v                                                           v                |
    |   ------------------------------------------    ----------------------------------------   |
    |   |                Unloaded                |    |           UnloadedInactive           |   |
    |   ------------------------------------------    ----------------------------------------   |
    |               |                         |          |                      |                |
    |               |                         v          v                      |                |
    |               |                 --------------------------                |                |
    |               |                 |       LoadFailed       |                |                |
    |               |                 --------------------------                |                |
    |               |                                                           |                |
    |               v                                                           v                |
    |   --------------------------                                  --------------------------   |
    +-- |         Active         | -------------------------------> |        Inactive        | --+
        |                        | <------------------------------- |                        |
        --------------------------                                  --------------------------

    */

    /*! pointer definition of entity component
     */
    class iEntityComponent;
    typedef iEntityComponent *iEntityComponentPtr;

    /*! entity component mask definition
     */
    typedef std::bitset<IGOR_MAX_ENTITY_COMPONENT_TYPES> iEntityComponentMask;

    /*! entity component factory function component
     */
    using iEntityComponentFactory = std::function<iEntityComponentPtr()>;

    /*! entity component type info
     */
    struct iEntityComponentTypeInfo
    {
        /*! factory function to create components
         */
        iEntityComponentFactory _factory;

        /*! bit mask to identify component type
         */
        iEntityComponentMask _mask;

        /*! human readable type name
         */
        iaString _typeName;

        iEntityComponentTypeInfo()
        {
        }

        iEntityComponentTypeInfo(iEntityComponentFactory factory, const iEntityComponentMask &mask, const iaString &typeName)
            : _factory(factory), _typeName(typeName), _mask(mask)
        {
        }
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
        friend class iEntityScene;
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

        /*! \returns state of this component
         */
        iEntityComponentState getState() const;

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

        /*! \returns a copy of this component
         */
        virtual iEntityComponentPtr getCopy() = 0;

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
}

#endif //  IGOR_ENTITY_COMPONENT_H