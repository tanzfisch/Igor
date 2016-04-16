// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iParticleSystem3D.h>
#include <iaConsole.h>
#include <iRenderer.h>
#include <iTextureResourceFactory.h>
#include <iMaterialResourceFactory.h>
#include <iRainbow.h>

#include <iostream>
using namespace std;

namespace Igor
{

	iParticleSystem3D::iParticleSystem3D()
	{
		_lifeTimeStep = 1.0f / _lifeTime;
		_particleCount = 1000;
		_vortexCount = 10;

		_minLift = 0.002f;
		_maxLift = 0.004f;
        
		_reduceLiftStep = 0.000005f;

		_minWeight = 0.001f;
		_maxWeight = 0.002f;

		_minSize = 10.0f;
		_maxSize = 20.0f;

		_sizeIncreaseStep = 0.1f;

		_minVRot = 1.0f;
		_maxVRot = 2.0f;

		_minVRange = 30.0f;
		_maxVRange = 60.0f;

		_vortexCheckRange = 30;

		_vorticityConfinement = 0.1f;

		_loopable = false;
		_phaseShiftR1 = 0.0;
		_phaseShiftR2 = 0.0;

		_singleColor = true;
		_color.set(1,1,1,1);
	}

	iParticleSystem3D::~iParticleSystem3D()
	{
	}

	bool iParticleSystem3D::getLoopAbility()
	{
		return _loopable;
	}

	float32 iParticleSystem3D::getPhaseShift0()
	{
		return _phaseShiftR1;
	}

	float32 iParticleSystem3D::getPhaseShift1()
	{
		return _phaseShiftR2;
	}

	float32 iParticleSystem3D::getLiftMin()
	{
		return _minLift;
	}

	float32 iParticleSystem3D::getLiftMax()
	{
		return _maxLift;
	}

	float32 iParticleSystem3D::getLiftDecrease()
	{
		return _reduceLiftStep;
	}

	float32 iParticleSystem3D::getWeightMin()
	{
		return _minWeight;
	}

	float32 iParticleSystem3D::getWeightMax()
	{
		return _maxWeight;
	}

	float32 iParticleSystem3D::getSizeMin()
	{
		return _minSize;
	}

	float32 iParticleSystem3D::getSizeMax()
	{
		return _maxSize;
	}

	float32 iParticleSystem3D::getSizeIncrease()
	{
		return _sizeIncreaseStep;
	}

	uint32 iParticleSystem3D::getParticleCount()
	{
		return _particleCount;
	}

	shared_ptr<iTexture> iParticleSystem3D::getTextureA()
	{
		return _textureA;
	}

	shared_ptr<iTexture> iParticleSystem3D::getTextureB()
	{
		return _textureB;
	}

	shared_ptr<iTexture> iParticleSystem3D::getTextureC()
	{
		return _textureC;
	}

	float32 iParticleSystem3D::getVorticityConfinement()
	{
		return _vorticityConfinement;
	}

	uint32 iParticleSystem3D::getVortexParticleCount()
	{
		return _vortexCount;
	}

	float32 iParticleSystem3D::getVortexRotationSpeedMin()
	{
		return _minVRot;
	}

	float32 iParticleSystem3D::getVortexRotationSpeedMax()
	{
		return _maxVRot;
	}

	float32 iParticleSystem3D::getVortexRangeMin()
	{
		return _minVRange;
	}

	float32 iParticleSystem3D::getVortexRangeMax()
	{
		return _maxVRange;
	}

	uint32 iParticleSystem3D::getVortexCheckRange()
	{
		return _vortexCheckRange;
	}

	void iParticleSystem3D::activateRainbow(bool b)
	{
		_singleColor = !b;
	}

	void iParticleSystem3D::setPhaseShift(float32 r1, float32 r2)
	{
		_phaseShiftR1 = r1;
		_phaseShiftR2 = r2;
	}

	void iParticleSystem3D::setVortexCheckRange(uint32 _particles)
	{
		_vortexCheckRange = _particles;
	}

	void iParticleSystem3D::setSizeIncrease(float32 si)
	{
		_sizeIncreaseStep = si;
		_mustReset = true;
	}

	void iParticleSystem3D::setLoopAbility(bool loop)
	{
		_loopable = loop;
		_mustReset = true;
	}

	void iParticleSystem3D::setParticleCount(uint32 count)
	{
		_particleCount = count;
		_mustReset = true;
	}

	void iParticleSystem3D::setParticleLifeTime(uint32 frames)
	{
		_lifeTime = frames;
		_lifeTimeStep = 1.0f / _lifeTime;
		_mustReset = true;
	}

	void iParticleSystem3D::setTextureA(iaString texture)
	{
		_textureA = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	void iParticleSystem3D::setTextureB(iaString texture)
	{
		_textureB = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	void iParticleSystem3D::setTextureC(iaString texture)
	{
		_textureC = iTextureResourceFactory::getInstance().requestFile(texture);
	}

	uint32 iParticleSystem3D::getParticleLifeTime()
	{
		return _lifeTime;
	}

	void iParticleSystem3D::setLiftDecrease(float32 ld)
	{
		_reduceLiftStep = ld;
		_mustReset = true;
	}

	void iParticleSystem3D::setVortexParticleCount(uint32 count)
	{
		_vortexCount = count;
		_mustReset = true;
	}

	void iParticleSystem3D::reset()
	{
		if(_vortexCount > _particleCount)
		{
			con_err("more vortex _particles than normal _particles defined");
			return;
		}

		if(_lifeTime == 0)
		{
			con_err("_lifeTime 0 defined");
			return;
		}

		_initFrame = _lifeTime;

		calcBirth();
		_mustReset = false;
	}

	void iParticleSystem3D::setLift(float32 min, float32 max)
	{
		_minLift = min;
		_maxLift = max;
		_mustReset = true;
	}

	void iParticleSystem3D::setWeight(float32 min, float32 max)
	{
		_minWeight = min;
		_maxWeight = max;
		_mustReset = true;
	}

	void iParticleSystem3D::setSize(float32 min, float32 max)
	{
		_minSize = min;
		_maxSize = max;
		_mustReset = true;
	}

	void iParticleSystem3D::setVortexRotationSpeed(float32 min, float32 max)
	{
		_minVRot = min;
		_maxVRot = max;
		_mustReset = true;
	}

	void iParticleSystem3D::setVortexRange(float32 min, float32 max)
	{
		_minVRange = min;
		_maxVRange = max;
		_mustReset = true;
	}

	void iParticleSystem3D::resetParticle(iParticle *particle, iaMatrixf &modelview)
	{
		/*if(!emitter) return;

		emitter->calcRandomStart(particle, modelview);
        */
		particle->_lift = _minLift + (rand()%100/100.0f) * (_maxLift-_minLift);
		particle->_weight = _minWeight + (rand()%100/100.0f) * (_maxWeight-_minWeight);
		particle->_size = _minSize + (rand()%100/100.0f) * (_maxSize-_minSize);

		particle->_lifeTime = 1.0f;
		particle->_visibleTime = 1.0f;
		particle->_visibleTimeStep = (1.0f / _lifeTime) * (1+rand()%200/100.0f);

		particle->_phase0.set(rand()%100/100.0f,rand()%100/100.0f);
		particle->_phase1.set(rand()%100/100.0f,rand()%100/100.0f);
	}

	void iParticleSystem3D::calcBirth()
	{
		iParticle *particle;
		iVortexParticle *vortexparticle;

		iaMatrixf modelview;

		_particlesBirth.clear();
		_particles.clear();

		for(uint32 i=0;i<_particleCount;++i)
		{
			if((i%(long)((float32)_particleCount/(float32)_vortexCount)) == 0)
			{
				vortexparticle = new iVortexParticle();

				vortexparticle->_force = _minVRot + (rand()%100/100.0f) * (_maxVRot-_minVRot);
				vortexparticle->_forceRange = _minVRange + (rand()%100/100.0f) * (_maxVRange-_minVRange);
				vortexparticle->_vortexNormal.set(rand()%100/100.0f-0.5f,rand()%100/100.0f-0.5f,rand()%100/100.0f-0.5f);
				vortexparticle->_vortexNormal.normalize();
				vortexparticle->_imune = true;
				vortexparticle->_particleid = i;

				_vortexParticles.push_back(vortexparticle);
				particle = (iParticle*)vortexparticle;
			}
			else
			{
				particle = new iParticle();
				particle->_imune = false;
			}

			resetParticle(particle, modelview);
			particle->_visible = true;
			_particlesBirth.push_back(*particle);
			particle->_visible = false;
			_particles.push_back(particle);
		}
	}

	vector<iParticle*> iParticleSystem3D::getCurrentFrame()
	{
		if(_mustReset)
		{
			reset();
		}
		return _particles;
	}

	vector<iVortexParticle*> iParticleSystem3D::getCurrentFrameVortex()
	{
		if(_mustReset)
		{
			reset();
		}
		return _vortexParticles;
	}

	void iParticleSystem3D::calcNextFrame()
	{
		iaMatrixf modelview;
		calcNextFrame(modelview);
	}

	void iParticleSystem3D::calcNextFrame(iaMatrixf &modelview)
	{
		if(_mustReset)
		{
			reset();
			if(_mustReset) return;
		}

		uint32 simulate = _particles.size();
		uint32 simulatevortex = _vortexParticles.size();

		if(_initFrame > 0)
		{
			_initFrame--;
			simulate = (uint32)((float32)_particles.size() / (float32)_lifeTime * ((float32)_lifeTime-(float32)_initFrame));
			simulatevortex = (uint32)((float32)_vortexParticles.size() / (float32)_lifeTime * ((float32)_lifeTime-(float32)_initFrame));
		}

		iaVector3f a,b;

		uint32 start,end;
		float32 vortexforce;

		for(uint32 j=0;j<simulatevortex;++j)
		{
			if(_vortexParticles[j]->_lifeTime<0.1f)
			{
				vortexforce = _vortexParticles[j]->_force * _vortexParticles[j]->_lifeTime * 10.0f;
			}
			else if(_vortexParticles[j]->_lifeTime > 0.9f)
			{
				vortexforce = _vortexParticles[j]->_force * (1-_vortexParticles[j]->_lifeTime) * 10.0f;
			}
			else
				vortexforce = _vortexParticles[j]->_force;

			start = _vortexParticles[j]->_particleid - _vortexCheckRange;
			if(start>0) start = 0;

			end = _vortexParticles[j]->_particleid + _vortexCheckRange;
			if(end>simulate) end=simulate;

			for(uint32 i=start;i<end;++i)
			{
				if(_particles[i]->_imune) continue;

				a = _vortexParticles[j]->_position - _particles[i]->_position;
				if(a.length()>_vortexParticles[j]->_forceRange) continue;

				b = a % _vortexParticles[j]->_vortexNormal;
				b.normalize();
				b *= (_vortexParticles[j]->_forceRange - a.length())/_vortexParticles[j]->_forceRange;
				b *= vortexforce;
				a.normalize();
				a*=_vorticityConfinement;
				b+=a;

				_particles[i]->_position += b * 0.1f;
			}
		}

		iaVector3f gravity(0,-1,0);

		for(uint32 i=0;i<simulate;++i)
		{
			_particles[i]->_velocity[1] += _particles[i]->_lift;
			_particles[i]->_velocity[1] -= _particles[i]->_weight;

			_particles[i]->_position += _particles[i]->_velocity;

			if(_particles[i]->_visible == false)
			{
				_particles[i]->_position = modelview * _particles[i]->_position;
				_particles[i]->_visible = true;
			}

			_particles[i]->_lift -= _reduceLiftStep;
			_particles[i]->_size += _sizeIncreaseStep;

			_particles[i]->_visibleTime -= _particles[i]->_visibleTimeStep;
			if(_particles[i]->_visibleTime<=0.0f) _particles[i]->_visible = false;

			if(_particles[i]->_lifeTime <= 0.0f)
			{
				if(_loopable)
				{
					_particles[i]->_position = _particlesBirth[i]._position;
					_particles[i]->_velocity = _particlesBirth[i]._velocity;

					_particles[i]->_lift = _particlesBirth[i]._lift;
					_particles[i]->_weight = _particlesBirth[i]._weight;
					_particles[i]->_lifeTime = _particlesBirth[i]._lifeTime;
					_particles[i]->_size = _particlesBirth[i]._size;

					_particles[i]->_visible = _particlesBirth[i]._visible;
					_particles[i]->_visibleTime = _particlesBirth[i]._visibleTime;

					_particles[i]->_imune = _particlesBirth[i]._imune;

					_particles[i]->_phase0 = _particlesBirth[i]._phase0;
					_particles[i]->_phase1 = _particlesBirth[i]._phase1;
				}
				else
				{
					resetParticle(_particles[i], modelview);
				}
			}
		}

		for(uint32 i=0;i<simulate;++i)
		{
			_particles[i]->_phase0.rotateXY(_phaseShiftR1);
			_particles[i]->_phase1.rotateXY(_phaseShiftR2);
		}
	}

	void iParticleSystem3D::setVorticityConfinement(float32 vc)
	{
		_vorticityConfinement = vc;
		_mustReset = true;
	}

	void iParticleSystem3D::setColor(iaColor4f &_color)
	{
		this->_color = _color;
	}

    iRainbow& iParticleSystem3D::getRainbow()
	{
		return _rainbow;
	}

/*		if(!emitter) return;
		iaMatrixf temp = iRenderer::getInstance().getModelViewMatrix();
		temp.invert();
		temp = emitter->modelviewmatrix * temp;
		calcNextFrame(temp);*/
    
        //    iRenderer::getInstance().setRenderStateSet(renderer_state_set);

        //! \todo		if(textureA) textureA->bind(0);
            //	if(textureB) textureB->bind(1);
                //if(textureC) textureC->bind(2); 

      /*  if (_singleColor)
        {
            iRenderer::getInstance().setColor(_color);
            iRenderer::getInstance().setRainbow(0);
        }
        else
        {
            iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));
            iRenderer::getInstance().setRainbow(&_rainbow);
        }

        iRenderer::getInstance().drawParticles(&_particles, iRenderer::getInstance().getCamMatrix());*/

};
