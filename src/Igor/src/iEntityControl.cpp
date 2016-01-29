// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include <iEntityControl.h>
#include <iEntityFactory.h>
#include <iEntity.h>
#include <iApplication.h>

namespace Igor
{
    
    iEntityControl::iEntityControl()
    {
        iApplication::getInstance().registerApplicationHandleDelegate(ApplicationHandleDelegate(this, &iEntityControl::onHandle));
    }

    iEntityControl::~iEntityControl()
    {
        iApplication::getInstance().unregisterApplicationHandleDelegate(ApplicationHandleDelegate(this, &iEntityControl::onHandle));
    }

    void iEntityControl::onHandle()
    {
        auto entities = iEntityFactory::getInstance().getEntities();

        auto entityIter = entities.begin();
        while (entities.end() != entityIter)
        {
            (*entityIter).second->onHandle();
            ++entityIter;
        }
    }

}