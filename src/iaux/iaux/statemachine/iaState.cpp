// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <iaux/statemachine/iaState.h>

#include <iaux/system/iaConsole.h>
using namespace iaux;

namespace iaux
{
    iaIDGenerator32 iaState::_idGenerator;

    iaState::iaState(const iaString &name)
    {
        _name = name;
        _id = _idGenerator.getNextID();
    }

    iaStateID iaState::getID() const
    {
        return _id;
    }

    const iaString &iaState::getName() const
    {
        return _name;
    }

} // namespace iaux