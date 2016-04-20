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

    enum class Igor_API iEmitterType
    {
        Mesh,
        Point
    };

    class Igor_API iEmitterTriangle
    {
    public:

        iaVector3f pos[3];
        iaVector3f vel[3];
    };

    class Igor_API iParticleEmitter
    {

    public:

        iParticleEmitter();
        virtual ~iParticleEmitter();

        void setMatrix(const iaMatrixf& matrix);
        const iaMatrixf& getMatrix() const;

        void setType(iEmitterType emitterType);
        iEmitterType getType() const;

        void calcRandomStart(iaVector3f& position, iaVector3f& velocity) const;

    protected:

        iEmitterType _type = iEmitterType::Point;
        vector<iEmitterTriangle>* _emitterTriangles = nullptr;
        iaMatrixf _matrix;

        void calcRandomStartFromPoint(iaVector3f& position, iaVector3f& velocity) const;
        void calcRandomStartFromMesh(iaVector3f& position, iaVector3f& velocity) const;

    };

};

#endif
