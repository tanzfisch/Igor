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

#ifndef __iNODEPARTICLESYSTEM__
#define __iNODEPARTICLESYSTEM__

#include <iNodeVolume.h>

#include <iParticleSystem3D.h>

#include <vector>
using namespace std;

namespace Igor
{

	class Igor_API iNodeParticleSystem : public iNodeVolume
	{

	public:

        iNodeParticleSystem();
        iNodeParticleSystem(iNodeParticleSystem* node);
		virtual ~iNodeParticleSystem();

        iParticleSystem3D& getParticleSystem();

        void setEmitter(uint64 emitterID);
        uint64 getEmitter() const;

        void draw();

    private:

        /*! inverted world matrix
        */
        iaMatrixf _worldMatrixInv;

        /*! id of emitter node
        */
        uint64 _emitterID = iNode::INVALID_NODE_ID;

        iParticleSystem3D _particleSystem;

        void onUpdateTransform(iaMatrixf& matrix);

    };

};

#endif
