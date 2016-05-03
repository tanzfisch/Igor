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
        void setStartVisibleTimeGradient(const iGradientVector2f& visibleGradient);

        /*! returns the emission gradient

        \param[out] visibleGradient out value for the visible gradient
        */
        void getStartVisibleTimeGradient(iGradientVector2f& visibleGradient) const;

        /*! sets size scale gradient for particles per frame

        \param sizeScaleGradient the size gradient
        */
        void setSizeScaleGradient(const iGradientf& sizeScaleGradient);

        /*! returns the size scale gradient

        \param[out] sizeScaleGradient out value for the size gradient
        */
        void getSizeScaleGradient(iGradientf& sizeScaleGradient) const;

        /*! sets start size gradient for particles at birth

        \param sizeGradient the start size gradient with min and max values
        */
        void setStartSizeGradient(const iGradientVector2f& sizeGradient);

        /*! returns the start size gradient for particles at birth

        \param[out] sizeGradient out value for the start size gradient
        */
        void getStartSizeGradient(iGradientVector2f& sizeGradient) const;

        /*! sets min max start velocity gradient for particles at birth

        \param velocityGradient the min max start velocity gradient
        */
        void setStartVelocityGradient(const iGradientVector2f& velocityGradient);

        /*! returns the min max start velocity gradient for particles at birth

        \param[out] velocityGradient out value for the start velocity gradient
        */
        void getStartVelocityGradient(iGradientVector2f& velocityGradient) const;

        /*! sets the likeliness of a vortex particle to appear

        \param likeliness value from 0 to 1
        */
        void setVortexParticleLikeliness(float32 likeliness);

        /*! \returns likeliness of vortex particle to appear
        */
        float32 getVortexParticleLikeliness() const;

        /*! sets vorticity confinement force

        \param vorticityConfinement the vorticity confinement force
        */
        void setVorticityConfinement(float32 vorticityConfinement);

        /*! \returns vorticity confinement force
        */
        float32 getVorticityConfinement();

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
