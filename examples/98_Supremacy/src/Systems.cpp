#include "Systems.h"

PawnSystem::PawnSystem(iQuadtree *quadtree)
    : _quadtree(quadtree)
{
}

void PawnSystem::update(iEntityScenePtr scene)
{
    auto view = scene->getEntities<PositionComponent, VelocityComponent>();

    for (auto entity : view)
    {
        auto [pos, vel] = view.get<PositionComponent, VelocityComponent>(entity);

        /*    if (movementControl != nullptr)
            {
                vel->_direction.set(0, 0);

                if (movementControl->_left)
                {
                    vel->_direction._x -= 1.0;
                }
                if (movementControl->_right)
                {
                    vel->_direction._x += 1.0;
                }
                if (movementControl->_up)
                {
                    vel->_direction._y -= 1.0;
                }
                if (movementControl->_down)
                {
                    vel->_direction._y += 1.0;
                }
            }*/

        iaVector2f &position = pos._position;
        iaVector2f &direction = vel._direction;
        const float32 speed = vel._speed;
        const iaVector2f projection = position + direction * speed;

        if (projection._x > 1000.0 ||
            projection._x < 0.0)
        {
            direction._x = -direction._x;
        }

        if (projection._y > 1000.0 ||
            projection._y < 0.0)
        {
            direction._y = -direction._y;
        }

        position = position + direction * speed;
    }
}

DisplayEntittiesSystem::DisplayEntittiesSystem()
{
    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");
}

void DisplayEntittiesSystem::update(iEntityScenePtr scene)
{
    auto view = scene->getEntities<PositionComponent, SizeComponent>();

    for (auto entity : view)
    {
        auto [pos, size] = view.get<PositionComponent, SizeComponent>(entity);

        const iaVector2f &position = pos._position;
        const float32 width = size._size;

        iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 0.6);
        iRenderer::getInstance().drawTexture(position._x - width * 0.5, position._y - width * 0.25, width, width * 0.5, _shadow);

        iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
        // iRenderer::getInstance().drawTexture(position._x - width * 0.5, position._y - width, width, width, visual->_character);
    }

}

