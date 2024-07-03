// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iBody2DComponent.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    iBody2DComponent::iBody2DComponent(iQuadtreed::ObjectPtr object, const iaString &name)
        : iEntityComponent(name), _object(object)
    {
    }

    void iBody2DComponent::onActivate(iEntityPtr entity)
    {
        auto &quadtree = entity->getScene()->getQuadtree();
        const iaVector2d center = quadtree.getRootBox().getCenter();

        _object = std::make_shared<iQuadtreed::Object>(iaCircled(center._x, center._y, 1.0), entity->getID());
        quadtree.insert(_object);
    }

    void iBody2DComponent::onDeactivate(iEntityPtr entity)
    {
        auto &quadtree = entity->getScene()->getQuadtree();
        quadtree.remove(_object);
    }
}