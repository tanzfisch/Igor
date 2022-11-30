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
// (c) Copyright 2012-2022 by Martin Loga
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
#include <igor/resources/texture/iTextureResourceFactory.h>

namespace igor
{

    /*! renders a sky box
    */
    class IGOR_API iNodeSkyBox : public iNodeRender
    {

        friend class iNodeManager;

    public:
        /*! draw the sky box
        */
        virtual void draw();

        /*! sets texture coordinates scaling

        \param scale scale of texture coordinates
        */
        void setTextureScale(float32 scale);

        /*! \returns texture scale
        */
        float32 getTextureScale() const;

        /*! sets the offset matrix

        \param offsetMatrix the offset matrix to set
        */
        void setOffsetMatrix(iaMatrixd &offsetMatrix);

        /*! returns the offset matrix

        \param offsetMatrix in out value for the offset matrix
        */
        void getOffsetMatrix(iaMatrixd &offsetMatrix);

        /*! switches the usage of the offset matrix on and off

        \param useMatrix true: offset matrix is used; false: no offset matrix is used
        */
        void setUseOffsetMatrix(bool useMatrix);

        /*! \returns true if offset matrix is used
        */
        bool isOffsetMatrixUsed() const;

        /*! set the textures used for the sky box

        \param front front texture
        \param back back texture
        \param left left texture
        \param right right texture
        \param top top texture
        \param bottom bottom texture
        */
        void setTextures(iTexturePtr front, iTexturePtr back, iTexturePtr left, iTexturePtr right, iTexturePtr top, iTexturePtr bottom);

        /*! sets the sky box size

        \param boxSize the sky box size
        */
        void setBoxSize(float32 boxSize);

        /*! \returns the sky box size
        */
        float32 getBoxSize() const;

    private:
        float32 _boxSize = 10.0f;

        /*! texture scaling
        */
        float32 _textureScale = 1.0f;

        /*! true if the matrix has to be set as offset on top of the camera matrix
        */
        bool _useMatrix = false;

        /*! offset matrix
        */
        iaMatrixd _offsetMatrix;

        /*! shared pointer to front texture
        */
        iTexturePtr _front;

        /*! shared pointer to back texture
        */
        iTexturePtr _back;

        /*! shared pointer to left texture
        */
        iTexturePtr _left;

        /*! shared pointer to right texture
        */
        iTexturePtr _right;

        /*! shared pointer to top texture
        */
        iTexturePtr _top;

        /*! shared pointer to bottom texture
        */
        iTexturePtr _bottom;

        /*! initializes member variables
        */
        iNodeSkyBox();

        /*! copy ctor
        */
        iNodeSkyBox(iNodeSkyBox *node);

        /*! releases textures
        */
        virtual ~iNodeSkyBox();

        /*! builds up a mesh for later rendering
        */
        void buildMesh();
    };

} // namespace igor

#endif