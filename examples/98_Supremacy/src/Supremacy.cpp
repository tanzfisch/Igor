// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "Supremacy.h"

Supremacy::Supremacy(iWindow *window)
    : iLayer(window, L"Supremacy"), _viewOrtho(iView(false)), _quadtree(iaRectangled(0, 0, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT))
{
}

iaVector2d Supremacy::getRandomDir()
{
    iaVector2d direction(0.0, 1.0);
    direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);
    return direction;
}

iEntity Supremacy::createPlayer()
{
    // init player
    iEntity entity = _entityScene.createEntity("player");

    auto position = entity.addComponent<PositionComponent>(iaVector2d(PLAYFIELD_WIDTH * 0.5, PLAYFIELD_HEIGHT * 0.5));
    auto size = entity.addComponent<SizeComponent>(STANDARD_UNIT_SIZE);
    entity.addComponent<OrientationComponent>(0.0);
    entity.addComponent<VelocityComponent>(iaVector2d(1.0, 0.0), 1.0, true);
    entity.addComponent<PartyComponent>(FRIEND);
    entity.addComponent<DamageComponent>(0.0);
    entity.addComponent<HealthComponent>(100.0);
    entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("tomato.png"), false, iaTime::fromSeconds(_rand.getNextFloat()));
    auto &weapon = entity.addComponent<WeaponComponent>(WEAPON_ROCKETLAUNCHER);
    weapon._time = iTimer::getInstance().getTime();
    weapon._offset = iaVector2d(0.0, -STANDARD_UNIT_SIZE * 0.5);

    entity.addComponent<TargetComponent>(iInvalidEntityID, false, false);
    entity.addComponent<MovementControlComponent>();

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<QuadtreeObject>(iaCircled(position._position._x, position._position._y, size._size), entity.getID());
    _quadtree.insert(object._object);

    return entity;
}

iEntity Supremacy::createViewport(iEntityID targetID)
{
    iEntity target(targetID, _entityScene);
    const iaVector2d targetPosition = target.getComponent<PositionComponent>()._position;

    iEntity entity = _entityScene.createEntity("viewport");

    auto &viewportComp = entity.addComponent<ViewportComponent>();
    viewportComp._targetOffset.set(0.0, 0.0);
    viewportComp._targetID = targetID;
    viewportComp._viewport.setSize(PLAYFIELD_VIEWPORT_WIDTH, PLAYFIELD_VIEWPORT_HEIGHT);
    viewportComp._viewport.setCenter(targetPosition);

    return entity;
}

void Supremacy::createUnit(const iaVector2d &pos, uint32 party, iEntityID target)
{
    iEntity entity = _entityScene.createEntity("enemy");
    entity.addComponent<PositionComponent>(pos);
    entity.addComponent<OrientationComponent>(0.0);
    entity.addComponent<VelocityComponent>(getRandomDir(), 0.3, false);

    auto size = entity.addComponent<SizeComponent>(STANDARD_UNIT_SIZE);
    entity.addComponent<PartyComponent>(party);
    entity.addComponent<DamageComponent>(10.0);
    entity.addComponent<HealthComponent>(30.0);
    entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("broccoli.png"), false, iaTime::fromSeconds(_rand.getNextFloat()));

    entity.addComponent<TargetComponent>(target); // I don't like this but it's quick

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<QuadtreeObject>();
    object._object->_userData = entity.getID();
    object._object->_circle.set(pos, size._size);
    _quadtree.insert(object._object);
}

void Supremacy::updateViewRectangleSystem()
{
    if (!_player.isValid())
    {
        return;
    }

    auto &viewportComp = _viewport.getComponent<ViewportComponent>();
    auto &targetOffset = viewportComp._targetOffset;
    const iaVector2d &playerPosition = _player.getComponent<PositionComponent>()._position;
    const iaVector2d lastPlayerPosition = viewportComp._viewport.getCenter() + targetOffset;
    const iaVector2d diff = playerPosition - lastPlayerPosition;

    const auto width = viewportComp._viewport.getWidth() * 0.5 * 0.6;
    const auto height = viewportComp._viewport.getHeight() * 0.5 * 0.6;

    bool skipStep = false;

    if (std::abs(diff._x) > width ||
        std::abs(diff._y) > height)
    {
        skipStep = true;
    }

    if (!skipStep)
    {
        targetOffset += diff;
    }

    targetOffset._x = std::clamp(targetOffset._x, -width, width);
    targetOffset._y = std::clamp(targetOffset._y, -height, height);

    viewportComp._viewport.setCenter(playerPosition - targetOffset);
}

void Supremacy::onInit()
{
    _viewOrtho.setClearColor(0.3, 0.9, 0.5, 1.0);
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
    _viewport = createViewport(_player.getID());

    // create some enemies
    for (int i = 0; i < 30; ++i)
    {
        iaVector2d pos(_rand.getNextFloat() * PLAYFIELD_WIDTH, _rand.getNextFloat() * PLAYFIELD_HEIGHT);
        createUnit(pos, FOE, _player.getID());
    }

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &Supremacy::onUpdate), iaTime::fromMilliseconds(10));
    _updateTimerHandle->start();

    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");

    // init font for render profiler
    _font = new iTextureFont("StandardFont.png");
}

void Supremacy::onUpdateQuadtreeSystem()
{
    // update quadtree data
    auto quadtreeView = _entityScene.getEntities<PositionComponent, QuadtreeObjectComponent>();
    for (auto entity : quadtreeView)
    {
        auto [pos, object] = quadtreeView.get<PositionComponent, QuadtreeObjectComponent>(entity);

        iaVector2d newPosition(pos._position._x, pos._position._y);

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

void Supremacy::doughnutQuery(const iaCircled &circle, std::vector<std::pair<iEntityID, iaVector2d>> &hits)
{
    QuadtreeObjects objects;
    _quadtree.query(circle, objects);

    for (const auto &object : objects)
    {
        hits.emplace_back(std::pair<iEntityID, iaVector2d>(object->_userData, object->_circle._center - circle._center));
    }

    const bool right = circle._center._x - circle._radius < 0.0;
    const bool left = circle._center._x + circle._radius > _quadtree.getRootBox()._width;

    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _quadtree.getRootBox()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    for (const auto &queryCircle : additionalQueries)
    {
        _quadtree.query(queryCircle, objects);

        for (const auto &object : objects)
        {
            hits.emplace_back(std::pair<iEntityID, iaVector2d>(object->_userData, object->_circle._center - queryCircle._center));
        }
    }
}

bool Supremacy::intersectDoughnut(const iaVector2d &position, const iaRectangled &rectangle, iaVector2d &offset)
{
    if (iIntersection::intersects(position, rectangle))
    {
        return true;
    }

    const bool right = rectangle._x < 0.0;
    const bool left = rectangle.getRight() > _quadtree.getRootBox()._width;

    const bool bottom = rectangle._y < 0.0;
    const bool top = rectangle.getBottom() > _quadtree.getRootBox()._height;

    std::vector<iaRectangled> additionalQueries;

    if (right || left)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryRectangle._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (bottom)
        {
            queryRectangle._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryRectangle._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (right || left && bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryRectangle._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryRectangle._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryRectangle._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryRectangle);
    }

    for (const auto &queryRectangle : additionalQueries)
    {
        if (iIntersection::intersects(position, queryRectangle))
        {
            offset = rectangle.getTopLeft() - queryRectangle.getTopLeft();
            return true;
        }
    }

    return false;
}

bool Supremacy::intersectDoughnut(const iaVector2d &position, const iaCircled &circle, iaVector2d &offset)
{
    if (iIntersection::intersects(position, circle))
    {
        return true;
    }

    const bool right = circle._center._x - circle._radius < 0.0;
    const bool left = circle._center._x + circle._radius > _quadtree.getRootBox()._width;

    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _quadtree.getRootBox()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    for (const auto &queryCircle : additionalQueries)
    {
        if (iIntersection::intersects(position, queryCircle))
        {
            offset = circle._center - queryCircle._center;
            return true;
        }
    }

    return false;
}

void Supremacy::onUpdateFollowTargetSystem()
{
    // follow given target
    auto targetView = _entityScene.getEntities<PositionComponent, VelocityComponent, TargetComponent>();
    for (auto entity : targetView)
    {
        auto &target = targetView.get<TargetComponent>(entity);

        // skip if entity is not following targets
        if (!target._followTarget)
        {
            continue;
        }

        // skip if there is no valid target
        iEntity targetEntity(target._targetID, _entityScene);
        if (!targetEntity.isValid())
        {
            continue;
        }

        // skip if farget has no position
        if (!targetEntity.hasComponent<PositionComponent>())
        {
            continue;
        }

        auto [pos, vel] = targetView.get<PositionComponent, VelocityComponent>(entity);

        auto targetPosition = targetEntity.getComponent<PositionComponent>();
        const iaVector2d &targetPos = targetPosition._position;
        const iaVector2d &position = pos._position;
        iaVector2d offset;
        iaCircled circle(position, 300.0);

        if (!intersectDoughnut(targetPos, circle, offset))
        {
            if (target._inRange)
            {
                vel._direction = getRandomDir();
                target._inRange = false;
            }
        }
        else
        {
            vel._direction = targetPos - position + offset;
            vel._direction.normalize();
            target._inRange = true;
        }
    }
}

void Supremacy::onUpdatePositionSystem()
{
    // move entity
    auto positionUpdateView = _entityScene.getEntities<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>();
    for (auto entityID : positionUpdateView)
    {
        auto [pos, size, vel, party, damage, health] = positionUpdateView.get<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>(entityID);

        iaVector2d &position = pos._position;
        const float64 radius = size._size * 0.5;
        float64 speed = vel._speed;

        iaCircled circle(position, radius * 1.1);
        QuadtreeObjects objects;
        _quadtree.query(circle, objects);

        iaVector2d diversion;
        uint32 totalHits = 0;

        for (const auto &object : objects)
        {
            // skip self
            if (object->_userData == entityID)
            {
                continue;
            }

            // get other entity
            iEntity otherEntity(object->_userData, _entityScene);

            // check if we do damage to other entity
            auto *otherEntityParty = otherEntity.tryGetComponent<PartyComponent>();
            if (otherEntityParty != nullptr &&
                otherEntityParty->_partyID != party._partyID)
            {
                auto *otherEntityHealth = otherEntity.tryGetComponent<HealthComponent>();
                if (otherEntityHealth != nullptr)
                {
                    otherEntityHealth->_health -= damage._damage;
                }

                // destroy your self on impact (no friendly fire)
                if (health._destroyOnImpact)
                {
                    health._health = 0.0;
                }
            }

            // don't calculate diversion if non blockable
            if (vel._nonBlockable)
            {
                continue;
            }

            // ignore other entity for diversion if non blockable
            auto *otherEntityVel = otherEntity.tryGetComponent<VelocityComponent>();
            if (otherEntityVel == nullptr || otherEntityVel->_nonBlockable)
            {
                continue;
            }

            // calc diversion
            diversion += position - object->_circle._center;
            totalHits++;
        }

        iEntity entity(entityID, _entityScene);
        auto *range = entity.tryGetComponent<RangeComponent>();

        if (totalHits)
        {
            diversion.normalize();
            diversion *= speed;
            if (range != nullptr)
            {
                range->_distanceTraveled += diversion.length();
            }
            position += diversion;
        }

        iaVector2d direction = vel._direction;
        direction *= speed;
        if (range != nullptr)
        {
            range->_distanceTraveled += direction.length();
        }
        position += direction;

        // jump to other side
        if (position._x > PLAYFIELD_WIDTH)
        {
            position._x -= PLAYFIELD_WIDTH;
        }
        if (position._x < 0)
        {
            position._x += PLAYFIELD_WIDTH;
        }

        if (position._y > PLAYFIELD_HEIGHT)
        {
            position._y -= PLAYFIELD_HEIGHT;
        }
        if (position._y < 0)
        {
            position._y += PLAYFIELD_HEIGHT;
        }
    }
}

void Supremacy::fire(const iaVector2d &from, const iaVector2d &dir, uint32 party, float64 damage, float64 speed, float64 range, WeaponType waponType)
{
    // skip if out of range
    if (!iIntersection::intersects(from, _quadtree.getRootBox()))
    {
        return;
    }

    float64 size = 10.0;

    iaString texture;

    int32 count = 1;
    float64 variation = 0.0;

    switch (waponType)
    {
    case WeaponType::Knife:
        texture = "knife.png";
        size = 20.0;
        break;

    case WeaponType::Shotgun:
        texture = "pellets.png";
        size = 20.0;
        count = 3;
        variation = 1.0;
        break;

    case WeaponType::Minigun:
        texture = "bullet.png";
        size = 10.0;
        break;

    case WeaponType::RocketLauncher:
        texture = "rocket.png";
        size = 20.0;
        break;
    }

    for (int i = 0; i < count; ++i)
    {
        auto bullet = _entityScene.createEntity(texture);
        bullet.addComponent<PositionComponent>(from);
        bullet.addComponent<OrientationComponent>(0.0);
        bullet.addComponent<RangeComponent>(range);

        iaVector2d d = dir;
        d.rotateXY((_rand.getNextFloat() - 0.5) * variation * 0.5);
        float64 s = speed + variation * (_rand.getNextFloat() - 0.5) * 5.0;
        bullet.addComponent<VelocityComponent>(d, s, true);

        bullet.addComponent<PartyComponent>(party);
        bullet.addComponent<DamageComponent>(damage);
        bullet.addComponent<HealthComponent>(100.0, true);
        bullet.addComponent<SizeComponent>(size);
        bullet.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile(texture), true);

        auto &object = bullet.addComponent<QuadtreeObjectComponent>();

        object._object = std::make_shared<QuadtreeObject>(iaCircled(from, size * 0.5), bullet.getID());

        object._object = std::make_shared<QuadtreeObject>();
        object._object->_userData = bullet.getID();
        object._object->_circle.set(from, size * 0.5);
        _quadtree.insert(object._object);
    }
}

void Supremacy::aquireTargetFor(iEntity &entity)
{
    if (!entity.isValid())
    {
        return;
    }

    // aquire target for player
    auto &position = entity.getComponent<PositionComponent>();
    auto &party = entity.getComponent<PartyComponent>();
    auto &target = entity.getComponent<TargetComponent>();

    iaCircled circle(position._position, 100.0);
    std::vector<std::pair<iEntityID, iaVector2d>> hits;
    doughnutQuery(circle, hits);

    float32 minDistance = std::numeric_limits<float32>::max();
    target._targetID = iInvalidEntityID;

    for (const auto &hit : hits)
    {
        if (hit.first == entity.getID())
        {
            continue;
        }

        iEntity entity(hit.first, _entityScene);

        auto *entParty = entity.tryGetComponent<PartyComponent>();
        if (entParty == nullptr ||
            entParty->_partyID == party._partyID)
        {
            continue;
        }

        const float32 dist = hit.second.length2();
        if (dist < minDistance)
        {
            minDistance = dist;
            target._targetID = hit.first;
        }
    }
}

void Supremacy::onUpdateWeaponSystem()
{
    iaTime now = iTimer::getInstance().getTime();

    auto view = _entityScene.getEntities<WeaponComponent, TargetComponent, PositionComponent, VelocityComponent>();
    for (auto entity : view)
    {
        auto [weapon, target, position, velocity] = view.get<WeaponComponent, TargetComponent, PositionComponent, VelocityComponent>(entity);

        // skip if there is no target
        if (target._targetID == iInvalidEntityID)
        {
            continue;
        }

        // check if unit needs to stand still
        if (weapon._standStillToFire && velocity._direction.length() * velocity._speed > 0.0)
        {
            continue;
        }

        // check if it is time to fire again
        if (now - weapon._time < weapon._attackInterval)
        {
            continue;
        }
        weapon._time = now;

        // get target position
        iEntity targetEntity(target._targetID, _entityScene);
        auto &targetPosition = targetEntity.getComponent<PositionComponent>();

        const iaVector2d firePos = position._position + weapon._offset;
        iaVector2d direction = targetPosition._position - firePos;
        direction.normalize();

        fire(firePos, direction, FRIEND, weapon._damage, weapon._speed, weapon._range, weapon._weaponType);
    }
}

void Supremacy::onUpdateRangeSystem()
{
    auto view = _entityScene.getEntities<PositionComponent, RangeComponent, HealthComponent>();
    for (auto entity : view)
    {
        auto [positiom, range, health] = view.get<PositionComponent, RangeComponent, HealthComponent>(entity);

        if (range._distanceTraveled > range._maxRange)
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

void Supremacy::onUpdate(const iaTime &time)
{
    onUpdateQuadtreeSystem();
    onUpdateMovementControlSystem();
    onUpdateFollowTargetSystem();
    onUpdatePositionSystem();

    aquireTargetFor(_player);

    onUpdateWeaponSystem();

    onUpdateRangeSystem();
    onUpdateCleanUpTheDeadSystem();

    updateViewRectangleSystem();
}

void Supremacy::onDeinit()
{
    // release resources
    if (_font != nullptr)
    {
        delete _font;
        _font = nullptr;
    }

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

void Supremacy::onRenderHUD()
{
    float64 health = 0.0;

    if (_player.isValid())
    {
        auto &healthComp = _player.getComponent<HealthComponent>();
        health = healthComp._health;
    }

    iaMatrixd matrix;
    matrix.translate(0.0, 0.0, -1.0);
    iRenderer::getInstance().setModelMatrix(matrix);

    iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
    iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(15.0f);
    iRenderer::getInstance().drawString(10, 10, iaString::toString(health, 0));
}

void Supremacy::onRenderOrtho()
{
    const iaTime now = iTimer::getInstance().getTime();

    auto &viewportComp = _viewport.getComponent<ViewportComponent>();
    const iaRectangled &viewRectangle = viewportComp._viewport;
    iaRectangled intersetRectangle = viewRectangle;
    float64 scale = viewRectangle._width * 0.1;
    intersetRectangle.adjust(-scale, -scale, scale * 2.0, scale * 2.0);

    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);
    matrix.translate(0, 0, -1);
    matrix.scale(static_cast<float64>(getWindow()->getClientWidth() / PLAYFIELD_VIEWPORT_WIDTH),
                 (static_cast<float64>(getWindow()->getClientHeight()) / PLAYFIELD_VIEWPORT_HEIGHT), 1.0);
    matrix.translate(-viewRectangle._x, -viewRectangle._y, 0);
    iRenderer::getInstance().setModelMatrix(matrix);

    // draw entities TODO
    auto view = _entityScene.getEntities<PositionComponent, SizeComponent, VisualComponent>();

    for (auto entity : view)
    {
        auto [pos, size, visual] = view.get<PositionComponent, SizeComponent, VisualComponent>(entity);

        iaVector2d position;

        if (!intersectDoughnut(pos._position, intersetRectangle, position))
        {
            continue;
        }

        position = pos._position + position;

        if (visual._useDirection)
        {
            const float64 width = size._size;
            iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
            iRenderer::getInstance().drawTexturedRectangle(position._x - width * 0.5, position._y - width * 0.5, width, width, visual._texture);
        }
        else
        {
            float64 width = size._size;
            float64 height = size._size;

            iaTime time = now + visual._timerOffset;
            float64 timing = std::fmod(time.getMilliseconds(), 1000.0) / 1000.0 * M_PI * 2;

            float64 value = sin(timing) * 0.5;
            if (value > 0.0)
            {
                value *= 0.5;
            }

            width += value * 5.0;
            height += (1.0 - value) * 10.0;

            iRenderer::getInstance().setMaterial(_materialWithTextureAndBlending);
            iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 0.6);
            iRenderer::getInstance().drawTexturedRectangle(position._x - size._size * 0.5, position._y + value * 2.0, size._size, size._size * 0.5, _shadow);

            iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
            iRenderer::getInstance().drawTexturedRectangle(position._x - size._size * 0.5, position._y - size._size + value * 15.0, width, height, visual._texture);
        }
    }

    onRenderHUD();
}

bool Supremacy::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    if (!_player.isValid())
    {
        return false;
    }

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
    }

    return false;
}

bool Supremacy::onKeyUp(iEventKeyUp &event)
{
    if (!_player.isValid())
    {
        return false;
    }

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