// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

Supremacy::Supremacy(iWindow *window)
    : iLayer(window, L"Supremacy"), _viewOrtho(iView(false)), _quadtree(iRectangled(0, 0, 1000, 1000))
{
}

Supremacy::~Supremacy()
{
}

void Supremacy::onInit()
{
    _viewOrtho.setClearColor(0.55, 0.0, 0.0, 1.0);
    _viewOrtho.setName("view ortho");
    _viewOrtho.setClearColor(true);
    _viewOrtho.setClearDepth(true);
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &Supremacy::onRenderOrtho));
    getWindow()->addView(&_viewOrtho, getZIndex() + 1);

    _materialWithTextureAndBlending = iMaterialResourceFactory::getInstance().createMaterial("TextureAndBlending");
    auto material = iMaterialResourceFactory::getInstance().getMaterial(_materialWithTextureAndBlending);
    material->setRenderState(iRenderState::Texture2D0, iRenderStateValue::On);
    material->setRenderState(iRenderState::Blend, iRenderStateValue::On);
    material->setRenderState(iRenderState::DepthTest, iRenderStateValue::Off);

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(getWindow()));

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &Supremacy::onUpdate), iaTime::fromMilliseconds(10));
    _updateTimerHandle->start();

    _rand.setSeed(1337);

    // setup ECS
    _logicSystems.addSystem(_pawnSystem);
    _renderSystems.addSystem(_displayEntittiesSystem);

    PositionComponent position;
    VelocityComponent velocity;
    HealthComponent health;
    PartyComponent party;
    VisualComponent visual;

    // create some enemies
    for (int i = 0; i < 100; ++i)
    {
        position._position.set(_rand.getNextFloat() * 1000.0, _rand.getNextFloat() * 1000.0);
        velocity._direction.set(0.0, 1.0);
        velocity._direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);
        velocity._speed = _rand.getNextFloat() * 0.1 + 0.1;
        health._health = 100;
        party._partyID = 10;
        visual._character = iTextureResourceFactory::getInstance().requestFile("particleGem.png");

        _ecs.createEntity(position, velocity, health, party, visual);
    }

    // init player
    MovementControlComponent movementControl;
    position._position.set(500.0, 500.0);
    velocity._speed = 1.0;
    health._health = 100;
    party._partyID = 20;
    visual._character = iTextureResourceFactory::getInstance().requestFile("particleStar.png");

    _player = _ecs.createEntity(position, velocity, health, party, visual, movementControl);
}

void Supremacy::onUpdate()
{
    _ecs.updateSystems(_logicSystems);
}

void Supremacy::onDeinit()
{
    // stop timer
    if (_updateTimerHandle != nullptr)
    {
        delete _updateTimerHandle;
        _updateTimerHandle = nullptr;
    }

    // clean up window
    getWindow()->removeView(&_viewOrtho);
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &Supremacy::onRenderOrtho));
}

void Supremacy::onPreDraw()
{
}

void Supremacy::onEvent(iEvent &event)
{
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onKeyUp));
}

static void renderTree(const std::unique_ptr<iQuadtreeNode> &node)
{
    if (node == nullptr)
    {
        return;
    }

    iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
    iRenderer::getInstance().drawLine(node->_box._x, node->_box._y, node->_box._x + node->_box._width, node->_box._y);
    iRenderer::getInstance().drawLine(node->_box._x, node->_box._y + node->_box._height, node->_box._x + node->_box._width, node->_box._y + node->_box._height);
    iRenderer::getInstance().drawLine(node->_box._x, node->_box._y, node->_box._x, node->_box._y + node->_box._height);
    iRenderer::getInstance().drawLine(node->_box._x + node->_box._width, node->_box._y, node->_box._x + node->_box._width, node->_box._y + node->_box._height);

    for (const auto &data : node->_userData)
    {
        iRenderer::getInstance().setColor(1.0, 1.0, 0.0, 1.0);
        iRenderer::getInstance().drawRectangle(data._pos._x - 5.0, data._pos._y- 5.0, 10, 10);
    }

    for (const auto &node : node->_children)
    {
        renderTree(node);
    }
}

void Supremacy::onRenderOrtho()
{
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    // draw entities
 //   iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
   // _ecs.updateSystems(_renderSystems);

    iRenderer::getInstance().setMaterial(iMaterialResourceFactory::getInstance().getDefaultMaterial());
    renderTree(_quadtree.getRoot());
}

bool Supremacy::onKeyDown(iEventKeyDown &event)
{
    MovementControlComponent *movementControl = _ecs.getComponent<MovementControlComponent>(_player);
    if (movementControl == nullptr)
    {
        return false;
    }

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl->_up = true;
        return true;

    case iKeyCode::A:
        movementControl->_left = true;
        return true;

    case iKeyCode::S:
        movementControl->_down = true;
        return true;

    case iKeyCode::D:
        movementControl->_right = true;
        return true;

    case iKeyCode::I:
        _quadtree.insert(nullptr, iaVector2d(_rand.getNextFloat() * 1000.0, _rand.getNextFloat() * 1000.0));
        return true;

    case iKeyCode::J:
        return true;

    case iKeyCode::K:
        return true;

    case iKeyCode::L:
        return true;

    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    return false;
}

bool Supremacy::onKeyUp(iEventKeyUp &event)
{
    MovementControlComponent *movementControl = _ecs.getComponent<MovementControlComponent>(_player);
    if (movementControl == nullptr)
    {
        return false;
    }

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl->_up = false;
        return true;

    case iKeyCode::A:
        movementControl->_left = false;
        return true;

    case iKeyCode::S:
        movementControl->_down = false;
        return true;

    case iKeyCode::D:
        movementControl->_right = false;
        return true;

    case iKeyCode::I:
        return true;

    case iKeyCode::J:
        return true;

    case iKeyCode::K:
        return true;

    case iKeyCode::L:
        return true;
    }

    return false;
}