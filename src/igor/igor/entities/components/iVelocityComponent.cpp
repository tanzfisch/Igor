// Igor game engine
// (c) Copyright 2012-2024 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/entities/components/iVelocityComponent.h>

namespace igor
{

    iEntityComponent *iVelocityComponent::createInstance()
    {
        return new iVelocityComponent();
    }

}