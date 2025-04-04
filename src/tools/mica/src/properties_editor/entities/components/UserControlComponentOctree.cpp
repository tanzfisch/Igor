// Igor game engine
// (c) Copyright 2012-2025 by Martin Loga
// see copyright notice in corresponding header file

#include "UserControlComponentOctree.h"

#include <igor/entities/components/iOctreeComponent.h>

UserControlComponentOctree::UserControlComponentOctree(const iEntitySceneID &scene, const iEntityID &entity, const iWidgetPtr parent)
    : UserControlComponent(scene, entity, "Octree", parent)
{
}

void UserControlComponentOctree::init()
{
    UserControlComponent::init();

    iWidgetLabelPtr label = new iWidgetLabel(_layout);
    label->setText("There is a reference to this entity in the Octree");
    label->setHorizontalAlignment(iHorizontalAlignment::Left);  
}

