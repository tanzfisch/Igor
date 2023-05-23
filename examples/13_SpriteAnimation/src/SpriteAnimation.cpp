// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include "SpriteAnimation.h"

#include "TileMapGenerator.h"

#include <sstream>
#include <map>

// notes
// https://www.youtube.com/watch?v=tFsETEP01k8
// https://www.youtube.com/watch?v=B0enS9BJne4

SpriteAnimation::SpriteAnimation(iWindowPtr window)
    : ExampleBase(window, "Sprite Animation")
{
}

void SpriteAnimation::onInit()
{
    getView().setClearColor(0.0, 1.0, 1.0, 1.0);
    getViewOrtho().setScene(getScene());

    _materialTerrain = iMaterialResourceFactory::getInstance().loadMaterial("examples/sprite_animation_textured.mat");

    // load atlantes
    _walk = iSprite::create("atlases/SpriteAnimationWalk.atlas");
    _tiles = iSprite::create("atlases/SpriteAnimationTiles.atlas");

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

    // initialize animation timer
    _animationTimer.setIntervall(iaTime::fromMilliseconds(200));
    _animationTimer.registerTimerDelegate(iTimerTickDelegate(this, &SpriteAnimation::onUpdate));
    _animationTimer.start();
}

void SpriteAnimation::onDeinit()
{
    // release materials (optional)
    _materialTerrain = nullptr;

    // release some resources
    _walk = nullptr;
    _tiles = nullptr;
}

void SpriteAnimation::onEvent(iEvent &event)
{
    // first call example base
    ExampleBase::onEvent(event);

    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(SpriteAnimation::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(SpriteAnimation::onKeyUp));
}

bool SpriteAnimation::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Left:
    case iKeyCode::A:
        _flags[0] = true;
        return true;

    case iKeyCode::Up:
    case iKeyCode::W:
        _flags[1] = true;
        return true;

    case iKeyCode::Right:
    case iKeyCode::D:
        _flags[2] = true;
        return true;

    case iKeyCode::Down:
    case iKeyCode::S:
        _flags[3] = true;
        return true;

    case iKeyCode::LShift:
        _flags[4] = true;
        return true;
    }

    return false;
}

bool SpriteAnimation::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Left:
    case iKeyCode::A:
        _flags[0] = false;
        return true;

    case iKeyCode::Up:
    case iKeyCode::W:
        _flags[1] = false;
        return true;

    case iKeyCode::Right:
    case iKeyCode::D:
        _flags[2] = false;
        return true;

    case iKeyCode::Down:
    case iKeyCode::S:
        _flags[3] = false;
        return true;

    case iKeyCode::LShift:
        _flags[4] = false;
        return true;
    }

    return false;
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

void SpriteAnimation::onUpdate()
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

void SpriteAnimation::onUpdate(const iaTime &time)
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
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    iaMatrixf walkMatrix;
    walkMatrix.translate(getWindow()->getClientWidth() * 0.5, getWindow()->getClientHeight() * 0.5, 0.0);
    walkMatrix.scale(70.0,70.0,1.0);

    iRenderer::getInstance().drawSprite(walkMatrix, _walk, _animationOffset + _animationIndex, iaColor4f::white, true);

    ExampleBase::onRenderOrtho();
}
