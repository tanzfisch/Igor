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

#ifndef __iPARTICLEEMITTER__
#define __iPARTICLEEMITTER__

#include <iDefines.h>

#include <iaMatrix.h>
using namespace IgorAux;

#include <vector>
using namespace std;

namespace Igor
{

    /*! type of emitter
    */
    enum class Igor_API iEmitterType
    {
        Mesh,
        Point,
        Disc,
        Circle,
        Sphere,
        Square,
        Cube
    };

    /*! a triangle of an emitter mesh
    */
    class Igor_API iEmitterTriangle
    {
    public:

        /*! 3 times vertex position
        */
        iaVector3f pos[3];

        /*! 3 times particle velocity
        */
        iaVector3f vel[3];
    };

    /*! basically a structure that delivers start positions and velocities for any kind of objects

    mainly used for particles
    */
    class Igor_API iParticleEmitter
    {

    public:

        /*! does nothing
        */
        iParticleEmitter() = default;

        /*! does nothing
        */
        virtual ~iParticleEmitter() = default;

        /*! sets world matrix of emitter

        \param matrix the matrix to be set
        */
        void setMatrix(const iaMatrixf& matrix);

        /*! \returns world matrix of emitter
        */
        const iaMatrixf& getMatrix() const;

        /*! sets size of emitter

        how it's interpreted depends on type of emitter
        Mesh -> none
        Point -> none
        Disc -> radius
        Circle -> radius
        Sphere -> radius
        Square -> half edge lenght
        Cube -> half edge lenght

        \param size size of emitter
        */
        void setSize(float32 size);

        /*! \returns size of emitter
        */
        float32 getSize() const;

        /*! sets the start velocity

        how it's interpreted depends on type of emitter
        Mesh -> scales with the velocities in mesh
        Point -> scales to 1 in random direction
        Disc -> scales to 1 in Y direction
        Circle -> scales to 1 in Y direction
        Sphere -> scales to 1 in random direction
        Square -> scales to 1 in Y direction
        Cube -> scales to 1 in random direction
        */
        void setVelocity(float32 velocity);

        /*! \returns start velocity
        */
        float32 getVelocity() const;

        /*! sets type of emitter

        \param emitterType emitter type
        */
        void setType(iEmitterType emitterType);

        /*! \returns type of emitter
        */
        iEmitterType getType() const;

        /*! calculates a random start position and velocity from emitter

        \param[out] position random position on emitter in world coordinates
        \param[out] velocity velocity depending on position
        */
        void calcRandomStart(iaVector3f& position, iaVector3f& velocity) const;

    protected:

        /*! type of emitter
        */
        iEmitterType _type = iEmitterType::Point;

        /*! size of emitter
        */
        float32 _size = 1.0;

        /*! start velocity
        */
        float32 _velocity = 0.0;

        /*! mesh used as emitter
        */
        vector<iEmitterTriangle>* _emitterTriangles = nullptr;

        /*! world matrix of emitter
        */
        iaMatrixf _matrix;

        void calcRandomStartFromCube(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromSquare(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromSphere(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromDisc(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromCircle(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromPoint(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromMesh(iaVector3f& position, iaVector3f& velocity) const;

    };

};

#endif
