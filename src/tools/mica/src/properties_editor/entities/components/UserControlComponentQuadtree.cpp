// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "UserControlComponentQuadtree.h"

#include <igor/entities/components/iQuadtreeComponent.h>

UserControlComponentQuadtree::UserControlComponentQuadtree(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Quadtree", parent)
{
}

void UserControlComponentQuadtree::init()
{
    UserControlComponent::init();

    iWidgetLabelPtr label = new iWidgetLabel(_layout);
    label->setText("There is a reference to this entity in the Quadtree");
    label->setHorizontalAlignment(iHorizontalAlignment::Left);  
}

void UserControlComponentQuadtree::onDestroyComponent(iEntityPtr entity)
{
    con_assert(entity != nullptr, "zero pointer");
    
    entity->destroyComponent<iQuadtreeComponent>();
}