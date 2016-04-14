// Igor game engine
// (c) Copyright 2014-2016 by Martin Loga
// see copyright notice in corresponding header file

#include <iPhysicsContextThread.h>
#include <iPhysics.h>
#include <iaConsole.h>

namespace Igor
{

    iPhysicsContextThread::iPhysicsContextThread()
	{
	}

    uint64 iPhysicsContextThread::getWorld() const
    {
        return _worldID;
    }

	void iPhysicsContextThread::init()
	{
        _worldID = iPhysics::getInstance().createWorld()->getID();
	}

	void iPhysicsContextThread::deinit()
	{
        iPhysics::getInstance().destroyWorld(_worldID);
	}

};
