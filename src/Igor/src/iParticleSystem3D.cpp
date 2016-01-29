// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleSystem3D.h>
#include <iaConsole.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iRainbow.h>

#include <iostream>
using namespace std;

namespace Igor
{

	iParticleSystemNode::iParticleSystemNode()
	{
		mustreset = true;

		initframe = lifetime = 200;
		lifetimedecrease = 1.0f / lifetime;
		particlecount = 1000;
		vortexcount = 10;

		minlift = 0.002f;
		maxlift = 0.004f;

		liftdecrease = 0.000005f;

		minweight = 0.001f;
		maxweight = 0.002f;

		minsize = 10.0f;
		maxsize = 20.0f;

		sizeincrease = 0.1f;

		minvrot = 1.0f;
		maxvrot = 2.0f;

		minvrange = 30.0f;
		maxvrange = 60.0f;

		vortexcheckrange = 30;

		vorticityconfinement = 0.1f;

		loopable = false;
		phaseshift_r1 = 0.0;
		phaseshift_r2 = 0.0;

		emitter = 0;

		singlecolor = true;
		color.set(1,1,1,1);

		transparent = true;

		textureA = 0;
		textureB = 0;
		textureC = 0;

        renderer_state_set.setRenderState(iRenderState::Blend, iRenderStateValue::On);
        renderer_state_set.setRenderState(iRenderState::CullFace, iRenderStateValue::On);
        renderer_state_set.setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
		renderer_state_set.setRenderState(iRenderState::Texture2D1, iRenderStateValue::On);
		renderer_state_set.setRenderState(iRenderState::Texture2D2, iRenderStateValue::On);
        renderer_state_set.setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
        renderer_state_set.setRenderState(iRenderState::BlendFuncSource, iRenderStateValue::SourceAlpha);
        renderer_state_set.setRenderState(iRenderState::BlendFuncDestination, iRenderStateValue::OneMinusSourceAlpha);
	}

	iParticleSystemNode::~iParticleSystemNode()
	{
        textureA = 0;
		textureB = 0;
		textureC = 0;
	}

	bool iParticleSystemNode::getLoopAbility()
	{
		return loopable;
	}

	float32 iParticleSystemNode::getPhaseShift0()
	{
		return phaseshift_r1;
	}

	float32 iParticleSystemNode::getPhaseShift1()
	{
		return phaseshift_r2;
	}

	float32 iParticleSystemNode::getLiftMin()
	{
		return minlift;
	}

	float32 iParticleSystemNode::getLiftMax()
	{
		return maxlift;
	}

	float32 iParticleSystemNode::getLiftDecrease()
	{
		return liftdecrease;
	}

	float32 iParticleSystemNode::getWeightMin()
	{
		return minweight;
	}

	float32 iParticleSystemNode::getWeightMax()
	{
		return maxweight;
	}

	float32 iParticleSystemNode::getSizeMin()
	{
		return minsize;
	}

	float32 iParticleSystemNode::getSizeMax()
	{
		return maxsize;
	}

	float32 iParticleSystemNode::getSizeIncrease()
	{
		return sizeincrease;
	}

	uint32 iParticleSystemNode::getParticleCount()
	{
		return particlecount;
	}

	shared_ptr<iTexture> iParticleSystemNode::getTextureA()
	{
		return textureA;
	}

	shared_ptr<iTexture> iParticleSystemNode::getTextureB()
	{
		return textureB;
	}

	shared_ptr<iTexture> iParticleSystemNode::getTextureC()
	{
		return textureC;
	}

	float32 iParticleSystemNode::getVorticityConfinement()
	{
		return vorticityconfinement;
	}

	uint32 iParticleSystemNode::getVortexParticleCount()
	{
		return vortexcount;
	}

	float32 iParticleSystemNode::getVortexRotationSpeedMin()
	{
		return minvrot;
	}

	float32 iParticleSystemNode::getVortexRotationSpeedMax()
	{
		return maxvrot;
	}

	float32 iParticleSystemNode::getVortexRangeMin()
	{
		return minvrange;
	}

	float32 iParticleSystemNode::getVortexRangeMax()
	{
		return maxvrange;
	}

	uint32 iParticleSystemNode::getVortexCheckRange()
	{
		return vortexcheckrange;
	}

	void iParticleSystemNode::activateRainbow(bool b)
	{
		singlecolor = !b;
	}

	iRenderStateSet* iParticleSystemNode::getRendererState()
	{
		return &renderer_state_set;
	}

	void iParticleSystemNode::setPhaseShift(float32 r1, float32 r2)
	{
		phaseshift_r1 = r1;
		phaseshift_r2 = r2;
	}

	void iParticleSystemNode::setVortexCheckRange(uint32 particles)
	{
		vortexcheckrange = particles;
	}

	void iParticleSystemNode::setSizeIncrease(float32 si)
	{
		sizeincrease = si;
		mustreset = true;
	}

	void iParticleSystemNode::setLoopAbility(bool loop)
	{
		loopable = loop;
		mustreset = true;
	}

	void iParticleSystemNode::setParticleCount(uint32 count)
	{
		particlecount = count;
		mustreset = true;
	}

	void iParticleSystemNode::setParticleLifeTime(uint32 frames)
	{
		lifetime = frames;
		lifetimedecrease = 1.0f / lifetime;
		mustreset = true;
	}

	void iParticleSystemNode::setTextureA(iaString texture)
	{
		textureA = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	void iParticleSystemNode::setTextureB(iaString texture)
	{
		textureB = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	void iParticleSystemNode::setTextureC(iaString texture)
	{
		textureC = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	uint32 iParticleSystemNode::getParticleLifeTime()
	{
		return lifetime;
	}

	void iParticleSystemNode::setLiftDecrease(float32 ld)
	{
		liftdecrease = ld;
		mustreset = true;
	}

	void iParticleSystemNode::setVortexParticleCount(uint32 count)
	{
		vortexcount = count;
		mustreset = true;
	}

	void iParticleSystemNode::reset()
	{
		if(emitter==0)
		{
			con_err("no emitters defined" << endl;
			return;
		}

		if(vortexcount > particlecount)
		{
			con_err("more vortex particles than normal particles defined" << endl;
			return;
		}

		if(lifetime == 0)
		{
			con_err("lifetime 0 defined" << endl;
			return;
		}

		initframe = lifetime;

		calcBirth();
		mustreset = false;
	}

	void iParticleSystemNode::setLift(float32 min, float32 max)
	{
		minlift = min;
		maxlift = max;
		mustreset = true;
	}

	void iParticleSystemNode::setWeight(float32 min, float32 max)
	{
		minweight = min;
		maxweight = max;
		mustreset = true;
	}

	void iParticleSystemNode::setSize(float32 min, float32 max)
	{
		minsize = min;
		maxsize = max;
		mustreset = true;
	}

	void iParticleSystemNode::setVortexRotationSpeed(float32 min, float32 max)
	{
		minvrot = min;
		maxvrot = max;
		mustreset = true;
	}

	void iParticleSystemNode::setVortexRange(float32 min, float32 max)
	{
		minvrange = min;
		maxvrange = max;
		mustreset = true;
	}

	void iParticleSystemNode::resetParticle(iParticle *particle, iaMatrixf &modelview)
	{
		if(!emitter) return;

		emitter->calcRandomStart(particle, modelview);

		particle->lift = minlift + (rand()%100/100.0f) * (maxlift-minlift);
		particle->weight = minweight + (rand()%100/100.0f) * (maxweight-minweight);
		particle->size = minsize + (rand()%100/100.0f) * (maxsize-minsize);

		particle->lifetime = 1.0f;
		particle->visibletime = 1.0f;
		particle->visibletimedecrease = (1.0f / lifetime) * (1+rand()%200/100.0f);

		particle->phase0.set(rand()%100/100.0f,rand()%100/100.0f);
		particle->phase1.set(rand()%100/100.0f,rand()%100/100.0f);
	}

	void iParticleSystemNode::calcBirth()
	{
		iParticle *particle;
		iVortexParticle *vortexparticle;

		iaMatrixf modelview;

		particlesbirth.clear();
		particles.clear();

		for(uint32 i=0;i<particlecount;++i)
		{
			if((i%(long)((float32)particlecount/(float32)vortexcount)) == 0)
			{
				vortexparticle = new iVortexParticle();

				vortexparticle->force = minvrot + (rand()%100/100.0f) * (maxvrot-minvrot);
				vortexparticle->forcerange = minvrange + (rand()%100/100.0f) * (maxvrange-minvrange);
				vortexparticle->vortexnormal.set(rand()%100/100.0f-0.5f,rand()%100/100.0f-0.5f,rand()%100/100.0f-0.5f);
				vortexparticle->vortexnormal.normalize();
				vortexparticle->imune = true;
				vortexparticle->particleid = i;

				vortexparticles.push_back(vortexparticle);
				particle = (iParticle*)vortexparticle;
			}
			else
			{
				particle = new iParticle();
				particle->imune = false;
			}

			resetParticle(particle, modelview);
			particle->visible = true;
			particlesbirth.push_back(*particle);
			particle->visible = false;
			particles.push_back(particle);
		}
	}

	vector<iParticle*> iParticleSystemNode::getCurrentFrame()
	{
		if(mustreset)
		{
			reset();
		}
		return particles;
	}

	vector<iVortexParticle*> iParticleSystemNode::getCurrentFrameVortex()
	{
		if(mustreset)
		{
			reset();
		}
		return vortexparticles;
	}

	void iParticleSystemNode::calcNextFrame()
	{
		iaMatrixf modelview;
		calcNextFrame(modelview);
	}

	void iParticleSystemNode::calcNextFrame(iaMatrixf &modelview)
	{
		if(mustreset)
		{
			reset();
			if(mustreset) return;
		}

		uint32 simulate = particles.size();
		uint32 simulatevortex = vortexparticles.size();

		if(initframe > 0)
		{
			initframe--;
			simulate = (uint32)((float32)particles.size() / (float32)lifetime * ((float32)lifetime-(float32)initframe));
			simulatevortex = (uint32)((float32)vortexparticles.size() / (float32)lifetime * ((float32)lifetime-(float32)initframe));
		}

		iaVector3f a,b;

		uint32 start,end;
		float32 vortexforce;

		for(uint32 j=0;j<simulatevortex;++j)
		{
			if(vortexparticles[j]->lifetime<0.1f)
			{
				vortexforce = vortexparticles[j]->force * vortexparticles[j]->lifetime * 10.0f;
			}
			else if(vortexparticles[j]->lifetime > 0.9f)
			{
				vortexforce = vortexparticles[j]->force * (1-vortexparticles[j]->lifetime) * 10.0f;
			}
			else
				vortexforce = vortexparticles[j]->force;

			start = vortexparticles[j]->particleid - vortexcheckrange;
			if(start>0) start = 0;

			end = vortexparticles[j]->particleid + vortexcheckrange;
			if(end>simulate) end=simulate;

			for(uint32 i=start;i<end;++i)
			{
				if(particles[i]->imune) continue;

				a = vortexparticles[j]->position - particles[i]->position;
				if(a.length()>vortexparticles[j]->forcerange) continue;

				b = a % vortexparticles[j]->vortexnormal;
				b.normalize();
				b *= (vortexparticles[j]->forcerange - a.length())/vortexparticles[j]->forcerange;
				b *= vortexforce;
				a.normalize();
				a*=vorticityconfinement;
				b+=a;

				particles[i]->position += b * 0.1f;
			}
		}

		iaVector3f gravity(0,-1,0);

		for(uint32 i=0;i<simulate;++i)
		{
			particles[i]->velocity[1] += particles[i]->lift;
			particles[i]->velocity[1] -= particles[i]->weight;

			particles[i]->position += particles[i]->velocity;

			if(particles[i]->visible == false)
			{
				particles[i]->position = modelview * particles[i]->position;
				particles[i]->visible = true;
			}

			particles[i]->lift -= liftdecrease;
			particles[i]->size += sizeincrease;

			particles[i]->visibletime -= particles[i]->visibletimedecrease;
			if(particles[i]->visibletime<=0.0f) particles[i]->visible = false;

			particles[i]->lifetime -= lifetimedecrease;
			if(particles[i]->lifetime <= 0.0f)
			{
				if(loopable)
				{
					particles[i]->position = particlesbirth[i].position;
					particles[i]->velocity = particlesbirth[i].velocity;

					particles[i]->lift = particlesbirth[i].lift;
					particles[i]->weight = particlesbirth[i].weight;
					particles[i]->lifetime = particlesbirth[i].lifetime;
					particles[i]->size = particlesbirth[i].size;

					particles[i]->visible = particlesbirth[i].visible;
					particles[i]->visibletime = particlesbirth[i].visibletime;

					particles[i]->imune = particlesbirth[i].imune;

					particles[i]->phase0 = particlesbirth[i].phase0;
					particles[i]->phase1 = particlesbirth[i].phase1;
				}
				else
				{
					resetParticle(particles[i], modelview);
				}
			}
		}

		for(uint32 i=0;i<simulate;++i)
		{
			particles[i]->phase0.rotateXY(phaseshift_r1);
			particles[i]->phase1.rotateXY(phaseshift_r2);
		}
	}

	void iParticleSystemNode::setVorticityConfinement(float32 vc)
	{
		vorticityconfinement = vc;
		mustreset = true;
	}

	void iParticleSystemNode::activateEmitter(bool active)
	{
		activeemitter = active;
	}

	bool iParticleSystemNode::emitterIsActive()
	{
		return activeemitter;
	}

	void iParticleSystemNode::setEmitter(iParticleEmitterNode *emitter)
	{
		this->emitter = emitter;
		mustreset = true;
	}

	void iParticleSystemNode::setColor(iaColor4f &color)
	{
		this->color = color;
	}

    iRainbow& iParticleSystemNode::getRainbow()
	{
		return _rainbow;
	}

	void iParticleSystemNode::preRenderCustom()
	{

	}

	void iParticleSystemNode::doRenderCustom()
	{
		if(!emitter) return;
		iaMatrixf temp = iRenderer::getInstance().getModelViewMatrix();
		temp.invert();
		temp = emitter->modelviewmatrix * temp;
		calcNextFrame(temp);
	}

	void iParticleSystemNode::transRenderCustom()
	{
		iRenderer::getInstance().setRenderStateSet(renderer_state_set);

//! \todo		if(textureA) textureA->bind(0);
	//	if(textureB) textureB->bind(1);
		//if(textureC) textureC->bind(2); 

		if(singlecolor)
		{
			iRenderer::getInstance().setColor(color);
			iRenderer::getInstance().setRainbow(0);
		}
		else
		{
			iRenderer::getInstance().setColor(iaColor4f(1,1,1,1));
			iRenderer::getInstance().setRainbow(&_rainbow);
		}

		iRenderer::getInstance().drawParticles(&particles, iRenderer::getInstance().getCamMatrix());
	}

	// iParticleEmitterNode

	iParticleEmitterNode::iParticleEmitterNode()
	{
	}

	iParticleEmitterNode::~iParticleEmitterNode()
	{
	}
    
	void iParticleEmitterNode::addTriangleEmitter(iTriangleEmitter emitter)
	{
		triangleemitters.push_back(emitter);
	}

	void iParticleEmitterNode::calcRandomStart(iParticle *particle, iaMatrixf &modelview)
	{
		if(triangleemitters.size()==0) return;
		iTriangleEmitter &emitter = triangleemitters[rand()%triangleemitters.size()];

		float32 a,b;

		do
		{
			a = rand()%100/100.0f;
			b = rand()%100/100.0f;
		}
		while(a+b>1.0f);

		iaVector3f ab = emitter.pos[1] - emitter.pos[0];
		iaVector3f ac = emitter.pos[2] - emitter.pos[0];

		particle->position = (ab*a) + (ac*b) + emitter.pos[0];
		particle->position = modelview * particle->position;

		ab = emitter.vel[0]*(1.0f-a) + emitter.vel[1]*a;
		ac = emitter.vel[2]*(1.0f-a) + emitter.vel[1]*a;

		particle->velocity = ab*(1.0f-b) + ac*b;
	}

	void iParticleEmitterNode::preRenderCustom()
	{
	}

	void iParticleEmitterNode::doRenderCustom()
	{
        modelviewmatrix = iRenderer::getInstance().getModelViewMatrix();
	}

	void iParticleEmitterNode::transRenderCustom()
	{
	}

};
