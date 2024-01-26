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

#ifndef IGOR_EVENTECS_H
#define IGOR_EVENTECS_H

#include <igor/events/iEvent.h>
#include <igor/entities/iEntity.h>

namespace igor
{
    /*! entity added to entity component system event
    */
    class IGOR_API iEventEntityCreated : public iEvent
    {
    public:
        /*! init members

        \param entity the entity that was added
        */
        iEventEntityCreated(const iEntityPtr entity);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the entity
        */
        iEntityPtr getEntity() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Entity)
        IGOR_EVENT_CLASS_TYPE(iEventEntityCreated)

    private:

        /*! entity handle
        */
        iEntityPtr _entity;
    };

    /*! entity about to be destroyed event
    */
    class IGOR_API iEventEntityDestroyed : public iEvent
    {
    public:
        /*! init members

        \param entity the entity that will be destroyed
        */
        iEventEntityDestroyed(const iEntityPtr entity);

        /*! \returns information to event
        */
        virtual const iaString getInfo() const override;

        /*! \returns the entity
        */
        iEntityPtr getEntity() const;

        IGOR_EVENT_KIND_MASK((iEventKindMask)iEventKind::Entity)
        IGOR_EVENT_CLASS_TYPE(iEventEntityDestroyed)

    private:

        /*! entity handle
        */
        iEntityPtr _entity;
    };

}; // namespace igor

#endif // IGOR_EVENTECS_H