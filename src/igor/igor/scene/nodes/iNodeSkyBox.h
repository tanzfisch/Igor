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

#ifndef __IGOR_SKYBOXNODE__
#define __IGOR_SKYBOXNODE__

#include <igor/scene/nodes/iNodeRender.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/mesh/iMesh.h>

namespace igor
{

    /*! renders a sky box
    */
    class IGOR_API iNodeSkyBox : public iNodeRender
    {

        friend class iNodeManager;

    public:
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

    private:
        /*! scale of the box
        */
        float32 _boxSize = 10.0f;

        /*! mesh for sky box
        */
        iMeshPtr _mesh;

        /*! initializes member variables
        */
        iNodeSkyBox();

        /*! copy ctor
        */
        iNodeSkyBox(iNodeSkyBox *node);

        /*! releases textures
        */
        virtual ~iNodeSkyBox() = default;

        /*! draw the sky box
        */
        void draw() override;

        /*! builds up a mesh for later rendering
        */
        void buildMesh();
    };

} // namespace igor

#endif