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

#ifndef IGOR_SHADER_MATERIALIO_H
#define IGOR_SHADER_MATERIALIO_H

#include <igor/resources/shader_material/iShaderMaterial.h>

class TiXmlElement;

namespace igor
{
    /*! reader for Igor shader material files
     */
    class IGOR_API iShaderMaterialIO
    {
    public:
        /*! reads material file and configures given material with it

        \param filename path to material file
        \param material the material to configure with
        */
        static bool read(const iaString &filename, const iShaderMaterialPtr &material);

        /*! writes material to file

        \param filename path to new material file
        \param material the material to use
        */
        static bool write(const iaString &filename, const iShaderMaterialPtr &material);

    private:

        /*! read material info and put it in to shader material

        \param materialXML the material element
        \param material the shader material
        */
        static bool readMaterial(TiXmlElement *materialXML, const iShaderMaterialPtr &material);
    };

} // namespace igor

#endif // IGOR_SHADER_MATERIALIO_H