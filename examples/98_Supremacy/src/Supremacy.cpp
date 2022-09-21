// Igor game engine
// (c) Copyright 2012-2020 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

Supremacy::Supremacy(iWindow *window)
    : iLayer(window, L"Supremacy"), _viewOrtho(iView(false)), _quadtree(iaRectangled(0, 0, 1000, 1000))
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

    _plainMaterial = iMaterialResourceFactory::getInstance().createMaterial("PlainMaterial");

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(getWindow()));

    _rand.setSeed(1337);

    // setup ECS
    _entityScene.registerSystem(iEntitySystemPtr(new PawnSystem(&_quadtree)));
    _entityScene.registerSystem(iEntitySystemPtr(new DisplayEntittiesSystem()));

    // create some enemies
    for (int i = 0; i < 1000; ++i)
    {
        iEntity entity = _entityScene.createEntity();
        entity.addComponent<PositionComponent>(iaVector2f(_rand.getNextFloat() * 500.0 + 250.0, _rand.getNextFloat() * 500.0 + 250.0));
        entity.addComponent<SizeComponent>(20.0f);

        iaVector2f direction(0.0, 1.0);
        direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);

        entity.addComponent<VelocityComponent>(direction, float32(_rand.getNextFloat() * 0.1 + 0.1));
        entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleGem.png"));
    }

    // init player
  /*  MovementControlComponent movementControl;
    position._position.set(500.0, 500.0);
    size._size = 10.0;
    velocity._speed = 1.0;
    health._health = 100;
    party._partyID = 20;
    visual._character = iTextureResourceFactory::getInstance().requestFile("particleStar.png");*/

    // _player = _ecs.createEntity(position, size, velocity, health, party, visual, movementControl);

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &Supremacy::onUpdate), iaTime::fromMilliseconds(16));
    _updateTimerHandle->start();
}

void Supremacy::onUpdate()
{
    /*for(auto entity : _entities)
    {
        PositionComponent *pos = _ecs.getComponent<PositionComponent>(entity.first);
        _quadtree.update(entity.second, iaVector2d(pos->_position._x, pos->_position._y));
    }*/
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
    // event.dispatch<iEventEntityCreated>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onEntityCreated));
    // event.dispatch<iEventEntityDestroyed>(IGOR_BIND_EVENT_FUNCTION(Supremacy::onEntityDestroyed));
}

static void renderTree(const std::shared_ptr<iQuadtreeNode> &node)
{
    if (node == nullptr)
    {
        return;
    }

    iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
    iRenderer::getInstance().drawRectangle(node->_box._x, node->_box._y, node->_box._width, node->_box._height);

    for (const auto userData : node->_userData)
    {
        iRenderer::getInstance().setColor(1.0, 1.0, 0.0, 1.0);
        iRenderer::getInstance().drawCircle(userData->getCircle().getX(), userData->getCircle().getY(), userData->getCircle().getRadius());
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
    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    _entityScene.updateSystems();

    // iRenderer::getInstance().setMaterial(_plainMaterial);
    // renderTree(_quadtree.getRoot());
}

/*bool Supremacy::onEntityCreated(iEventEntityCreated &event)
{
    iEntityPtr entity = event.getEntity();
    PositionComponent *pos = _ecs.getComponent<PositionComponent>(entity);
    SizeComponent *size = _ecs.getComponent<SizeComponent>(entity);

    if (pos == nullptr || size == nullptr)
    {
        return true;
    }

    const iaCircled circle(pos->_position._x, pos->_position._y, size->_size * 0.5);
    iQuadtreeUserDataPtr data = static_cast<iQuadtreeUserDataPtr>(new QuadtreeData(circle, event.getEntity()));
    _quadtree.insert(data);

    _entities[entity] = data;

    return true;
}

bool Supremacy::onEntityDestroyed(iEventEntityDestroyed &event)
{
    auto iter = _entities.find(event.getEntity());
    if (iter != _entities.end())
    {
        _entities.erase(iter);
    }

    return true;
}*/

bool Supremacy::onKeyDown(iEventKeyDown &event)
{
    // MovementControlComponent *movementControl = _ecs.getComponent<MovementControlComponent>(_player);
    /*if (movementControl == nullptr)
    {
        return false;
    }*/

    switch (event.getKey())
    {
    case iKeyCode::W:
        // movementControl->_up = true;
        return true;

    case iKeyCode::A:
        // movementControl->_left = true;
        return true;

    case iKeyCode::S:
        // movementControl->_down = true;
        return true;

    case iKeyCode::D:
        // movementControl->_right = true;
        return true;

    case iKeyCode::I:
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
    /*    MovementControlComponent *movementControl = _ecs.getComponent<MovementControlComponent>(_player);
        if (movementControl == nullptr)
        {
            return false;
        }*/

    switch (event.getKey())
    {
    case iKeyCode::W:
        // movementControl->_up = false;
        return true;

    case iKeyCode::A:
        //        movementControl->_left = false;
        return true;

    case iKeyCode::S:
        //      movementControl->_down = false;
        return true;

    case iKeyCode::D:
        //    movementControl->_right = false;
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