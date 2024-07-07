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
// (c) Copyright 2012-2024 by Martin Loga
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

#ifndef IGOR_SKYBOX_H
#define IGOR_SKYBOX_H

#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMesh.h>

namespace igor
{

    /*! renders a sky box
    */
    class iSkyBox
    {
    public:
        /*! initializes member variables
        */
        iSkyBox();

        /*! set the textures used for the sky box

        \param texture the texture to use
        */
        void setTexture(iTexturePtr texture);

        /*! sets the sky box size (radius)

        \param boxSize the sky box size
        */
        void setBoxSize(float32 boxSize);

        /*! \returns the sky box size
        */
        float32 getBoxSize() const;

        /*! draw the sky box
        */
        void render();

    private:
        /*! scale of the box
        */
        float32 _boxSize = 10.0f;

        /*! mesh for sky box
        */
        iMeshPtr _mesh;

        /*! material to render with
        */
        iMaterialPtr _material;

        /*! builds up a mesh for later rendering
        */
        iMeshPtr buildMesh();
    };

} // namespace igor

#endif // IGOR_SKYBOX_H