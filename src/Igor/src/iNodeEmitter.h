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
#include <iTexture.h>
#include <iScene.h>

#include <vector>
using namespace std;

namespace Igor
{

	class Igor_API iParticle
	{
	public:

		iaVector3f position;
		iaVector3f velocity;

		float32 lift;
		float32 weight;
		float32 lifetime;
		float32 visibletime;
		float32 visibletimedecrease;
		float32 size;

		bool visible;
		bool imune;

		iaVector2f phase0;
		iaVector2f phase1;
	};

	class Igor_API iVortexParticle : public iParticle
	{
	public:

		iaVector3f vortexnormal;

		float32 force;
		float32 forcerange;
		long particleid;
	};

	class Igor_API iTriangleEmitter
	{
	public:

		iaVector3f pos[3];
		iaVector3f vel[3];
	};

/*	class Igor_API iParticleEmitterNode : public iNode
	{
		friend class iParticleSystemNode;

	private:

		void preRenderCustom();
		void doRenderCustom();
		void transRenderCustom();

		void calcRandomStart(iParticle *particle, iaMatrixf &modelview);

		iaMatrixf modelviewmatrix;

		vector<iTriangleEmitter> triangleemitters;

	public:

		void addTriangleEmitter(iTriangleEmitter emitter);

		iParticleEmitterNode();
		virtual ~iParticleEmitterNode();
	};

	class Igor_API iParticleSystemNode : public iNodeRender // todo volume node
	{

	public:

		void activateRainbow(bool b);
		void setColor(iaColor4f &color);
		iRainbow& getRainbow();

		void setParticleCount(uint32 count);
		uint32 getParticleCount();
		void setParticleLifeTime(uint32 frames);
		uint32 getParticleLifeTime();

		iRenderStateSet* getRendererState();

		void setTextureA(iaString texture);
		void setTextureB(iaString texture);
		void setTextureC(iaString texture);
		shared_ptr<iTexture> getTextureA();
		shared_ptr<iTexture> getTextureB();
		shared_ptr<iTexture> getTextureC();

		void setEmitter(iParticleEmitterNode *emitter);

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

		void activateEmitter(bool active);
		bool emitterIsActive();

		iParticleSystemNode();
		virtual ~iParticleSystemNode();

        protected:

        void calcBirth();
        void resetParticle(iParticle *particle, iaMatrixf &modelview);

        void preRenderCustom();
        void doRenderCustom();
        void transRenderCustom();

        vector<iParticle*> particles;
        vector<iParticle> particlesbirth;

        vector<iVortexParticle*> vortexparticles;

        iRenderStateSet renderer_state_set;

        bool mustreset;

        //to serialize
        iParticleEmitterNode *emitter;

        uint32 particlecount;
        uint32 vortexcount;

        uint32 lifetime;
        float32 lifetimedecrease;
        uint32 initframe;

        bool activeemitter;

        float32 minlift,maxlift;
        float32 liftdecrease;

        float32 minweight,maxweight;

        float32 minsize,maxsize;
        float32 sizeincrease;

        float32 minvrot,maxvrot;
        float32 minvrange,maxvrange;

        bool loopable;
        float32 phaseshift_r1;
        float32 phaseshift_r2;

        bool singlecolor;
        iaColor4f color;
        iRainbow _rainbow;

        long vortexcheckrange;

        float32 vorticityconfinement;

        shared_ptr<iTexture> textureA;
        shared_ptr<iTexture> textureB;
        shared_ptr<iTexture> textureC;
	};*/

};

#endif
