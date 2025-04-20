//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \/___/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2012-2025 by Martin A. Loga
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

#ifndef IGOR_MATERIALIO_H
#define IGOR_MATERIALIO_H

#include <igor/resources/material/iMaterial.h>

namespace igor
{
    /*! reader for Igor material files
     */
    class IGOR_API iMaterialIO
    {
    public:
        /*! reads material file and configures given material with it

        \param filename path to material file
        \param material the material to configure with
        */
        static bool read(const iaString &filename, const iMaterialPtr &material);

        /*! writes material to file

        \param filename path to new material file
        \param material the material to use
        */
        static bool write(const iaString &filename, const iMaterialPtr &material);
    };

} // namespace igor

#endif // IGOR_MATERIALIO_H