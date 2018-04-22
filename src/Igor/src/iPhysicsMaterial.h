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
// (c) Copyright 2012-2018 by Martin Loga
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

#ifndef __iPHYSICSMATERIAL__
#define __iPHYSICSMATERIAL__

#include <iDefines.h>

#include <iaString.h>
using namespace IgorAux;

namespace Igor
{

    /*! physics material
    */
    class Igor_API iPhysicsMaterial
    {

        friend class iPhysics;
        
    public:

        static const int64 INVALID_PHYSICSMATERIAL_ID = 0;

        /*! \returns material ID
        */
        int64 getID();

        /*! sets name of material

        \param name the new name
        */
        void setName(const iaString& name);

        /*! \returns material name
        */
        const iaString& getName() const;

    private:

        /*! material name
        */
        iaString _name;

        /*! id of material
        */
        int64 _id = INVALID_PHYSICSMATERIAL_ID;

        /*! initializes members

        \param id newton material id
        */
        iPhysicsMaterial(int64 id);

        /*! does nothing
        */
        virtual ~iPhysicsMaterial() = default;

    };
    
}

#endif
