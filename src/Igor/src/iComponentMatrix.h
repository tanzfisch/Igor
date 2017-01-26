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

#ifndef __iCOMPONENT_MATRIX__
#define __iCOMPONENT_MATRIX__

#include <iComponent.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <map>
using namespace std;

namespace Igor
{

    class Igor_API iComponentMatrix : public iComponent
    {

    public:

        /*! \returns component data for specified entity

        \param entityID the specified entity's id
        */
        void* getData(uint64 entityID);

        /*! \returns component data for all entitties
        */
        void* getData();

        /*! links an entity to this component

        \param entityID the entity to link with
        */
        void linkEntity(uint64 entityID);

        /*! unlinks an entity from this component

        \param entityID the entity to unlink
        */
        void unlinkEntity(uint64 entityID);

        /*! does nothing
        */
        iComponentMatrix() = default;

        /*! does nothing
        */
        ~iComponentMatrix() = default;

    private:

        /*! the actual data
        */
        map<uint64, iaMatrixd> _data;

    };
}

#endif