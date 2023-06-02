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

#ifndef __IGOR_PHYSICSMATERIAL__
#define __IGOR_PHYSICSMATERIAL__

#include <igor/iDefines.h>

#include <iaux/data/iaString.h>
using namespace iaux;

namespace igor
{

    /*! physics material id defintiion
    */
    typedef int64 iPhysicsMaterialID;

    /*! physics material
    */
    class IGOR_API iPhysicsMaterial
    {

        friend class iPhysics;

    public:
        static const iPhysicsMaterialID INVALID_PHYSICSMATERIAL_ID = 0;

        /*! \returns material ID
        */
        iPhysicsMaterialID getID();

        /*! sets name of material

        \param name the new name
        */
        void setName(const iaString &name);

        /*! \returns material name
        */
        const iaString &getName() const;

    private:
        /*! material name
        */
        iaString _name;

        /*! id of physics material
        */
        iPhysicsMaterialID _id = INVALID_PHYSICSMATERIAL_ID;

        /*! initializes members

        \param id physics material id
        */
        iPhysicsMaterial(iPhysicsMaterialID id);

        /*! does nothing
        */
        virtual ~iPhysicsMaterial() = default;
    };

} // namespace igor

#endif // __IGOR_PHYSICSMATERIAL__
