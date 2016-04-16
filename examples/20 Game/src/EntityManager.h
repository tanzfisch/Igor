#ifndef __ENTITYMANAGER__
#define __ENTITYMANAGER__

#include <iOctree.h>
using namespace Igor;

#include <iaSingleton.h>
using namespace IgorAux;

#include <map>
using namespace std;

#include "Entity.h"

class EntityManager : public iaSingleton<EntityManager>
{

    friend class iaSingleton<EntityManager>;
    friend class Entity;

public:

    Entity* getEntity(uint64 entityID);

    void getEntities(const iSphered& sphere, vector<uint64>& result);

    void handle();

private:

    map<uint64, Entity*> _entities;

    iOctree* _octree = nullptr;

    void registerEntity(Entity* entity);
    void unregisterEntity(Entity* entity);
    void updateEntityPos(Entity* entity);

    EntityManager();
    ~EntityManager();

};

#endif