// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iOctreeComponent.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    iOctreeComponent::iOctreeComponent()
    {
    }

    iEntityComponent *iOctreeComponent::createInstance()
    {
        return new iOctreeComponent();
    }

    const iaString &iOctreeComponent::getTypeName()
    {
        static const iaString name("igor_octree_component");
        return name;
    }

    iOctreeComponent::iOctreeComponent(iOctreed::ObjectPtr object)
        : _object(object)
    {
    }

    iEntityComponentPtr iOctreeComponent::getCopy()
    {
        iOctreeComponent *component = new iOctreeComponent();
        return component;
    }

    bool iOctreeComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        if (!entity->getScene()->hasOctree())
        {
            return false;
        }

        return true;
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

    std::vector<iaString> iOctreeComponent::getInfo() const
    {
        std::vector<iaString> result = iEntityComponent::getInfo();

        if(_object != nullptr)
        {
            result.push_back(iaString("object initialized"));
        }
        else
        {
            result.push_back(iaString("no object"));
        }

        return result;
    }
}