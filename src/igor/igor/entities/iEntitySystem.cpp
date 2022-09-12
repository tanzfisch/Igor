#include <igor/entities/iEntitySystem.h>

namespace igor
{
    const std::vector<uint32> &iEntitySystem::getComponentTypes() const
    {
        return _componentTypes;
    }

    const std::vector<uint32> &iEntitySystem::getComponentFlags() const
    {
        return _componentFlags;
    }

    void iEntitySystem::addComponentType(uint32 componentType, uint32 componentFlag)
    {
        _componentTypes.push_back(componentType);
        _componentFlags.push_back(componentFlag);
    }

    bool iEntitySystem::isValid()
    {
        for(uint32 i = 0;  i < _componentFlags.size(); i++) {
            if((_componentFlags[i] & static_cast<uint32>(iComponentFlag::Optional)) == 0) {
                return true;
            }
        }
        return false;
    }

    bool iEntitySystemList::removeSystem(iEntitySystem& system)
    {
        for(uint32 i = 0; i < _systems.size(); i++) {
            if(&system == _systems[i]) {
                _systems.erase(_systems.begin() + i);
                return true;
            }
        }
        return false;
    }
}
