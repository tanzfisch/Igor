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

#ifndef __iPARTICLESYSTEM3D__
#define __iPARTICLESYSTEM3D__

#include <iGradient.h>

#include <iaVector3.h>
#include <iaVector2.h>
#include <iaColor4.h>
#include <iaMatrix.h>
using namespace IgorAux;

#include <memory>
#include <vector>
using namespace std;

namespace Igor
{

    class iParticleEmitter;

    /*! single particle
    */
    class Igor_API iParticle
    {
    public:

        /*! current position of particle
        */
        iaVector3f _position;

        /*! current velocity of particle
        */
        iaVector3f _velocity;

        /*! current lift value
        */
        float32 _lift = 0.0;

        /*! weight of particle
        */
        float32 _weight = 0.0;

        /*! life of particle in seconds
        */
        float32 _life = 1.0;

        /*! visible time in seconds
        */
        float32 _visibleTime = 0.0;
        float32 _size = 1.0;

        bool _visible = true;
        bool _imune = false;

        iaVector2f _phase0;
        iaVector2f _phase1;
    };

    class Igor_API iVortexParticle : public iParticle
    {
    public:

        iaVector3f _normal;

        float32 _torque;

        float32 _range;

        long _particleid;
    };

    class Igor_API iParticleSystem3D
    {

    public:

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

        /*! sets the rotation per frame of the second texture

        \param angle rotation in rad per frame
        */
        void setSecondTextureRotation(float32 angle);

        /*! \returns rotation speed of second texture in rad per frame
        */
        float32 getSecondTextureRotation();

        /*! sets the rotation per frame of the third texture

        \param angle rotation in rad per frame
        */
        void setThirdTextureRotation(float32 angle);

        /*! \returns rotation speed of third texture in rad per frame
        */
        float32 getThirdTextureRotation();

        vector<iParticle*> getCurrentFrame();
        vector<iVortexParticle*> getCurrentFrameVortex();

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

        bool isFinished() const;

        void setLoop(bool loop = true);
        bool getLoop() const;

        void setParticleSystemMatrix(const iaMatrixf& worldInvMatrix);

        void setVortexParticleLikeliness(float32 likeliness);
        float32 getVortexParticleLikeliness() const;

        void calcNextFrame(const iParticleEmitter& emitter);

        /*!
        */
        void setVortexCheckRange(uint32 particles);
        uint32 getVortexCheckRange();

        void setVorticityConfinement(float32 vc);
        float32 getVorticityConfinement();

        /*! sets color gradient

        the gradient must contain colors at given time in seconds

        \param colorGradient the color gradient
        */
        void setColorGradient(const iGradientColor4f& colorGradient);

        /*! returns the color gradient

        the gradient contains colors at given time in seconds

        \param[out] colorGradient out value for the color gradient
        */
        void getColorGradient(iGradientColor4f& colorGradient) const;

        /*! \returns reference to color gradient
        */
        const iGradientColor4f& getColorGradient() const;

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

        /*! sets size gradient for particles per frame

        \param sizeGradient the size gradient
        */
        void setSizeModifierGradient(const iGradientf& sizeGradient);

        /*! returns the emission gradient

        \param[out] sizeGradient out value for the size gradient
        */
        void getSizeModifierGradient(iGradientf& sizeGradient) const;

        iParticleSystem3D();
        virtual ~iParticleSystem3D();

    private:

        bool _finished = false;

        bool _loop = true;

        bool _running = false;

        float32 _vortexLikeliness = 0.1;

        iaMatrixf _particleSystemInvWorldMatrix;

        iaMatrixf _birthTransformationMatrix;

        bool _mustReset = true;

        /*! color gradient for particles during their lifetime
        */
        iGradientColor4f _colorGradient;

        /*! start visible time gradient
        */
        iGradientf _startVisibleTimeGradient;

        /*! emission rate gradient during particle system lifetime
        */
        iGradientui _emissionRateGradient;

        /*! size modoification gradient during particle system lifetime
        */
        iGradientf _sizeModifierGradient;

        /*! start sizes of particles during particles system lifetime
        */
        iGradientf _startSizeGradient;

        iGradientVector3f _velocityGradient;

        float32 _particleSystemPeriodTime = 0;
        uint32 _lifeTime = 200;
        float32 _lifeTimeStep = 0;

        float32 _minLift = 0.0f;
        float32 _maxLift = 0.0f;
        float32 _reduceLiftStep = 0.0f;

        float32 _minWeight = 0.0;
        float32 _maxWeight = 0.0;

        float32 _minSize = 1.0;
        float32 _maxSize = 2.0;
        float32 _sizeIncreaseStep = 0.0;

        float32 _minVortexTorque = 0.1;
        float32 _maxVortexTorque = 0.5;
        float32 _minVRange = 10.0;
        float32 _maxVRange = 20.0;

        iaVector2f _octave1Shift = { 0.001, 0.001 };
        iaVector2f _octave2Shift = { 0.001, 0.001 };

        float32 _octave1Rotation = 0.001;
        float32 _octave2Rotation = -0.001;

        float64 _startTime = 0;
        float64 _time = 0;

        long _vortexCheckRange = 30;

        float32 _vorticityConfinement = 0.1;

        vector<iParticle*> _particles;

        vector<iVortexParticle*> _vortexParticles;

        void initDefaultGradients();

        void createParticles(uint32 particleCount, const iParticleEmitter& emitter, float32 particleSystemTime);
        void resetParticle(iParticle *particle, const iParticleEmitter& emitter, float32 particleSystemTime);
    };

};

#endif
