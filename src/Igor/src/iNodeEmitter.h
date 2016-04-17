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

#ifndef __iNODEEMITTER__
#define __iNODEEMITTER__

#include <iNode.h>
#include <iaVector3.h>

#include <vector>
using namespace std;

namespace Igor
{

    class Igor_API iEmitterTriangle
	{
	public:

		iaVector3f pos[3];
		iaVector3f vel[3];
	};

	class Igor_API iNodeEmitter : public iNode
	{
		friend class iNodeParticleSystem;

	public:

        iNodeEmitter();
        virtual ~iNodeEmitter();

        /*! 
        */
		void addTriangleEmitter(const iEmitterTriangle& emitter);

        /*! calculates random start point in emitter in world coordinates

        also calculates the velocity at that start point
        */
        void calcRandomStart(iaVector3f& position, iaVector3f& velocity);

    private:

        /*! matrix including position an orientation of this emitter in world coordinates
        */
        iaMatrixf _worldMatrixInv;

        iaMatrixf _modelviewmatrix;

        /*! set the world matrix of the emitter

        \param matrix world matrix for the emitter
        */
        void onUpdateTransform(iaMatrixf& matrix);

        vector<iEmitterTriangle> _emitterTriangles;

	};

};

#endif
