#include <igor/entities/iEntitySystem.h>

namespace igor
{
    iEntitySystem::iEntitySystem(const iaString &name)
        : _name(name)
    {
    }

    const std::vector<uint32> &iEntitySystem::getComponentTypes() const
    {
        return _componentTypes;
    }

    const std::vector<uint32> &iEntitySystem::getComponentFlags() const
    {
        return _componentFlags;
    }

    void iEntitySystem::addComponentType(uint32 componentType, uint32 componentFlags)
    {
        _componentTypes.push_back(componentType);
        _componentFlags.push_back(componentFlags);
    }

    const iaString &iEntitySystem::getName() const
    {
        return _name;
    }

    bool iEntitySystemList::removeSystem(iEntitySystem &system)
    {
        for (uint32 i = 0; i < _systems.size(); i++)
        {
            if (&system == _systems[i])
            {
                _systems.erase(_systems.begin() + i);
                return true;
            }
        }

        con_err("system coudn't be removed " << system.getName());
        return false;
    }
}
