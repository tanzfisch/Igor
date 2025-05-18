// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include <igor/entities/iEntityComponent.h>

namespace igor
{

    iEntityComponent::iEntityComponent()
    {
    }

    const iEntityComponentID &iEntityComponent::getID() const
    {
        return _id;
    }

    bool iEntityComponent::onLoad(iEntityPtr entity, bool &asyncLoad)
    {
        return true;
    }

    void iEntityComponent::onActivate(iEntityPtr entity)
    {
    }

    void iEntityComponent::onDeactivate(iEntityPtr entity)
    {
    }

    void iEntityComponent::onUnLoad(iEntityPtr entity)
    {
    }

    iEntityComponentState iEntityComponent::getState() const
    {
        return _state;
    }

    std::wostream &operator<<(std::wostream &stream, const iEntityComponentState &state)
    {
        const static std::wstring text[] = {
            L"Unloaded",
            L"UnloadedInactive",
            L"LoadFailed",
            L"Inactive",
            L"Active"};

        stream << text[static_cast<int>(state)];
        return stream;
    }

    std::vector<iaString> iEntityComponent::getInfo() const
    {
        std::vector<iaString> result;

        // nothing yet

        return result;
    }

    void iEntityComponent::setDirty()
    {
        _version++;
    }

    uint16 iEntityComponent::getVersion() const
    {
        return _version;
    }

} // igor