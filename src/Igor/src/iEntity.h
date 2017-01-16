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

#ifndef __iENTITY__
#define __iENTITY__

#include <iDefines.h>

#include <iaVector3.h>
#include <iSphere.h>
using namespace IgorAux;

#include <mutex>
using namespace std;

namespace Igor
{

    /*! base class for entities
    */
    class Igor_API iEntity
    {

        friend class iEntityManager;

        enum class iEntityState : uint8
        {
            Created,
            Running,
            Delete
        };

    public:

        /*! invalid entity id definition
        */
        static const uint64 INVALID_ENTITY_ID = 0;

        /*! \returns entity id
        */
        uint64 getID() const;

        /*! sets position of entity

        \param position new position to set
        */
        virtual void setPosition(const iaVector3d& position);

        /*! \returns current position
        */
        iaVector3d getPosition() const;

        /*! \returns sphere of entity
        */
        iSphered getSphere() const;

        /*! marks an entity to be deleted
        */
        void kill();

    protected:

        /*! current position
        */
        iSphered _sphere;

        /*! initialize entity
        */
        virtual void init() = 0;

        /*! deinitialize entity
        */
        virtual void deinit() = 0;

        /*! handle entity
        */
        virtual void handle() = 0;

        /*! initializes members
        */
        iEntity();

        /*! does nothing
        */
        virtual ~iEntity() = default;

    private:

        iEntityState _state = iEntityState::Created;

        /*! entity ID
        */
        uint64 _id = INVALID_ENTITY_ID;

        /*! next entity id
        */
        static uint64 _nextID;

        /*! mutex to save unique id generation
        */
        static mutex _mutexID;

    };

}

#endif