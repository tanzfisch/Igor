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

#include <iaux/data/iaUUID.h>
using namespace iaux;

namespace igor
{

    /*! entity component id
     */
    typedef iaUUID iEntityComponentID;

    /*! entity id
     */
    typedef iaUUID iEntityID;

    /*! component states

        ------------> Unloaded (initial)
        |                   |
        |onUnload           | onLoad
        |                   |
        |                 fail? -> LoadFailed
        |                   |
        |----------------
                         Loaded
        |--------------->
        |                   |
        | onDeactivate      | onActivate
        |                   |
        |                Active
        |----------------
    */
    enum class iEntityComponentState
    {
        Unloaded,   //! initial state unloaded
        Loaded,     //! loaded but inactive
        LoadFailed, //! load failed, stays inactive
        Active,     //! active
    };

    /*! entity component base class
     */
    class IGOR_API iEntityComponent
    {

        friend class iEntity;

    public:
        /*! ctor

        \param name the name of this component
        */
        iEntityComponent(const iaString &name = "");

        /*! does nothing
         */
        virtual ~iEntityComponent() = default;

        /*! \returns entity component id
         */
        const iEntityComponentID &getID() const;

        /*! sets name of component
         */
        void setName(const iaString &name);

        /*! \returns component name
         */
        const iaString &getName() const;

        /*! \returns true if in un loaded state
         */
        bool isUnloaded() const;

        /*! \returns true if in loaded state
         */
        bool isLoaded() const;

        /*! \returns true if in load failed
         */
        bool isLoadFailed() const;

        /*! \returns true if in active state
         */
        bool isActive() const;

    protected:
        /*! called when loading component
        \returns true if success
        */
        virtual bool onLoad();

        /*! called when unloading component
         */
        virtual void onUnload();

        /*! called when activating component
         */
        virtual void onActivate();

        /*! called when deactivating component
         */
        virtual void onDeactivate();

    private:
        /*! entity component id
         */
        iEntityComponentID _id;

        /*! component name
         */
        iaString _name;

        /*! id of entity owning this component
         */
        iEntityID _entityID;

        /*! entity component state
         */
        iEntityComponentState _state = iEntityComponentState::Unloaded;
    };

    /*! pointer definition of entity component
     */
    typedef iEntityComponent *iEntityComponentPtr;

}

#endif //  IGOR_ENTITY_COMPONENT_H