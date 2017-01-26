#include "Ascent.h"

#include <Igor.h>
using namespace Igor;

/*#include <iEntityManager.h>
#include <iSystem.h>
#include <iComponent3DPosition.h>

iComponent3DPosition component3DPosition;

class BlubSystem : public iSystem
{

public:

    void handle()
    {
        iComponent3DPosition* component = static_cast<iComponent3DPosition*>(_entityManager->getComponent(component3DPosition.getID()));

        for (auto entityID : _entities)
        {
            iaVector3d* vec = reinterpret_cast<iaVector3d*>(component3DPosition.getData(entityID));
            vec->_x += 10;
            vec->_y += 20;
            vec->_z += 30;
        }
    }

};*/

int main()
{
    Igor::startup();

/*    iEntityManager entityManager;
    BlubSystem blubSystem;

    entityManager.registerComponent(&component3DPosition);
    entityManager.registerSystem(&blubSystem, vector<iComponent*>({ &component3DPosition }));

    uint64 e1 = entityManager.createEntity();
    uint64 e2 = entityManager.createEntity();

    entityManager.linkComponent(e1, component3DPosition.getID());
    entityManager.linkComponent(e2, component3DPosition.getID());


    iaVector3d* vec = reinterpret_cast<iaVector3d*>(component3DPosition.getData(e1));
    vec->_x = 1;
    vec->_y = 2;
    vec->_z = 3;

    entityManager.unlinkComponent(e1, component3DPosition.getID());

    vec = reinterpret_cast<iaVector3d*>(component3DPosition.getData(e2));
    vec->_x = 4;
    vec->_y = 5;
    vec->_z = 6;

    entityManager.handle();*/

	Ascent* ascent = new Ascent();
    ascent->run();
	delete ascent;
	
	Igor::shutdown();

	return 0;
}
