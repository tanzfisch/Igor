#ifndef __ENTITYMANAGER__
#define __ENTITYMANAGER__

#include <iOctree.h>
using namespace Igor;

#include <iaSingleton.h>
using namespace IgorAux;

#include <map>
using namespace std;

#include "Entity.h"

namespace Igor
{
    class iPhysicsBody;
}

class EntityManager : public iaSingleton<EntityManager>
{

    friend class iaSingleton<EntityManager>;
    friend class Entity;

public:

    Entity* getEntity(uint64 entityID);

    void getEntities(const iSphered& sphere, vector<uint64>& result);

    void handle();

    int64 getTerrainMaterialID() const;
    int64 getEntityMaterialID() const;
    int64 getBulletMaterialID() const;

private:

    map<uint64, Entity*> _entities;

    int64 _terrainMaterialID = 0;
    int64 _bulletMaterialID = 0;
    int64 _entityMaterialID = 0;

    iOctree* _octree = nullptr;

    void registerEntity(Entity* entity);
    void unregisterEntity(Entity* entity);

    void onContact(iPhysicsBody* body0, iPhysicsBody* body1);
    void onContactTerrainBullet(iPhysicsBody* body0, iPhysicsBody* body1);

    void initPhysics();

    EntityManager();
    ~EntityManager();

};

#endif