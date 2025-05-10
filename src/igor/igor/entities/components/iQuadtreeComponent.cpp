// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iQuadtreeComponent.h>

#include <igor/entities/iEntityScene.h>

namespace igor
{
    iQuadtreeComponent::iQuadtreeComponent(iQuadtreed::ObjectPtr object)
        : _object(object)
    {
    }

    iEntityComponent *iQuadtreeComponent::createInstance()
    {
        return new iQuadtreeComponent();
    }  

    const iaString& iQuadtreeComponent::getTypeName()
    {
        static const iaString name("igor_quadtree_component");
        return name;
    }       

    iEntityComponentPtr iQuadtreeComponent::getCopy()
    {
        iQuadtreeComponent *component = new iQuadtreeComponent();
        return component;
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

    std::vector<iaString> iQuadtreeComponent::getInfo() const
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