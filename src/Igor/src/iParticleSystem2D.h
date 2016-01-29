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
// (c) Copyright 2014-2015 by Martin Loga
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

#ifndef __iPARTICLESYSTEM2D__
#define __iPARTICLESYSTEM2D__

#include <iTimerHandle.h>
#include <iaVector2.h>
#include <iRainbow.h>

#include <vector>
using namespace std;

namespace Igor
{

	class iTexture;

    /*! emitter types

    \todo DotEmitter, RingEmitter, SqareEmitter, ShapeEmitter
    */
	enum class iEmitterType
	{
		PointEmitter
	};

    /*! single particle and all it's attributes
    */
	class Igor_API iParticle2D
	{
	public:

        /*! current position of particle
        */
		iaVector2f _position;

        /*! current velocity of particle
        */
		iaVector2f _velocity;

        /*! current size of particle
        */
		float32 _size;

        /*! size delta of particle
        */
		float32 _sizeDelta;

        /*! current orientation of particle
        */
		float32 _angle;

        /*! orientation offset
        */
		float32 _angleDelta;

        /*! how much more to live
        */
		float32 _life;
	};

    /*! 2d particle system
    */
	class Igor_API iParticleSystem2D
	{

    public:

        /*! initialize
        */
        iParticleSystem2D();

        /*! clean up
        */
        ~iParticleSystem2D();

        /*! sets particle size range when created

        \param minSize minimum size of particle created
        \param maxSize maximum size of particle created
        */
        void setParticleSize(float32 minSize, float32 maxSize);

        /*! sets particle scale speed range over time

        \param minSizeDelta minimum size scale over time
        \param maxSizeDelta maximum size scale over time
        */
        void setParticleSizeDelta(float32 minSizeDelta, float32 maxSizeDelta);

        /*! sets particle rotation (orientation) range when created

        \param minRotation minimum rotation of particle created
        \param maxRotation maximum rotation of particle created
        */
        void setParticleRotation(float32 minRotation, float32 maxRotation);

        /*! sets particle rotation speed range over time

        \param minRotationDelta minimum rotation speed change over time
        \param maxRotationDelta maximum rotation speed change over time
        */
        void setParticleRotationDelta(float32 minRotationDelta, float32 maxRotationDelta);

        /*! sets particle life time in iterations

        \param iterations life time in iterations
        */
        void setParticleLifetime(int32 iterations);

        /*! sets maximum particle count

        basically to prevent you from endlessly creating particles

        \param count particle count
        */
        void setMaxParticleCount(int32 count);

        /*! \returns maximum particle count
        */
        int32 getMaxParticleCount();

        /*! \returns current in use particle count
        */
        int32 getParticleCount();

        /*! sets gravitation force

        \param gravitation gravitational force
        */
        void setGravitation(const iaVector2f& gravitation);

        /*! sets initial velocity

        \param initialVelocity initial velocity
        */
        void setInitialVelocity(const iaVector2f& initialVelocity);

        /*! sets air drag

        \param airDrag air drag that effects speed of particles over time
        */
        void setAirDrag(float32 airDrag);

        /*! sets spread factor of particle emission

        \param spreadFactor spread factor
        */
        void setSpreadFactor(float32 spreadFactor);

        /*! sets particle system loopable

        \param loop true: particles will be emitted endlessly
        */
        void setLoopable(bool loop);

        /*! sets particle emition rate

        \param emitRate amout of particles emitted per time
        */
        void setEmitRate(int32 emitRate);						

        /*! translates emitter position

        \param v translation vector
        */
        void translateEmitter(iaVector2f v);

        /*! sets emitter position

        \param pos new position of emitter
        */
        void setEmitterPos(iaVector2f pos);

        /*! \returns list of particles

        mainly used for rendering them
        */
        iParticle2D* getParticles();

        /*! \returns true if no more particles will be emitted and all particles lifes have ended
        */
        bool finished();

        /*! handles particle system iteration
        */
        void handle();

	private:

        /*! min size of particle during creation
        */
		float32 _minSize = 10.0f;

        /*! max size of particle during creation
        */
        float32 _maxSize = 10.0f;

        /*! min size change of particle during lifetime
        */
		float32 _minSizeDelta = 0.0f;

        /*! max size change of particle during lifetime
        */
        float32 _maxSizeDelta = 0.0f;

        /*! min rotation (orientation) during creation
        */
        float32 _minRotation = 0.0f;

        /*! max rotation (orientation) during creation
        */
        float32 _maxRotation = 0.0f;

        /*! min rotation speed of particle during lifetime
        */
		float32 _minRotationDelta = 0.0f;

        /*! max rotation speed of particle during lifetime
        */
        float32 _maxRotationDelta = 0.0f;

        /*! life delta reducing particles life every iteration
        */
		float32 _lifeDelta = 0.01f;

        /*! gravitation
        */
        iaVector2f _gravitation = { 0.0f, 0.0f };

        /*! initial velocity
        */
        iaVector2f _initialVelocity = {0.0f, 1.0f};

        /*! air drag
        */
        float32 _airDrag = 0.999f;

        /*! emitter position
        */
        iaVector2f _emitterPos = { 0.0f, 0.0f };

        /*! spread factor
        */
		float32 _spreadFactor = 0.0f;

        /*! emittion rate per iteration
        */
		int32 _emitRate = 1;

        /*! if particles are reused after their lifes end
        */
		bool _rebirth = true;

        /*! maximum particle count
        */
		int32 _maxParticleCount = 0;

        int32 _particleCount = 0;

        /*! emitter type
        */
        iEmitterType _emitterType = iEmitterType::PointEmitter;

        /*! list of particles
        */
		iParticle2D* _particles = nullptr;

        /*! if true particle system has stopped and does not emitt particles enymore
        */
		bool _done = false;

        /*! reset a particle to start values in order to reuse it

        \param particle the particle to reset
        */
		void resetParticle(iParticle2D& particle);

    };

};

#endif
