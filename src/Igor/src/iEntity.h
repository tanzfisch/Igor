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

#include <mutex>
using namespace std;

namespace Igor
{

    /*! entity
    */
    class Igor_API iEntity
    {
        friend class iEntityManager;

    public:

        /*! definition of invalid entity id
        */ 
        static const uint64 INVALID_ENTITY_ID = 0;

        /*! \returns entity id
        */
        uint64 getID() const;

    private:

        /*! id of entity
        */
        uint64 _id = INVALID_ENTITY_ID;

        /*! next id for next generated entity
        */
        static uint64 _nextEntityID;

        /*! mutex to protect entity id generation
        */
        static mutex _mutexID;

        /*! generated entity id
        */
        iEntity();

        /*! does nothing
        */
        virtual ~iEntity() = default;
    };

}

#endif