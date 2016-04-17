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
#include <iTexture.h>

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

    class Igor_API iParticle
    {
    public:

        iaVector3f _position;
        iaVector3f _velocity;

        float32 _lift;
        float32 _weight;
        float32 _lifeTime = 1.0;
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

        iaVector3f _vortexNormal;

        float32 _force;
        float32 _forceRange;
        long _particleid;
    };

    class Igor_API iParticleSystem3D
    {

    public:

        void activateRainbow(bool b);
        void setColor(iaColor4f &color);
        iRainbow& getRainbow();

        void setParticleCount(uint32 count);
        uint32 getParticleCount();
        void setParticleLifeTime(uint32 frames);
        uint32 getParticleLifeTime();

        void setTextureA(const iaString& texture);
        void setTextureB(const iaString& texture);
        void setTextureC(const iaString& texture);
        shared_ptr<iTexture> getTextureA() const;
        shared_ptr<iTexture> getTextureB() const;
        shared_ptr<iTexture> getTextureC() const;

        void setVorticityConfinement(float32 vc);
        float32 getVorticityConfinement();

        void setVortexParticleCount(uint32 count);
        void setVortexRotationSpeed(float32 min, float32 max);
        void setVortexRange(float32 min, float32 max);
        void setVortexCheckRange(uint32 particles);

        uint32 getVortexParticleCount();
        float32 getVortexRotationSpeedMin();
        float32 getVortexRotationSpeedMax();
        float32 getVortexRangeMin();
        float32 getVortexRangeMax();
        uint32 getVortexCheckRange();

        void setLoopAbility(bool loop);
        void setPhaseShift(float32 r1, float32 r2);
        void setLift(float32 min, float32 max);
        void setLiftDecrease(float32 ld);
        void setWeight(float32 min, float32 max);
        void setSize(float32 min, float32 max);
        void setSizeIncrease(float32 si);

        bool getLoopAbility();
        float32 getPhaseShift0();
        float32 getPhaseShift1();
        float32 getLiftMin();
        float32 getLiftMax();
        float32 getLiftDecrease();
        float32 getWeightMin();
        float32 getWeightMax();
        float32 getSizeMin();
        float32 getSizeMax();
        float32 getSizeIncrease();

        void reset();

        vector<iParticle*> getCurrentFrame();
        vector<iVortexParticle*> getCurrentFrameVortex();
        void calcNextFrame();
        void calcNextFrame(iaMatrixf &modelview);

        iParticleSystem3D();
        virtual ~iParticleSystem3D();

    protected:

        bool _mustReset = true;

        uint32 _particleCount = 100;
        uint32 _vortexCount = 5;

        uint32 _lifeTime = 200;
        uint32 _initFrame = 200;
        float32 _lifeTimeStep = 0;

        float32 _minLift = 0.002f;
        float32 _maxLift = 0.004f;
        float32 _reduceLiftStep = 0.000005f;

        float32 _minWeight = 0.001;
        float32 _maxWeight = 0.002;

        float32 _minSize = 1.0;
        float32 _maxSize = 2.0;
        float32 _sizeIncreaseStep = 0.01;

        float32 _minVRot = 1.0;
        float32 _maxVRot = 2.0;
        float32 _minVRange = 30.0;
        float32 _maxVRange = 60.0;

        bool _loopable = false;
        float32 _phaseShiftR1 = 0.001;
        float32 _phaseShiftR2 = -0.001;

        bool _singleColor = true;
        iaColor4f _color;
        iRainbow _rainbow;

        long _vortexCheckRange = 30;

        float32 _vorticityConfinement = 0.1;

        shared_ptr<iTexture> _textureA;
        shared_ptr<iTexture> _textureB;
        shared_ptr<iTexture> _textureC;

        vector<iParticle*> _particles;
        vector<iParticle> _particlesBirth;

        vector<iVortexParticle*> _vortexParticles;

        void calcBirth();
        void resetParticle(iParticle *particle, iaMatrixf &modelview);
    };

};

#endif
