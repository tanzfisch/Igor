// Igor game engine
// (c) Copyright 2012-2023 by Martin Loga
// see copyright notice in corresponding header file

#include <igor/threading/iThread.h>

#include <igor/physics/iPhysics.h>
#include <iaux/system/iaConsole.h>

namespace igor
{

    iThread::iThread(const iaString &type)
    : iaThread(type)
    {
    }

    uint64 iThread::getWorld() const
    {
        return _worldID;
    }

    void iThread::init()
    {
        iaThread::init();
        _worldID = iPhysics::getInstance().createWorld()->getID();
    }

    void iThread::deinit()
    {
        iPhysics::getInstance().destroyWorld(_worldID);
        iaThread::deinit();
    }

}; // namespace igor
