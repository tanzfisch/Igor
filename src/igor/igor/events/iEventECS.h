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

#ifndef __IGOR_EVENTECS__
#define __IGOR_EVENTECS__

#include <igor/events/iEvent.h>

namespace igor
{
#if 0
    /*! entity added to entity component system event
    */
    class IGOR_API iEventEntityCreated : public iEvent
    {
    public:
        /*! init members

        \param entity the entity that was added
        \param ecs the related entity component system
        */
        iEventEntityCreated(const iEntityPtr entity, const iEntityComponentSystemPtr ecs);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the entity
        */
        iEntityPtr getEntity() const;

        /*! \returns the entity component system
        */
        iEntityComponentSystemPtr getECS() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::ECS)
        IGOR_EVENT_CLASS_TYPE(iEventEntityCreated)

    private:

        /*! entity handle
        */
        iEntityPtr _entity;
        
        /*! entity component system
        */
        iEntityComponentSystemPtr _ecs;
    };

    /*! entity about to be destroyed event
    */
    class IGOR_API iEventEntityDestroyed : public iEvent
    {
    public:
        /*! init members

        \param entity the entity that will be destroyed
        \param ecs the related entity component system
        */
        iEventEntityDestroyed(const iEntityPtr entity, const iEntityComponentSystemPtr ecs);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the entity
        */
        iEntityPtr getEntity() const;

        /*! \returns the entity component system
        */
        iEntityComponentSystemPtr getECS() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::ECS)
        IGOR_EVENT_CLASS_TYPE(iEventEntityDestroyed)

    private:

        /*! entity handle
        */
        iEntityPtr _entity;
        
        /*! entity component system
        */
        iEntityComponentSystemPtr _ecs;
    };
#endif
}; // namespace igor

#endif // __IGOR_EVENTECS__