// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iQuadtreeComponent.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    iQuadtreeComponent::iQuadtreeComponent(iQuadtreed::ObjectPtr object, const iaString &name)
        : iEntityComponent(name), _object(object)
    {
    }

    void iQuadtreeComponent::onActivate(iEntityPtr entity)
    {
        auto &quadtree = entity->getScene()->getQuadtree();
        const iaVector2d &center = quadtree.getArea().getCenter();

        _object = std::make_shared<iQuadtreed::Object>(iaCircled(center._x, center._y, 1.0), entity->getID());
        quadtree.insert(_object);
    }

    void iQuadtreeComponent::onDeactivate(iEntityPtr entity)
    {
        auto &quadtree = entity->getScene()->getQuadtree();
        quadtree.remove(_object);
    }
}