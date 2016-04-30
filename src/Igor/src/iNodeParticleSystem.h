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
#include <iTexture.h>
#include <iParticleSystem3D.h>
#include <iGradient.h>

#include <vector>
using namespace std;

namespace Igor
{

    /*! node to control a 3d particle system
    */
	class Igor_API iNodeParticleSystem : public iNodeVolume
	{

        friend class iNodeFactory;

	public:

        /*! clears all particles
        */
        void reset();

        /*! starts or resumes particle system
        */
        void start();

        /*! stops or pauses the particle system
        */
        void stop();

        /*! \returns true if particle system is currently running
        */
        bool isRunning();

        /*! sets the range of vortex torque

        \param min minimum vortex torque
        \param max maximum vortex torque
        */
        void setVortexTorque(float32 min, float32 max);

        /*! \returns minimum vortex torque
        */
        float32 getVortexTorqueMin();

        /*! \returns maximum vortex torque
        */
        float32 getVortexTorqueMax();

        /*! sets minimum and maximum range of vortexes

        \param min minimum range of vortex
        \param max maximum range of vortex
        */
        void setVortexRange(float32 min, float32 max);

        /*! \returns minimum range of vortexes
        */
        float32 getVortexRangeMin();

        /*! \returns maximum range of vortexes
        */
        float32 getVortexRangeMax();

        /*! sets the min and max values of how much the particles lift them selves agains gravity

        \param min min lift of particles
        \param max max lift of particles
        */
        void setLift(float32 min, float32 max);

        /*! \returns min lift of particles
        */
        float32 getLiftMin();

        /*! \returns min lift of particles
        */
        float32 getLiftMax();

        /*! defines how much the lift of each particle decreases each frame

        \param decrease decrease of lift per frame
        */
        void setLiftDecrease(float32 decrease);

        /*! \returns the lift decrease per frame
        */
        float32 getLiftDecrease();

        /*! sets min max weight of each particle

        \param min min weight of a particle
        \param max max weight of a particle
        */
        void setWeight(float32 min, float32 max);

        /*! \returns min weight of a particle
        */
        float32 getWeightMin();

        /*! \returns max weight of a particle
        */
        float32 getWeightMax();

        /*! sets min max size of particles

        \param min min size of particles
        \param max max size of particles
        */
        void setSize(float32 min, float32 max);

        /*! \returns min size of particles
        */
        float32 getSizeMin();

        /*! \returns max size of particles
        */
        float32 getSizeMax();

        /*! sets particle size increate per frame

        negative values will decrease

        \param increment size increment per frame
        */
        void setSizeIncrease(float32 increment);

        /*! \returns particle size increment per frame
        */
        float32 getSizeIncrease();

        /*! sets emitter node id

        \param emitterID the emitter node's id
        */
        void setEmitter(uint64 emitterID);

        /*! \returns the emitter node's id
        */
        uint64 getEmitter() const;

        /*! sets rainbow gradient for particles color

        \param rainbow the rainbow gradient
        */
        void setColorGradient(const iGradientColor4f& colorGradient);

        /*! returns the rainbow gradient

        \param[out] rainbow out value for the rainbow gradient
        */
        void getColorGradient(iGradientColor4f& colorGradient) const;

        /*! sets emission gradient for particles per frame

        \param emissionGradient the emission gradient
        */
        void setEmissionGradient(const iGradientui& emissionGradient);

        /*! returns the emission gradient

        \param[out] emissionGradient out value for the emission gradient
        */
        void getEmissionGradient(iGradientui& emissionGradient) const;

        /*! sets visible gradient for particles per frame

        \param visibleGradient the visible gradient
        */
        void setStartVisibleTimeGradient(const iGradientf& visibleGradient);

        /*! returns the emission gradient

        \param[out] visibleGradient out value for the visible gradient
        */
        void getStartVisibleTimeGradient(iGradientf& visibleGradient) const;

        /*! sets first texture

        \param filename file name of texture
        */
        void setTextureA(const iaString& filename);

        /*! sets second texture

        \param filename file name of texture
        */
        void setTextureB(const iaString& filename);

        /*! sets third texture

        \param filename file name of texture
        */
        void setTextureC(const iaString& filename);

        /*! \returns file name of first texture
        */
        iaString getTextureA() const;

        /*! \returns file name of second texture
        */
        iaString getTextureB() const;

        /*! \returns file name of third texture
        */
        iaString getTextureC() const;

        /*! draw function
        */
        void draw();

    private:

        /*! inverted world matrix
        */
        iaMatrixf _worldMatrixInv;

        /*! id of emitter node
        */
        uint64 _emitterID = iNode::INVALID_NODE_ID;

        /*! the actual particle system implementation
        */
        iParticleSystem3D _particleSystem;

        /*! first texture 
        */
        shared_ptr<iTexture> _textureA;

        /*! second texture
        */
        shared_ptr<iTexture> _textureB;
        
        /*! third texture
        */
        shared_ptr<iTexture> _textureC;

        /*! called when there is a new transformation matrix for this node

        \param matrix the new world matrix
        */
        void onUpdateTransform(iaMatrixf& matrix);

        /*! init members
        */
        iNodeParticleSystem();

        /*! copy ctor

        \param node make copy from this node
        */
        iNodeParticleSystem(iNodeParticleSystem* node);

        /*! does nothing
        */
        virtual ~iNodeParticleSystem() = default;

    };

};

#endif
