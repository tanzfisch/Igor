// Igor game engine
// (c) Copyright 2014-2015 by Martin Loga
// see copyright notice in corresponding header file

#include "GameControl.h"

#include <iEntityFactory.h>
#include <iEntity.h>
#include <iPhysics.h>
#include <iTimer.h>
using namespace Igor;

#include <iaVector2.h>
using namespace IgorAux;

GameControl::GameControl()
{
    srand(42);
    _tiles = new Tiles(50, 50);

    iEntity* entity = iEntityFactory::getInstance().createEntity();
    entity->setPosition(iaVector3f(10, 0, 10));
    entity->setBool(static_cast<uint32>(GameAction::MoveLeft), false);
    entity->setBool(static_cast<uint32>(GameAction::MoveRight), false);
    entity->setBool(static_cast<uint32>(GameAction::MoveUp), false);
    entity->setBool(static_cast<uint32>(GameAction::MoveDown), false);
    entity->setBool(static_cast<uint32>(GameAction::PrimaryAction), false);
    entity->setBool(static_cast<uint32>(GameAction::SecondaryAction), false);
    entity->setInt(_directionIndex, 0);
    iaMatrixf offset;
    offset.translate(0, 0, 0);
    entity->setBody(iPhysics::getInstance().createBody(iPhysics::getInstance().createBox(1, 1, 1, offset)));
    entity->getBody()->setMass(5);
    _player = entity->getID();

    offset.identity();
    offset.translate(0, -1, 0);
    _floor = iPhysics::getInstance().createBody(iPhysics::getInstance().createBox(1000, 2, 1000, offset));
    iaMatrixf position;
    _floor->updateMatrix(position);

    for (int z = 0; z < 10; ++z)
    {
        for (int x = 0; x < 10; ++x)
        {
            int s = rand() % 20;
            if (s <= 12)
            {
                s = 0;
            }
            else if (s <= 15)
            {
                s = 1;
            }
            else if (s <= 18)
            {
                s = 2;
            }
            else if (s <= 19)
            {
                s = 3;
            }
            
            for (int y = 0; y < s; ++y)
            {
                iEntity* entity = iEntityFactory::getInstance().createEntity();
                entity->setPosition(iaVector3f(static_cast<float32>(x), static_cast<float32>(y) + 0.5f, static_cast<float32>(z)));
                iaMatrixf offset;
                offset.translate(0, 0, 0);
                entity->setBody(iPhysics::getInstance().createBody(iPhysics::getInstance().createBox(1, 1, 1, offset)));
                entity->getBody()->setMass(50);
            }
        }
    }
    _isRunning = true;
}

GameControl::~GameControl()
{

}

uint32 GameControl::getPlayer()
{
    return _player;
}

void GameControl::activateEntityAction(uint32 id, GameAction action)
{
    iEntityFactory::getInstance().getEntity(id)->setBool(static_cast<uint32>(action), true);
}

void GameControl::deactivateEntityAction(uint32 id, GameAction action)
{
    iEntityFactory::getInstance().getEntity(id)->setBool(static_cast<uint32>(action), false);
}

void GameControl::exitGame()
{
    _isRunning = false;
}

bool GameControl::onHandle()
{
    const float32 additionalStraveForce = 1000.0f;
    const float32 additionalJumpForce = 100.0f;
    const float32 maxStraveForce = 1000.0f;

    // replace this with some newton magic
    const iaVector3f moveRight(additionalStraveForce, 0, -additionalStraveForce);
    const iaVector3f moveLeft(-additionalStraveForce, 0, additionalStraveForce);
    const iaVector3f moveUp(-additionalStraveForce, 0, -additionalStraveForce);
    const iaVector3f moveDown(additionalStraveForce, 0, additionalStraveForce);
    const iaVector3f jump(0, additionalJumpForce, 0);
    
    int32 resultingDirection = 0;

    auto entities = iEntityFactory::getInstance().getEntities();
    auto entityIter = entities.begin();
    while (entities.end() != entityIter)
    {
        iEntity* entity = (*entityIter).second;

/*        if (entity->hasBody() && entity->getBody()->getBodyType() == PhysicsBodyType::Player)
        {
            iaVector3f resultingForce;

            if (entity->getBool(static_cast<uint32>(GameAction::MoveLeft)))
            {
                resultingForce += moveLeft;
            }

            if (entity->getBool(static_cast<uint32>(GameAction::MoveRight)))
            {
                resultingForce += moveRight;
            }

            if (entity->getBool(static_cast<uint32>(GameAction::MoveUp)))
            {
                resultingForce += moveUp;
            }

            if (entity->getBool(static_cast<uint32>(GameAction::MoveDown)))
            {
                resultingForce += moveDown;
            }

            if (entity->getBool(static_cast<uint32>(GameAction::PrimaryAction)))
            {
                // TODO only add if contact with floor
                resultingForce += jump;
            }

            iaVector2f dir(resultingForce._x, resultingForce._z);

            if (0.0f < dir.length2())
            {
                resultingDirection = static_cast<int32>(((dir.angleX() / (2.0f * M_PI)) * 8.0f) + 0.5f);

                if (resultingForce._z >= 0)
                {
                    resultingDirection = 8 - resultingDirection;              
                }

                entity->setInt(_directionIndex, resultingDirection);

                dir.normalize();
                dir *= maxStraveForce;

                resultingForce._x = dir._x;
                resultingForce._z = dir._y;
            }

            auto velocity = entity->getVelocity();

            velocity._y = 0;
            velocity.negate();
            resultingForce += velocity / (1.0 / iPhysics::getSimulationRate());

            entity->setForce(resultingForce);

        }*/

        ++entityIter;
    }

    return _isRunning;
}

Tiles* GameControl::getTiles()
{
    return _tiles;
}