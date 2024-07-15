// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iOctreeComponent.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    iOctreeComponent::iOctreeComponent(iOctreed::ObjectPtr object)
        : _object(object)
    {
    }

    void iOctreeComponent::onActivate(iEntityPtr entity)
    {
        auto &octree = entity->getScene()->getOctree();
        const iaVector3d &center = octree.getVolume().getCenter();

        _object = std::make_shared<iOctreed::Object>(iaSphere(center, 0.0), entity->getID());
        octree.insert(_object);
    }

    void iOctreeComponent::onDeactivate(iEntityPtr entity)
    {
        auto &octree = entity->getScene()->getOctree();
        octree.remove(_object);
    }

}