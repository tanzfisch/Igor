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
// (c) Copyright 2014-2016 by Martin Loga
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
// contact: martinloga@gmx.de  

#ifndef __iCOMPONENT_BASE__
#define __iCOMPONENT_BASE__

#include <iDefines.h>

#include <mutex>
using namespace std;

namespace Igor
{

    class Igor_API iComponentBase
    {

        friend class iEntityManager;

    public:

        /*! definition of invalid entity id
        */
        static const uint64 INVALID_COMPONENT_ID = 0;

        /*! \returns entity id
        */
        uint64 getID() const;

        /*! \returns component data for specified entity

        \param entityID the specified entity's id
        */
        virtual void* getData(uint64 entityID) = 0;

        /*! \returns component data for all entitties
        */
        virtual void* getData() = 0;

        /*! links an entity to this component

        \param entityID the entity to link with
        */
        virtual void linkEntity(uint64 entityID) = 0;

        /*! unlinks an entity from this component

        \param entityID the entity to unlink
        */
        virtual void unlinkEntity(uint64 entityID) = 0;

        iComponentBase();
        virtual ~iComponentBase() = default;

    private:

        /*! id of entity
        */
        uint64 _id = INVALID_COMPONENT_ID;

        /*! next id for next generated entity
        */
        static uint64 _nextComponentID;

        /*! mutex to protect entity id generation
        */
        static mutex _mutexID;

    };
}

#endif