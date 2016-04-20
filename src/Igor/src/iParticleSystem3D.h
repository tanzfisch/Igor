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

#include <iRainbow.h>

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

    class Igor_API iParticle
    {
    public:

        iaVector3f _position;
        iaVector3f _velocity;

        float32 _lift;
        float32 _weight;
        float32 _life = 1.0;
        float32 _visibleTime;
        float32 _visibleTimeStep;
        float32 _size;

        bool _visible;
        bool _imune;

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

        /*! sets particle count

        \param count particle count
        */
        void setParticleCount(uint32 count);

        /*! \returns particle count
        */
        uint32 getParticleCount();

        /*! sets particles lifetime in frames

        \param frames particle life time in frames 
        */
        void setParticleLifeTime(uint32 frames);

        /*! \returns particle lifetime in frames
        */
        uint32 getParticleLifeTime();

        /*! sets vortex particle count

        values can go from zero to particle count

        \param count vortex particle count
        */
        void setVortexParticleCount(uint32 count);

        /*! \returns vortex particle count
        */
        uint32 getVortexParticleCount();

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

        /*! activates or deactivates loop ability

        if loob ability is on the particle system will repeadetly apear the same in preiod of it's lifetime

        \param loop flag if true loop ability will be switched on
        */
        void setLoop(bool loop = true);

        /*! \returns true if loop ability is switched on
        */
        bool getLoop();

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

        void reset(const iParticleEmitter& emitter);

        vector<iParticle*> getCurrentFrame();
        vector<iVortexParticle*> getCurrentFrameVortex();

        void setParticleSystemMatrix(const iaMatrixf& worldInvMatrix);

        void calcNextFrame(const iParticleEmitter& emitter);

        /*!
        */
        void setVortexCheckRange(uint32 particles);
        uint32 getVortexCheckRange();

        void setVorticityConfinement(float32 vc);
        float32 getVorticityConfinement();


        iParticleSystem3D();
        virtual ~iParticleSystem3D();

    protected:

        iaMatrixf _particleSystemInvWorldMatrix;

        iaMatrixf _birthTransformationMatrix;

        bool _mustReset = true;

        uint32 _particleCount = 300;
        uint32 _vortexCount = 15;

        uint32 _lifeTime = 200;
        uint32 _initFrame = 0;
        float32 _lifeTimeStep = 0;

        float32 _minLift = 0.002f;
        float32 _maxLift = 0.004f;
        float32 _reduceLiftStep = 0.000005f;

        float32 _minWeight = 0.001;
        float32 _maxWeight = 0.002;

        float32 _minSize = 1.0;
        float32 _maxSize = 2.0;
        float32 _sizeIncreaseStep = 0.01;

        float32 _minVortexTorque = 0.1;
        float32 _maxVortexTorque = 0.5;
        float32 _minVRange = 10.0;
        float32 _maxVRange = 20.0;

        bool _loopable = false;

        iaVector2f _octave1Shift = { 0.001, 0.001 };
        iaVector2f _octave2Shift = { 0.001, 0.001 };

        float32 _octave1Rotation = 0.001;
        float32 _octave2Rotation = -0.001;

        long _vortexCheckRange = 30;

        float32 _vorticityConfinement = 0.1;

        vector<iParticle*> _particles;
        vector<iParticle> _particlesBirth;

        vector<iVortexParticle*> _vortexParticles;

        void calcBirth(const iParticleEmitter& emitter);
        void resetParticle(iParticle *particle, const iParticleEmitter& emitter);
    };

};

#endif
