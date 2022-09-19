#include "Systems.h"

PawnSystem::PawnSystem(iQuadtree *quadtree)
    : iEntitySystem("Pawn"), _quadtree(quadtree)
{
    addComponentType(PositionComponent::ID);
    addComponentType(VelocityComponent::ID);
    addComponentType(HealthComponent::ID);
    addComponentType(PartyComponent::ID);
    addComponentType(MovementControlComponent::ID, static_cast<uint32>(iComponentFlag::Optional));
}

void PawnSystem::updateComponents(iBaseComponent **components)
{
    PositionComponent *pos = static_cast<PositionComponent *>(components[0]);
    VelocityComponent *vel = static_cast<VelocityComponent *>(components[1]);
    HealthComponent *health = static_cast<HealthComponent *>(components[2]);
    PartyComponent *party = static_cast<PartyComponent *>(components[3]);
    MovementControlComponent *movementControl = static_cast<MovementControlComponent *>(components[4]);

    if (movementControl != nullptr)
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
    }
    else
    {
        // TODO how to find other party?
    }

    if (health->_health > 0.0)
    {
        const iaVector2d position = pos->_quadtreeUserData->_circle._center;
        const iaVector2d projection = position + vel->_direction * vel->_speed;

        if (projection._x > 1000.0 ||
            projection._x < 0.0)
        {
            vel->_direction._x = -vel->_direction._x;
        }

        if (projection._y > 1000.0 ||
            projection._y < 0.0)
        {
            vel->_direction._y = -vel->_direction._y;
        }

        const iaVector2d newPosition = position + vel->_direction * vel->_speed;

        _quadtree->update(pos->_quadtreeUserData, newPosition);
    }
}

DisplayEntittiesSystem::DisplayEntittiesSystem()
    : iEntitySystem("DisplayEntitties")
{
    addComponentType(PositionComponent::ID);
    addComponentType(VelocityComponent::ID);
    addComponentType(HealthComponent::ID);
    addComponentType(VisualComponent::ID);

    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");
}

void DisplayEntittiesSystem::updateComponents(iBaseComponent **components)
{
    PositionComponent *position = static_cast<PositionComponent *>(components[0]);
    VelocityComponent *vel = static_cast<VelocityComponent *>(components[1]);
    HealthComponent *health = static_cast<HealthComponent *>(components[2]);
    VisualComponent *visual = static_cast<VisualComponent *>(components[3]);

    if (health->_health > 0.0)
    {
        const iaCircled &circle = position->_quadtreeUserData->getCircle();
        const float64 width = circle.getRadius() * 2.0;

        iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 0.6);
        iRenderer::getInstance().drawTexture(circle._center._x - 20, circle._center._y - 10, width, width * 0.5, _shadow);

        iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
        iRenderer::getInstance().drawTexture(circle._center._x - 20, circle._center._y - 40, width, width, visual->_character);
    }
}
