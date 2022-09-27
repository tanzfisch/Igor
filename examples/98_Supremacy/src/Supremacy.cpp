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

iaVector2d Supremacy::getRandomDir() const
{
    iaVector2d direction(0.0, 1.0);
    direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);
    return direction;
}

iEntity Supremacy::createPlayer()
{
    // init player
    iEntity player = _entityScene.createEntity("player");

    auto position = player.addComponent<PositionComponent>(iaVector2d(800.0, 100.0));
    auto size = player.addComponent<SizeComponent>(20.0);

    player.addComponent<VelocityComponent>(iaVector2d(1.0, 0.0), 1.0, true);
    player.addComponent<PartyComponent>(10u);
    player.addComponent<DamageComponent>(0.0);
    player.addComponent<HealthComponent>(100.0);
    player.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleStar.png"));

    player.addComponent<MovementControlComponent>();
    auto &object = player.addComponent<QuadtreeObjectComponent>();
    object._object = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>(new iQuadtreeObject<float64, iEntityID>());
    object._object->_userData = player.operator iEntityID();
    object._object->_circle.set(position._position._x, position._position._y, size._size);
    _quadtree.insert(object._object);

    return player;
}

void Supremacy::createEnemy()
{
    iEntity entity = _entityScene.createEntity("enemy");
    auto position = entity.addComponent<PositionComponent>(iaVector2d(_rand.getNextFloat() * 1000.0, _rand.getNextFloat() * 1000.0));
    auto size = entity.addComponent<SizeComponent>(20.0);
    entity.addComponent<VelocityComponent>(getRandomDir(), 0.3, false);
    entity.addComponent<PartyComponent>(20u);
    entity.addComponent<DamageComponent>(0.0);
    entity.addComponent<HealthComponent>(100.0);
    entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleGem.png"));
    entity.addComponent<TargetComponent>(_player.operator iEntityID());

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>(new iQuadtreeObject<float64, iEntityID>());
    object._object->_userData = entity.operator iEntityID();
    object._object->_circle.set(position._position._x, position._position._y, size._size);
    _quadtree.insert(object._object);
}

void Supremacy::onInit()
{
    _viewOrtho.setClearColor(0.5, 0.5, 0.5, 1.0);
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
    material = iMaterialResourceFactory::getInstance().getMaterial(_plainMaterial);
    material->setRenderState(iRenderState::Blend, iRenderStateValue::On);

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(getWindow()));

    _rand.setSeed(1337);

    _player = createPlayer();

    // create some enemies
    for (int i = 0; i < 1000; ++i)
    {
        createEnemy();
    }

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &Supremacy::onUpdate), iaTime::fromMilliseconds(16));
    _updateTimerHandle->start();

    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");
}

void Supremacy::onUpdateQuadtreeSystem()
{
    // update quadtree data
    auto quadtreeView = _entityScene.getEntities<PositionComponent, SizeComponent, QuadtreeObjectComponent>();
    for (auto entity : quadtreeView)
    {
        auto [pos, size, object] = quadtreeView.get<PositionComponent, SizeComponent, QuadtreeObjectComponent>(entity);

        const iaVector2d newPosition(pos._position._x, pos._position._y);
        _quadtree.update(object._object, newPosition);
    }
}

void Supremacy::onUpdateMovementControlSystem()
{
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
}

void Supremacy::onUpdateFollowTargetSystem()
{
    // follow given target
    auto targetView = _entityScene.getEntities<PositionComponent, VelocityComponent, TargetComponent>();
    for (auto entity : targetView)
    {
        auto &target = targetView.get<TargetComponent>(entity);

        iEntity targetEntity(target._targetID, _entityScene);
        if (!targetEntity.hasComponent<PositionComponent>())
        {
            continue;
        }

        auto [pos, vel] = targetView.get<PositionComponent, VelocityComponent>(entity);

        auto targetPosition = targetEntity.getComponent<PositionComponent>();
        const iaVector2d &targetPos = targetPosition._position;
        const iaVector2d &position = pos._position;

        if (position.distance2(targetPos) > 300.0 * 300.0)
        {
            if (target._inRange)
            {
                vel._direction = getRandomDir();
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
}

void Supremacy::onUpdatePositionSystem()
{
    // move entity
    auto positionUpdateView = _entityScene.getEntities<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>();
    for (auto entity : positionUpdateView)
    {
        auto [pos, size, vel, party, damage, health] = positionUpdateView.get<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>(entity);

        iaVector2d &position = pos._position;
        iaVector2d &direction = vel._direction;
        const float64 radius = size._size * 0.5;
        float64 speed = vel._speed;
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

        iaCircled circle(position, radius * 1.1);
        std::vector<std::shared_ptr<iQuadtreeObject<float64, iEntityID>>> objects;
        _quadtree.query(circle, objects);

        iaVector2d diversion;
        uint32 totalHits = 0;

        if (!vel._nonBlockable)
        {
            for (const auto &object : objects)
            {
                if (object->_userData == entity)
                {
                    continue;
                }

                iEntity ent(object->_userData, _entityScene);

                auto *entVel = ent.tryGetComponent<VelocityComponent>();
                if (entVel == nullptr)
                {
                    continue;
                }

                if (entVel->_nonBlockable)
                {
                    continue;
                }

                diversion += position - object->_circle._center;
                totalHits++;
            }

            if (totalHits)
            {
                diversion.normalize();
                position += diversion * speed;
            }
        }
        else
        {
            for (const auto &object : objects)
            {
                if (object->_userData == entity)
                {
                    continue;
                }

                iEntity ent(object->_userData, _entityScene);

                auto *entParty = ent.tryGetComponent<PartyComponent>();
                if (entParty == nullptr ||
                    entParty->_partyID == party._partyID)
                {
                    continue;
                }

                auto *entHealth = ent.tryGetComponent<HealthComponent>();
                if (entHealth == nullptr)
                {
                    continue;
                }

                entHealth->_health -= damage._damage;

                if (health._destroyOnImpact)
                {
                    health._health = 0.0;
                }

                break;
            }
        }

        position += direction * speed;
    }
}

void Supremacy::aquireTargetFor(iEntity &entity)
{
    // aquire target for player
    static int countdown = 10; // LOL timer?
    VelocityComponent &playerVelocity = entity.getComponent<VelocityComponent>();
    float64 trueSpeed = playerVelocity._direction.length() * playerVelocity._speed;
    if (countdown < 0 && trueSpeed <= 0.0)
    {
        auto &position = entity.getComponent<PositionComponent>();
        auto &size = entity.getComponent<SizeComponent>();
        auto &party = entity.getComponent<PartyComponent>();
        iaCircled circle(position._position, 150.0);
        std::vector<std::shared_ptr<iQuadtreeObject<float64, iEntityID>>> objects;
        _quadtree.query(circle, objects);

        float32 minDistance = 999999999;
        std::shared_ptr<iQuadtreeObject<float64, iEntityID>> foundObject;

        for (const auto &object : objects)
        {
            if (object->_userData == entity.operator iEntityID())
            {
                continue;
            }

            iEntity entity(object->_userData, _entityScene);

            auto *entParty = entity.tryGetComponent<PartyComponent>();
            if (entParty == nullptr ||
                entParty->_partyID == party._partyID)
            {
                continue;
            }

            float32 dist = object->_circle._center.distance2(circle._center);
            if (dist < minDistance)
            {
                minDistance = dist;
                foundObject = object;
            }
        }

        if (foundObject != nullptr)
        {
            const iaVector2d firePos(position._position._x, position._position._y - size._size * 0.5);

            auto bullet = _entityScene.createEntity("bullet");
            auto &position = bullet.addComponent<PositionComponent>(firePos);

            iaVector2d direction = foundObject->_circle._center - firePos;
            direction.normalize();
            bullet.addComponent<VelocityComponent>(direction, 10.0f, true);
            bullet.addComponent<PartyComponent>(10u);
            bullet.addComponent<DamageComponent>(50.0);
            bullet.addComponent<HealthComponent>(100.0, true);
            auto &size = bullet.addComponent<SizeComponent>(10.0f);
            bullet.addComponent<OriginComponent>(firePos);

            auto &object = bullet.addComponent<QuadtreeObjectComponent>();
            object._object = std::shared_ptr<iQuadtreeObject<float64, iEntityID>>(new iQuadtreeObject<float64, iEntityID>());
            object._object->_userData = bullet.operator iEntityID();
            object._object->_circle.set(position._position._x, position._position._y, size._size);
            _quadtree.insert(object._object);

            bullet.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleFlame.png"), true);

            countdown = 3;
        }
    }
    countdown--;
}

void Supremacy::onUpdateDistanceToOriginSystem()
{
    auto originUpdateView = _entityScene.getEntities<PositionComponent, OriginComponent, HealthComponent>();
    for (auto entity : originUpdateView)
    {
        auto [pos, ori, health] = originUpdateView.get<PositionComponent, OriginComponent, HealthComponent>(entity);

        if (pos._position.distance2(ori._origin) > 300.0 * 300.0)
        {
            health._health = 0.0;
        }
    }
}

void Supremacy::onUpdateCleanUpTheDeadSystem()
{
    // clean up the dead
    auto healthView = _entityScene.getEntities<HealthComponent, QuadtreeObjectComponent>();

    for (auto entity : healthView)
    {
        auto [health, object] = healthView.get<HealthComponent, QuadtreeObjectComponent>(entity);

        if (health._health <= 0.0)
        {
            _quadtree.remove(object._object);
            _entityScene.destroyEntity(entity);
        }
    }
}

void Supremacy::onUpdate()
{
    onUpdateQuadtreeSystem();
    onUpdateMovementControlSystem();
    onUpdateFollowTargetSystem();
    onUpdatePositionSystem();

    aquireTargetFor(_player);

    onUpdateDistanceToOriginSystem();
    onUpdateCleanUpTheDeadSystem();
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

void Supremacy::onRenderOrtho()
{
    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    // draw entities
    auto view = _entityScene.getEntities<PositionComponent, VelocityComponent, SizeComponent, VisualComponent, HealthComponent>();

    for (auto entity : view)
    {
        auto [pos, vel, size, visual, health] = view.get<PositionComponent, VelocityComponent, SizeComponent, VisualComponent, HealthComponent>(entity);

        if (health._health < 0)
        {
            continue;
        }

        const iaVector2d &position = pos._position;
        const float64 width = size._size;

        if (visual._useDirection)
        {
            iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
            iRenderer::getInstance().drawTexture(position._x - width * 0.5, position._y - width * 0.5, width, width, visual._character);
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