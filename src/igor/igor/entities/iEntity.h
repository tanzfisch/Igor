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
// (c) Copyright 2012-2020 by Martin Loga
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

#ifndef __iENTITY__
#define __iENTITY__

#include <igor/iDefines.h>

#include <iaux/system/iaMutex.h>
using namespace iaux;

namespace igor
{

    /*! engine internal entity base types
    */
    enum class iEntityType
    {
        Undefined,
        Base,
        Locatable
    };

    /*! entity base class
    */
    class IGOR_API iEntity
    {

        /*! so we can call the handle
        */
        friend class iEntityManager;

    public:
        /*! invalid entity id definition
        */
        static const uint64 INVALID_ENTITY_ID;

        /*! \returns entity id
        */
        uint64 getID() const;

        /*! \returns entity type
        */
        iEntityType getType() const;

        /*! init id and register entity
        */
        iEntity();

        /*! unregister
        */
        virtual ~iEntity();

    protected:
        /*! called every simulation frame
        */
        virtual void handle() = 0;

        /*! entity type
        */
        iEntityType _type = iEntityType::Undefined;

    private:
        /*! entity id
        */
        uint64 _id = 0;

        /*! next entity id
        */
        static uint64 _nextID;

        /*! mutex to protec id generation
        */
        static iaMutex _mutexID;
    };

} // namespace igor

#endif