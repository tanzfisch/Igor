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
#include <queue>
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

        /*! life of particle in seconds
        */
        float32 _life = 1.0;

        /*! visible time in seconds
        */
        float32 _visibleTime = 0.0;

        /*! individual visible time increase
        */
        float32 _visibleTimeIncrease = 0.0;

        /*! 
        */
        float32 _size = 1.0;
        float32 _sizeScale = 1.0;

        bool _visible = true;
        bool _imune = false;

        iaVector2f _phase0;
        iaVector2f _phase1;

        iaVector3f _normal;
        float32 _torque = 0;
        float32 _vortexRange = 0;
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

        /*! defines how much the lift of each particle decreases each frame

        \param decrease decrease of lift per frame
        */
        void setLiftDecrease(float32 decrease);

        /*! \returns the lift decrease per frame
        */
        float32 getLiftDecrease();

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

        /*! \returns particles of current frame
        */
        const deque<iParticle>& getCurrentFrame() const;

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

        /*! \returns true if particle system is finished
        */
        bool isFinished() const;

        /*! sets if the particle system runs in a loop

        \param loop if true particle system runs in loop
        */
        void setLoop(bool loop = true);

        /*! \returns true if particle system runs in loop
        */
        bool getLoop() const;

        void setParticleSystemMatrix(const iaMatrixf& worldInvMatrix);

        /*! sets the likeliness of a vortex particle to appear

        \param likeliness value from 0 to 100 in percent
        */
        void setVortexParticleLikeliness(uint64 likeliness);

        /*! \returns likeliness of vortex particle to appear
        */
        float32 getVortexParticleLikeliness() const;

        /*! calculates next frame

        \param emitter the emitter to emitt particles from
        */
        void calcNextFrame(const iParticleEmitter& emitter);

        /*! sets vortex check range

        \param particles distance in indexes from vortex particle
        */
        void setVortexCheckRange(uint32 particles);

        /*! \return vortex check rangein indexes from vortex particle
        */
        uint32 getVortexCheckRange();

        /*! sets vorticity confinement force

        \param vorticityConfinement the vorticity confinement force
        */
        void setVorticityConfinement(float32 vorticityConfinement);

        /*! \returns vorticity confinement force
        */
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

        /*! sets min max start size gradient for particles at birth

        \param sizeGradient the min max start size gradient
        */
        void setStartSizeGradient(const iGradientVector2f& sizeGradient);

        /*! returns the min max start size gradient for particles at birth

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

        /*! sets min max start lift gradient for particles at birth

        negative values are basically extra weight on a particle

        \param liftGradient the min max start lift gradient
        */
        void setStartLiftGradient(const iGradientVector2f& liftGradient);

        /*! returns the min max start lift gradient for particles at birth

        \param[out] liftGradient out value for the start lift gradient
        */
        void getStartLiftGradient(iGradientVector2f& liftGradient) const;

        /*! sets the air drag

        0.0-1.0
        1.0 means basically no air drag
        0.0 is a full stop
        and about 0.99 is a usefull value for simulating air drag

        \param airDrag the air drag factor (0.0 - 1.0)
        */
        void setAirDrag(float32 airDrag);

        /*! \returns air drag factor
        */
        float32 getAirDrag() const;

        /*! sets period time of this particle system

        if loop is true. the particle system will be restarted after this time
        if loop is false. the particle system simulation will be stopped

        \param periodTime the period time in seconds
        */
        void setPeriodTime(float32 periodTime);

        /*! \returns period time of this particle system in seconds
        */
        float32 getPeriodTime() const;

        iParticleSystem3D();
        virtual ~iParticleSystem3D();

    private:

        /*! true if particle system is finished
        */
        bool _finished = false;

        /*! true if particle system runs in endless loop
        */
        bool _loop = true;

        /*! true if particle system is currently running. false if paused or finished
        */
        bool _running = false;

        iaMatrixf _particleSystemInvWorldMatrix;

        iaMatrixf _birthTransformationMatrix;

        /*! works like a dirty flag. if true all is set to beginning
        */
        bool _mustReset = true;

        /*! color gradient for particles during their lifetime
        */
        iGradientColor4f _colorGradient;

        /*! gradient how the torque of vortex particles changes over time
        
        for internal use only
        */
        iGradientf _torqueFactorGradient;

        /*! start visible time gradient
        */
        iGradientVector2f _startVisibleTimeGradient;

        /*! emission rate gradient during particle system lifetime
        */
        iGradientui _emissionRateGradient;

        /*! size modoification gradient during particle system lifetime
        */
        iGradientf _sizeScaleGradient;

        /*! min max start sizes of particles
        */
        iGradientVector2f _startSizeGradient;

        /*! min max start velocity of particles
        */
        iGradientVector2f _startVelocityGradient;

        /*! min max start lift of particles
        */
        iGradientVector2f _startLiftGradient;

        /*! particle system period time in ms
        */
        float32 _particleSystemPeriodTime = 1000.0;

        /*! life time of any particle

        calculated based on longest visible time
        */
        uint32 _lifeTime = 0;

        float32 _airDrag = 1.0;

        iaVector2f _octave1Shift = { 0.001, 0.001 };
        iaVector2f _octave2Shift = { 0.001, 0.001 };

        float32 _octave1Rotation = 0.001;
        float32 _octave2Rotation = -0.001;

        float64 _startTime = 0;
        float64 _time = 0;

        /*!
        */
        uint64 _vortexLikeliness = 0;
        uint64 _particleCounter = 0;

        float32 _minVortexTorque = 0.1;
        float32 _maxVortexTorque = 0.5;
        float32 _minVortexRange = 10.0;
        float32 _maxVortexRange = 20.0;

        long _vortexCheckRange = 20;

        float32 _vorticityConfinement = 0.1;

        deque<iParticle> _particles;
        
        void initDefaultGradients();

        void createParticles(uint32 particleCount, const iParticleEmitter& emitter, float32 particleSystemTime);
        void resetParticle(iParticle &particle, const iParticleEmitter& emitter, float32 particleSystemTime);
    };

};

#endif
