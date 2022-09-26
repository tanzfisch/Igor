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

    // init player
    _player = _entityScene.createEntity();

    auto position = _player.addComponent<PositionComponent>(iaVector2d(800.0, 100.0));
    auto size = _player.addComponent<SizeComponent>(20.0);

    _player.addComponent<VelocityComponent>(iaVector2d(1.0, 0.0), 1.0, true);
    _player.addComponent<PartyComponent>(10u);
    _player.addComponent<DamageComponent>(0.0);
    _player.addComponent<HealthComponent>(100.0);
    _player.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleStar.png"));

    _player.addComponent<MovementControlComponent>();
    auto &object = _player.addComponent<QuadtreeObjectComponent>();
    object._object = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>(new iQuadtreeObject<float64, iEntityID>());
    object._object->_userData = _player.operator iEntityID();
    object._object->_position.set(position._position._x, position._position._y);
    _quadtree.insert(object._object);

    // create some enemies
    for (int i = 0; i < 1000; ++i)
    {
        iEntity entity = _entityScene.createEntity();
        auto position = entity.addComponent<PositionComponent>(iaVector2d(_rand.getNextFloat() * 1000.0, _rand.getNextFloat() * 1000.0));
        auto size = entity.addComponent<SizeComponent>(10.0);

        iaVector2d direction(0.0, 1.0);
        direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);

        entity.addComponent<VelocityComponent>(direction, 0.3, false);
        entity.addComponent<PartyComponent>(20u);
        entity.addComponent<DamageComponent>(100.0);
        entity.addComponent<HealthComponent>(10.0);
        entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleGem.png"));
        auto &object = entity.addComponent<QuadtreeObjectComponent>();
        object._object = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>(new iQuadtreeObject<float64, iEntityID>());
        object._object->_userData = entity.operator iEntityID();
        object._object->_position.set(position._position._x, position._position._y);
        _quadtree.insert(object._object);

        entity.addComponent<TargetComponent>(_player.operator igor::iEntityID());
    }

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &Supremacy::onUpdate), iaTime::fromMilliseconds(16));
    _updateTimerHandle->start();

    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");
}

void Supremacy::onUpdate()
{
    // update quadtree data
    auto quadtreeView = _entityScene.getEntities<PositionComponent, SizeComponent, QuadtreeObjectComponent>();
    for (auto entity : quadtreeView)
    {
        auto [pos, size, object] = quadtreeView.get<PositionComponent, SizeComponent, QuadtreeObjectComponent>(entity);

        const iaVector2d newPosition(pos._position._x, pos._position._y);
        _quadtree.update(object._object, newPosition);
    }

    auto movementControlView = _entityScene.getEntities<MovementControlComponent, VelocityComponent>();

    for (auto entity : movementControlView)
    {
        auto [movementControl, vel] = movementControlView.get<MovementControlComponent, VelocityComponent>(entity);

        vel._direction.set(0, 0);

        if (movementControl._left)
        {
            vel._direction._x -= 1.0;
        }
        if (movementControl._right)
        {
            vel._direction._x += 1.0;
        }
        if (movementControl._up)
        {
            vel._direction._y -= 1.0;
        }
        if (movementControl._down)
        {
            vel._direction._y += 1.0;
        }        
    }

    // follow given target
    auto targetView = _entityScene.getEntities<PositionComponent, VelocityComponent, TargetComponent>();

    for (auto entity : targetView)
    {
        auto &target = targetView.get<TargetComponent>(entity);

        iEntity targetEntity(target._targetID, &_entityScene);
        if (!targetEntity.hasComponent<PositionComponent>())
        {
            continue;
        }

        auto [pos, vel] = targetView.get<PositionComponent, VelocityComponent>(entity);

        auto targetPosition = targetEntity.getComponent<PositionComponent>();
        const iaVector2d &targetPos = targetPosition._position;
        const iaVector2d &position = pos._position;

        if (position.distance2(targetPos) > 200.0 * 200.0)
        {
            if (target._inRange)
            {
                vel._direction.set(0.0, 1.0);
                vel._direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);
                target._inRange = false;
            }
        }
        else
        {
            vel._direction = targetPos - position;
            vel._direction.normalize();
            target._inRange = true;
        }
    }

    // aquire target for player
    static int countdown = 100; // LOL timer?
    if (countdown > 0)
    {
        auto &position = _player.getComponent<PositionComponent>();
        auto &size = _player.getComponent<SizeComponent>();
        iaCircled circle(position._position, 50.0);
        std::vector<std::shared_ptr<iQuadtreeObject<float64, iEntityID>>> objects;
        _quadtree.query(circle, objects);

        float32 minDistance = 999999999;
        std::shared_ptr<iQuadtreeObject<float64, iEntityID>> foundObject;

        for (const auto &object : objects)
        {
            if (object->_userData == _player.operator iEntityID())
            {
                continue;
            }

            float32 dist = object->_position.distance2(circle._center);
            if (dist < minDistance)
            {
                minDistance = dist;
                foundObject = object;
            }
        }

        if (foundObject != nullptr)
        {
            auto bullet = _entityScene.createEntity();
            bullet.addComponent<PositionComponent>(circle._center);

            iaVector2d direction = foundObject->_position - circle._center;
            direction.normalize();
            bullet.addComponent<VelocityComponent>(direction, 10.0f, true);
            bullet.addComponent<PartyComponent>(10u);
            bullet.addComponent<DamageComponent>(1.0);
            bullet.addComponent<HealthComponent>(0.0);
            bullet.addComponent<SizeComponent>(10.0f);
            bullet.addComponent<OriginComponent>(circle._center);

            bullet.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleTrail.png"), true);

            countdown = 100;
        }
    }
    else
    {
        countdown--;
    }

    // move entity
    auto positionUpdateView = _entityScene.getEntities<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>();

    for (auto entity : positionUpdateView)
    {
        auto [pos, size, vel, party, damage, health] = positionUpdateView.get<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>(entity);

        iaVector2d &position = pos._position;
        iaVector2d &direction = vel._direction;
        const float64 radius = size._size * 0.5;
        const float64 speed = vel._speed;
        const iaVector2d projection = position + direction * speed;

        if (projection._x > 1000.0 ||
            projection._x < 0.0)
        {
            direction._x = -direction._x;
        }

        if (projection._y > 1000.0 ||
            projection._y < 0.0)
        {
            direction._y = -direction._y;
        }

        iaCircled circle(projection, radius);
        std::vector<std::shared_ptr<iQuadtreeObject<float64, iEntityID>>> objects;
        _quadtree.query(circle, objects);

        for(const auto &object : objects)
        {
            // iEntityID entityID = reinterpret_cast<uint32>(object->_userData);
            
        }


        // only move if nothing is in the way
        if (objects.size() <= 1 || vel._nonBlockable)
        {
            position = position + direction * speed;
        }
    }

    auto originUpdateView = _entityScene.getEntities<PositionComponent, OriginComponent>();

    for (auto entity : originUpdateView)
    {
        auto [pos, ori] = originUpdateView.get<PositionComponent, OriginComponent>(entity);

        if (pos._position.distance2(ori._origin) > 300.0 * 300.0)
        {
            _entityScene.destroyEntity(entity);
        }
    }
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

static void renderTree(const std::shared_ptr<iQuadtreeNode<float64, iEntityID>> &node)
{
    if (node == nullptr)
    {
        return;
    }

    iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
    iRenderer::getInstance().drawRectangle(node->_box._x, node->_box._y, node->_box._width, node->_box._height);

    iRenderer::getInstance().setColor(1.0, 1.0, 0.0, 1.0);
    for (const auto object : node->_objects)
    {
        iRenderer::getInstance().drawCircle(object->_position._x, object->_position._y, 5.0f);
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

    auto view = _entityScene.getEntities<PositionComponent, VelocityComponent, SizeComponent, VisualComponent>();

    for (auto entity : view)
    {
        auto [pos, vel, size, visual] = view.get<PositionComponent, VelocityComponent, SizeComponent, VisualComponent>(entity);

        const iaVector2d &position = pos._position;
        const float64 width = size._size;

        if (visual._useDirectory)
        {
            const iaVector2d &dir = vel._direction;
            iRenderer::getInstance().setMaterial(_plainMaterial); // TODO SOOOO BAD

            iRenderer::getInstance().setLineWidth(2.0);
            iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 1.0);
            iRenderer::getInstance().drawLine(position._x, position._y, position._x + dir._x, position._y + dir._y);
        }
        else
        {
            iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
            iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 0.6);
            iRenderer::getInstance().drawTexture(position._x - width * 0.5, position._y - width * 0.25, width, width * 0.5, _shadow);

            iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
            iRenderer::getInstance().drawTexture(position._x - width * 0.5, position._y - width, width, width, visual._character);
        }
    }

    iRenderer::getInstance().setMaterial(_plainMaterial);
    renderTree(_quadtree.getRoot());
}

bool Supremacy::onKeyDown(iEventKeyDown &event)
{
    MovementControlComponent &movementControl = _player.getComponent<MovementControlComponent>();

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl._up = true;
        return true;

    case iKeyCode::A:
        movementControl._left = true;
        return true;

    case iKeyCode::S:
        movementControl._down = true;
        return true;

    case iKeyCode::D:
        movementControl._right = true;
        return true;

    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    return false;
}

bool Supremacy::onKeyUp(iEventKeyUp &event)
{
    MovementControlComponent &movementControl = _player.getComponent<MovementControlComponent>();

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl._up = false;
        return true;

    case iKeyCode::A:
        movementControl._left = false;
        return true;

    case iKeyCode::S:
        movementControl._down = false;
        return true;

    case iKeyCode::D:
        movementControl._right = false;
        return true;
    }

    return false;
}