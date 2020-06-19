// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "SpriteAnimation.h"

#include <iaux/system/iaConsole.h>
#include <iaux/data/iaString.h>
using namespace iaux;

#include <igor/system/iMouse.h>
#include <igor/system/iKeyboard.h>
#include <igor/resources/texture/iAtlas.h>
#include <igor/resources/texture/iTextureFont.h>
#include <igor/system/iTimer.h>
#include <igor/system/iApplication.h>
#include <igor/graphics/iRenderer.h>
#include <igor/resources/texture/iTextureResourceFactory.h>
#include <igor/resources/material/iMaterial.h>
#include <igor/resources/material/iMaterialResourceFactory.h>
#include <igor/resources/profiler/iProfiler.h>
#include <igor/scene/iSceneFactory.h>
#include <igor/scene/nodes/iNodeMesh.h>
#include <igor/scene/nodes/iNodeManager.h>
#include <igor/resources/material/iTargetMaterial.h>
#include <igor/scene/nodes/iNodeTransform.h>
#include <igor/scene/iScene.h>
#include <igor/scene/traversal/iNodeVisitorPrintTree.h>
#include <igor/scene/nodes/iNodeCamera.h>
#include <igor/scene/nodes/iNodeModel.h>
using namespace igor;

#include <sstream>
#include <map>

#include "TileMapGenerator.h"

SpriteAnimation::SpriteAnimation()
	: ExampleBase("Sprite Animation")
{
}

void SpriteAnimation::init()
{
	getView().setClearColor(1.0, 1.0, 1.0, 1.0);
	getViewOrtho().setScene(getScene());

	_materialTerrain = iMaterialResourceFactory::getInstance().createMaterial();
	auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialTerrain);
	material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
	material->setRenderState(iRenderState::DepthMask, iRenderStateValue::Off);
	material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);
	material->addShaderSource("igor/textured.vert", iShaderObjectType::Vertex);
	material->addShaderSource("igor/textured.frag", iShaderObjectType::Fragment);
	material->compileShader();

	// load atlantes
	_walk = new iAtlas(iTextureResourceFactory::getInstance().loadFile("SpriteAnimationWalk.png", iResourceCacheMode::Free, iTextureBuildMode::Normal));
	_walk->loadFrames("../data/atlantes/SpriteAnimationWalk.xml");

	_tiles = new iAtlas(iTextureResourceFactory::getInstance().loadFile("SpriteAnimationTiles.png", iResourceCacheMode::Free, iTextureBuildMode::Normal));
	_tiles->loadFrames("../data/atlantes/SpriteAnimationTiles.xml");

	// generate ground map
	TileMapGenerator tileMapGenerator;
	tileMapGenerator.setAtlas(_tiles);
	tileMapGenerator.setMaterial(_materialTerrain);
	iNodePtr terrainNodeGround = tileMapGenerator.generateFromRandom(iaVector2i(32, 32), 0, 18);
	terrainNodeGround->setName("Ground");
	iNodeTransform *terrainGroundTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	terrainGroundTransform->translate(0, 0, 0);
	terrainGroundTransform->insertNode(terrainNodeGround);
	getScene()->getRoot()->insertNode(terrainGroundTransform);

	// generate dressing and trees map
	iNodePtr terrainNodeDressing = tileMapGenerator.generateFromTexture("SpriteAnimationTerrain.png");
	terrainNodeDressing->setName("Dressing");
	iNodeTransform *terrainDressingTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	terrainDressingTransform->translate(0, 0, 0);
	terrainDressingTransform->insertNode(terrainNodeDressing);
	getScene()->getRoot()->insertNode(terrainDressingTransform);

	// setup camera
	_cameraTransform = iNodeManager::getInstance().createNode<iNodeTransform>();
	_cameraTransform->translate(0, 0, 30);
	// anf of corse the camera
	iNodeCamera *camera = iNodeManager::getInstance().createNode<iNodeCamera>();
	_cameraTransform->insertNode(camera);
	getScene()->getRoot()->insertNode(_cameraTransform);
	getViewOrtho().setCurrentCamera(camera->getID());

	// some flags to handle the character movement TODO
	for (int i = 0; i < 5; ++i)
	{
		_flags[i] = false;
	}

	// load requested textures right now
	iTextureResourceFactory::getInstance().flush();

	// initialize animation timer
	_animationTimer.setIntervall(iaTime::fromMilliseconds(200));
	_animationTimer.registerTimerDelegate(iTimerTickDelegate(this, &SpriteAnimation::onAnimationTimerTick));
	_animationTimer.start();
}

void SpriteAnimation::deinit()
{
	// release materials (optional)
	iMaterialResourceFactory::getInstance().destroyMaterial(_materialTerrain);
	_materialTerrain = iMaterial::INVALID_MATERIAL_ID;

	// release some textures. otherwhise you will get a reminder of possible mem leak
	if (_walk != nullptr)
	{
		delete _walk;
		_walk = nullptr;
	}

	if (_tiles != nullptr)
	{
		delete _tiles;
		_tiles = nullptr;
	}
}

void SpriteAnimation::onMouseMoved(const iaVector2i &position)
{
	ExampleBase::onMouseMoved(position);
}

void SpriteAnimation::onKeyDown(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::Left:
		_flags[0] = true;
		break;

	case iKeyCode::Up:
		_flags[1] = true;
		break;

	case iKeyCode::Right:
		_flags[2] = true;
		break;

	case iKeyCode::Down:
		_flags[3] = true;
		break;

	case iKeyCode::LShift:
		_flags[4] = true;
		break;
	}

	ExampleBase::onKeyDown(key);
}

void SpriteAnimation::onKeyUp(iKeyCode key)
{
	switch (key)
	{
	case iKeyCode::Left:
		_flags[0] = false;
		break;

	case iKeyCode::Up:
		_flags[1] = false;
		break;

	case iKeyCode::Right:
		_flags[2] = false;
		break;

	case iKeyCode::Down:
		_flags[3] = false;
		break;

	case iKeyCode::LShift:
		_flags[4] = false;
		break;
	}

	ExampleBase::onKeyUp(key);
}

iaString SpriteAnimation::getCharacterStateName(CharacterState state)
{
	switch (state)
	{
	case CharacterState::WalkN:
		return "WalkN";
	case CharacterState::WalkNE:
		return "WalkNE";
	case CharacterState::WalkE:
		return "WalkE";
	case CharacterState::WalkSE:
		return "WalkSE";
	case CharacterState::WalkS:
		return "WalkS";
	case CharacterState::WalkSW:
		return "WalkSW";
	case CharacterState::WalkW:
		return "WalkW";
	case CharacterState::WalkNW:
		return "WalkNW";
	case CharacterState::RunN:
		return "RunN";
	case CharacterState::RunNE:
		return "RunNE";
	case CharacterState::RunE:
		return "RunE";
	case CharacterState::RunSE:
		return "RunSE";
	case CharacterState::RunS:
		return "RunS";
	case CharacterState::RunSW:
		return "RunSW";
	case CharacterState::RunW:
		return "RunW";
	case CharacterState::RunNW:
		return "RunNW";
	case CharacterState::Idle:
		return "Idle";
	}

	return "unknown";
}

void SpriteAnimation::onPreDraw()
{
	// moves the logo towards the mouse position

	iaVector2f velocity;

	if (_flags[0])
	{
		velocity._x -= 1;
	}
	if (_flags[1])
	{
		velocity._y -= 0.5;
	}
	if (_flags[2])
	{
		velocity._x += 1;
	}
	if (_flags[3])
	{
		velocity._y += 0.5;
	}

	if (velocity.length())
	{
		velocity.normalize();
		if (_flags[4])
		{
			velocity *= 3;
		}
	}

	_characterVelocity = velocity;
	_characterPosition += _characterVelocity;

	_cameraTransform->setPosition(_characterPosition._x, _characterPosition._y, 30);

	CharacterState oldCharacterState = _characterState;

	if (_characterVelocity.length() == 0)
	{
		_characterState = CharacterState::Idle;
		_animationOffset = 8;
	}
	else
	{
		float32 step = M_PI * 2.0 / 16.0;
		float32 dir = _characterVelocity.angle() + M_PI;
		bool run = _characterVelocity.length() > 1.1;

		if ((0 <= dir && dir < step * 1.0) ||
			(step * 15.0 <= dir && dir <= step * 16.0))
		{
			_characterState = run ? CharacterState::RunN : CharacterState::WalkN;
			_animationOffset = run ? 21 : 6;
		}
		else if (step * 1 <= dir && dir < step * 3)
		{
			_characterState = run ? CharacterState::RunNW : CharacterState::WalkNW;
			_animationOffset = run ? 21 : 6;
		}
		else if (step * 3 <= dir && dir < step * 5)
		{
			_characterState = run ? CharacterState::RunW : CharacterState::WalkW;
			_animationOffset = run ? 13 : 2;
		}
		else if (step * 5 <= dir && dir < step * 7)
		{
			_characterState = run ? CharacterState::RunSW : CharacterState::WalkSW;
			_animationOffset = run ? 9 : 0;
		}
		else if (step * 7 <= dir && dir < step * 9)
		{
			_characterState = run ? CharacterState::RunS : CharacterState::WalkS;
			_animationOffset = run ? 9 : 0;
		}
		else if (step * 9 <= dir && dir < step * 11)
		{
			_characterState = run ? CharacterState::RunSE : CharacterState::WalkSE;
			_animationOffset = run ? 9 : 0;
		}
		else if (step * 11 <= dir && dir < step * 13)
		{
			_characterState = run ? CharacterState::RunE : CharacterState::WalkE;
			_animationOffset = run ? 17 : 4;
		}
		else if (step * 13 <= dir && dir < step * 15)
		{
			_characterState = run ? CharacterState::RunNE : CharacterState::WalkNE;
			_animationOffset = run ? 21 : 6;
		}
	}

	if (oldCharacterState != _characterState)
	{
		_animationIndex = 0;
	}

	// con_endl(getCharacterStateName(_characterState));
}

void SpriteAnimation::onAnimationTimerTick()
{
	if (_characterVelocity.length() < 0.0001)
	{
		return;
	}

	bool run = _characterVelocity.length() > 1.1;
	if (run)
	{
		_animationIndex++;
		if (_animationIndex > 3)
		{
			_animationIndex = 0;
		}
	}
	else
	{
		_animationIndex++;
		if (_animationIndex > 1)
		{
			_animationIndex = 0;
		}
	}
}

void SpriteAnimation::onRenderOrtho()
{
	// since the model matrix is by default an identity matrix which would cause all our 2d rendering end up at depth zero
	// and the near clipping plane of our frustum can't be zero we have to push the scene a bit away from zero (e.g. -30 just a random number with no meaning)
	iaMatrixd matrix;
	iRenderer::getInstance().setViewMatrix(matrix);
	matrix.translate(0, 0, -30);
	iRenderer::getInstance().setModelMatrix(matrix);

	// change material again to textured an draw the character
	iRenderer::getInstance().setMaterial(getFontMaterial());
	iRenderer::getInstance().setColor(iaColor4f(1, 1, 1, 1));

	// draw walking character
	iRenderer::getInstance().drawSprite(_walk, _animationOffset + _animationIndex, iaVector2f(getWindow().getClientWidth() * 0.5, getWindow().getClientHeight() * 0.5));

	ExampleBase::onRenderOrtho();
}
