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
// (c) Copyright 2012-2020 by Martin Loga
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

#include <igor/iDefines.h>

#include <iaux/math/iaMatrix.h>
#include <iaux/math/iaRandomNumberGenerator.h>
using namespace IgorAux;

#include <vector>

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
        iaVector3d pos[3];

        /*! 3 times particle velocity
        */
        iaVector3d vel[3];
    };

    /*! basically a structure that delivers start positions and velocities for any kind of objects

    mainly used for particles
    */
    class Igor_API iParticleEmitter
    {

    public:
        /*! does nothing
        */
        iParticleEmitter();

        /*! does nothing
        */
        virtual ~iParticleEmitter() = default;

        /*! sets world matrix of emitter

        \param matrix the matrix to be set
        */
        void setWorldMatrix(const iaMatrixd &matrix);

        /*! \returns world matrix of emitter
        */
        const iaMatrixd &getWorldMatrix() const;

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

        /*! sets type of emitter

        \param emitterType emitter type
        */
        void setType(iEmitterType emitterType);

        /*! \returns type of emitter
        */
        iEmitterType getType() const;

        /*! adds triangle to mesh emitter

        \param triangle the triangle to add
        */
        void addTriangle(const iEmitterTriangle &triangle);

        /*! removes all emitter triangles from mesh
        */
        void clearTriangles();

        /*! calculates a random start position and velocity from emitter

        \param[out] position random position on emitter in world coordinates
        \param[out] velocity velocity depending on position
        */
        void calcRandomStart(iaVector3d &position, iaVector3d &velocity);

    protected:
        /*! type of emitter
        */
        iEmitterType _type = iEmitterType::Point;

        /*! size of emitter
        */
        float32 _size = 0.0;

        /*! mesh used as emitter
        */
        std::vector<iEmitterTriangle> _emitterTriangles;

        /*! world matrix of emitter
        */
        iaMatrixd _worldMatrix;

        /*! random number generator
        */
        iaRandomNumberGeneratoru _rand;

        void calcRandomStartFromCube(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromSquare(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromSphere(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromDisc(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromCircle(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromPoint(iaVector3d &position, iaVector3d &velocity);
        void calcRandomStartFromMesh(iaVector3d &position, iaVector3d &velocity);
    };

}; // namespace Igor

#endif
