#include <iSystem.h>

#include <iEntityManager.h>

namespace Igor
{

    iSystem::iSystem()
    {
        iEntityManager::getInstance().registerSystem(this);
    }

    iSystem::~iSystem()
    {
        iEntityManager::getInstance().unregisterSystem(this);
    }


}