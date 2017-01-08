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
#include <iParticleEmitter.h>

#include <vector>
#include <map>
using namespace std;

namespace Igor
{

    /*! node to control an emitter for a particle system
    */
	class Igor_API iNodeEmitter : public iNode
	{

        friend class iNodeFactory;
        friend class iNodeParticleSystem;

	public:

        /*! sets the type of the emitter

        \param emitterType emitter type
        */
        void setType(iEmitterType emitterType);

        /*! \returns the type of the emitter
        */
        iEmitterType getType() const;

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

        /*! adds triangle to mesh emitter

        \param triangle the triangle to add
        */
		void addTriangle(const iEmitterTriangle& triangle);

        /*! removes all emitter triangles from mesh
        */
        void clearTriangles();

        /*! \returns current world matrix of this node
        */
        const iaMatrixd& getWorldMatrix() const;

    private:

        /*! the actual particle emitter strucure
        */
        iParticleEmitter _emitter;

        /*! matrix including position an orientation of this emitter in world coordinates
        */
        iaMatrixd _worldMatrix;

        /*! set the world matrix of the emitter

        \param matrix world matrix for the emitter
        */
        void onUpdateTransform(iaMatrixd& matrix);

        /*! \returns internal particle emitter structure
        */
        iParticleEmitter& getParticleEmitter();

        /*! init members
        */
        iNodeEmitter();

        /*! copy ctor

        \param node the node to copy from
        */
        iNodeEmitter(iNodeEmitter* node);

        /*! does nothing
        */
        virtual ~iNodeEmitter() = default;

	};

};

#endif
