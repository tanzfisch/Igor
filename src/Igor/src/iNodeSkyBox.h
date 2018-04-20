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

#ifndef __iSKYBOXNODE__
#define __iSKYBOXNODE__

#include <iNodeRender.h>
#include <iTextureResourceFactory.h>

namespace Igor
{

    /*! renders a sky box
    */
	class Igor_API iNodeSkyBox : public iNodeRender
	{

		friend class iNodeFactory;

    public:

        /*! \returns true if this node was reached by culling

        the sky box node is always reached!
        */
        virtual bool wasReached();

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
        void setOffsetMatrix(iaMatrixd& offsetMatrix);

        /*! returns the offset matrix

        \param offsetMatrix in out value for the offset matrix
        */
        void getOffsetMatrix(iaMatrixd& offsetMatrix);

        /*! switches the usage of the offset matrix on and off

        \param useMatrix true: offset matrix is used; false: no offset matrix is used
        */
        void setUseOffsetMatrix(bool useMatrix);

        /*! \returns true if offset matrix is used
        */
        bool isOffsetMatrixUsed() const;

        /*! sets the alpha value for rendering the sky box

        this can be used to render multiple sky boxes that layer up

        \param alpha
        */
        void setAlpha(float32 alpha);

        /*! set the textures used for the sky box

        \param front front texture
        \param back back texture
        \param left left texture
        \param right right texture
        \param top top texture
        \param bottom bottom texture
        */
        void setTextures(shared_ptr<iTexture> front, shared_ptr<iTexture> back, shared_ptr<iTexture> left, shared_ptr<iTexture> right, shared_ptr<iTexture> top, shared_ptr<iTexture> bottom);

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

        /*! alpha value
        */
        float32 _alpha = 1.0f;

        /*! true if the matrix has to be set as offset on top of the camera matrix
        */
        bool _useMatrix = false;

        /*! offset matrix
        */
		iaMatrixd _offsetMatrix;

        /*! shared pointer to front texture
        */
		shared_ptr<iTexture> _front;

        /*! shared pointer to back texture
        */
        shared_ptr<iTexture> _back;

        /*! shared pointer to left texture
        */
        shared_ptr<iTexture> _left;
		
        /*! shared pointer to right texture
        */
        shared_ptr<iTexture> _right;
		
        /*! shared pointer to top texture
        */
        shared_ptr<iTexture> _top;
		
        /*! shared pointer to bottom texture
        */
        shared_ptr<iTexture> _bottom;

        /*! initializes member variables
        */
		iNodeSkyBox();

        /*! copy ctor
        */
        iNodeSkyBox(iNodeSkyBox* node);

        /*! releases textures
        */
		virtual ~iNodeSkyBox();

	};

}

#endif