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

    auto position = _player.addComponent<PositionComponent>(iaVector2f(800.0, 100.0));
    auto size = _player.addComponent<SizeComponent>(20.0f);

    _player.addComponent<VelocityComponent>(iaVector2f(1.0, 0.0), 1.0f);
    _player.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleStar.png"));

    _player.addComponent<MovementControlComponent>();
    auto &object = _player.addComponent<QuadtreeObjectComponent>();
    object._object = iQuadtreeObjectPtr(new iQuadtreeObject());
    object._object->_userData = reinterpret_cast<iUserData>(_player.operator igor::iEntityID());
    object._object->_circle.set(position._position._x, position._position._y, size._size);
    _quadtree.insert(object._object);

    // create some enemies
    for (int i = 0; i < 1000; ++i)
    {
        iEntity entity = _entityScene.createEntity();
        auto position = entity.addComponent<PositionComponent>(iaVector2f(_rand.getNextFloat() * 1000.0, _rand.getNextFloat() * 1000.0));
        auto size = entity.addComponent<SizeComponent>(10.0f);

        iaVector2f direction(0.0, 1.0);
        direction.rotateXY(_rand.getNextFloat() * M_PI * 2.0);

        entity.addComponent<VelocityComponent>(direction, 0.3f);
        entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleGem.png"));
        auto &object = entity.addComponent<QuadtreeObjectComponent>();
        object._object = iQuadtreeObjectPtr(new iQuadtreeObject());
        object._object->_userData = reinterpret_cast<iUserData>(entity.operator igor::iEntityID());
        object._object->_circle.set(position._position._x, position._position._y, size._size);
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

    auto targetView = _entityScene.getEntities<PositionComponent, VelocityComponent, TargetComponent>();

    iaCircled circle;
    circle.setRadius(50);

    for (auto entity : targetView)
    {
        auto [pos, vel, target] = targetView.get<PositionComponent, VelocityComponent, TargetComponent>(entity);

        circle.setX(pos._position._x);
        circle.setY(pos._position._y);

        iQuadtreeObjects objects;
        _quadtree.query(circle, objects);

        const iUserData userData = reinterpret_cast<iUserData>(target._targetID);

        auto iter = std::find_if(objects.begin(), objects.end(),
                                 [&](const iQuadtreeObjectPtr &x)
                                 { return x->_userData == userData; });

        if (iter == objects.end())
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
            vel._direction._x = (*iter)->_circle._center._x - pos._position._x;
            vel._direction._y = (*iter)->_circle._center._y - pos._position._y;
            vel._direction.normalize();
            target._inRange = true;
        }
    }

    static int countdown = 100; // LOL

    // aquire target for player
    if (countdown > 0)
    {
        const iUserData playerUserData = reinterpret_cast<iUserData>(_player.operator igor::iEntityID());
        auto &position = _player.getComponent<PositionComponent>();
        auto &size = _player.getComponent<SizeComponent>();
        iaCircled circle(position._position._x, position._position._y, 50.0);
        iQuadtreeObjects objects;
        _quadtree.query(circle, objects);

        float32 minDistance = 999999999;
        iQuadtreeObjectPtr foundObject;

        for (const auto &object : objects)
        {
            if (object->_userData == playerUserData)
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
            auto bullet = _entityScene.createEntity();
            bullet.addComponent<PositionComponent>(iaVector2f(circle._center._x, circle._center._y));

            iaVector2d direction = foundObject->_circle._center - circle._center;
            direction.normalize();
            bullet.addComponent<VelocityComponent>(iaVector2f(direction._x, direction._y), 10.0f);
            bullet.addComponent<SizeComponent>(10.0f);
            bullet.addComponent<OriginComponent>(iaVector2f(circle._center._x, circle._center._y));

            bullet.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("particleTrail.png"), true);

            countdown = 100;
        }
    }
    else
    {
        countdown--;
    }

    auto positionUpdateView = _entityScene.getEntities<PositionComponent, VelocityComponent>();

    for (auto entity : positionUpdateView)
    {
        auto [pos, vel] = positionUpdateView.get<PositionComponent, VelocityComponent>(entity);

        iaVector2f &position = pos._position;
        iaVector2f &direction = vel._direction;
        const float32 speed = vel._speed;
        const iaVector2f projection = position + direction * speed;

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

        position = position + direction * speed;
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

static void renderTree(const std::shared_ptr<iQuadtreeNode> &node)
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
        iRenderer::getInstance().drawCircle(object->_circle.getX(), object->_circle.getY(), object->_circle.getRadius());
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

        const iaVector2f &position = pos._position;        
        const float32 width = size._size;

        if (visual._useDirectory)
        {
            const iaVector2f &dir = vel._direction;
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

    // iRenderer::getInstance().setMaterial(_plainMaterial);
    // renderTree(_quadtree.getRoot());
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