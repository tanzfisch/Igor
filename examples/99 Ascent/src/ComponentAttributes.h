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

#ifndef __iCOMPONENT_ATTRIBUTES__
#define __iCOMPONENT_ATTRIBUTES__

#include <iComponent.h>

#include <map>
using namespace std;

namespace Igor
{

    enum class Fraction
    {
        Red,
        Blue,
        Green,
        Yellow,
        None
    };

    enum class GameObjectKind
    {
        Vehicle,
        Weapon,
        None
    };

    struct Attributes
    {
        /*! the shield value
        */
        float32 _shield = 0;

        /*! the health value
        */
        float32 _health = 0;

        /*! shield damage value
        */
        float32 _shieldDamage = 0;

        /*! damage value
        */
        float32 _damage = 0;

        /*! fraction the game object belongs to
        */
        Fraction _fraction = Fraction::None;

        /*! kind type of this game object
        */
        GameObjectKind _kind = GameObjectKind::None;
    };

    class ComponentAttributes : public iComponent
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
        ComponentAttributes() = default;

        /*! does nothing
        */
        ~ComponentAttributes() = default;

    private:

        /*! the actual data
        */
        map<uint64, Attributes> _data;

    };
}

#endif