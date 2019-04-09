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
// (c) Copyright 2012-2019 by Martin Loga
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

#ifndef __iTRANSFORMNODE__
#define __iTRANSFORMNODE__

#include <iNode.h>
#include <iaMatrix.h>

namespace Igor
{

    /*! the transform node
    */
	class Igor_API iNodeTransform : public iNode
	{

		friend class iNodeFactory;
		friend class iNodeVisitorUpdateTransform;

    public:

        /*! set transformation matrix

        \param matrix the transformation matrix
        */
        virtual void setMatrix(const iaMatrixd& matrix);

        /*! returns the transformation matrix

        \param[out] matrix the returned transformation matrix
        */
        virtual void getMatrix(iaMatrixd& matrix);

        /*! high level function to translate the transformation matrix

        \param vector vector to translate the transformation matrix
        */
        virtual void translate(const iaVector3d& vector);

        /*! high level function to translate the transformation matrix

        \param x x component of translation vector
        \param y y component of translation vector
        \param z z component of translation vector
        */
        virtual void translate(float64 x, float64 y, float64 z);

        /*! high level function to set the position of the transformation matrix

        \param vector position to set
        */
        virtual void setPosition(const iaVector3d& vector);

        /*! high level function to set the position of the transformation matrix

        \param x x component of position
        \param y y component of position
        \param z z component of position
        */
        virtual void setPosition(float64 x, float64 y, float64 z);

        /*! high level function to scale the transformation matrix

        \param vector vector to scale the transformation matrix
        */
        virtual void scale(const iaVector3d& vector);

        /*! high level function to scale the transformation matrix

        \param x x component of scale vector
        \param y y component of scale vector
        \param z z component of scale vector
        */
        virtual void scale(float64 x, float64 y, float64 z);

        /*! applies rotation to matrix

        \param angle angle to rotate in radians
        \param axis the axis to rotate around
        */
        virtual void rotate(float64 angle, iaAxis axis);

        /*! applies 3 rotations to matrix in order X,Y,Z

        \param angleX angle to rotate arround X axis
        \param angleY angle to rotate arround Y axis
        \param angleZ angle to rotate arround Z axis
        */
        virtual void rotate(float64 angleX, float64 angleY, float64 angleZ);

        /*! resets the transformation matrix to an identity matrix
        */
        virtual void identity();

    private:

        /*! transformation matrix
        */
        iaMatrixd _transform;

        /*! called by update transform run

        this node will modifiey the matrix

        \parma[in, out] matrix current transform matrix
        */
        void onUpdateTransform(iaMatrixd& matrix);
        
        /*! initializes memeber varialbes
        */
		iNodeTransform();

        /*! copy ctor
        */
        iNodeTransform(iNodeTransform* node);

        /*! does nothing
        */
		virtual ~iNodeTransform() = default;
		
	};

};

#endif
