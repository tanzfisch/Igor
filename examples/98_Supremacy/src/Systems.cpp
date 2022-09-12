#include "Systems.h"

PawnSystem::PawnSystem()
    : iEntitySystem()
{
    addComponentType(PositionComponent::ID);
    addComponentType(VelocityComponent::ID);
    addComponentType(HealthComponent::ID);
    addComponentType(PartyComponent::ID);
    addComponentType(MovementControlComponent::ID, static_cast<uint32>(iComponentFlag::Optional));
}

void PawnSystem::updateComponents(BaseComponent **components)
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

    if (health->_health > 0.0)
    {
        pos->_position += vel->_direction * vel->_speed;
    }
}

DisplayEntittiesSystem::DisplayEntittiesSystem()
    : iEntitySystem()
{
    addComponentType(PositionComponent::ID);
    addComponentType(VelocityComponent::ID);
    addComponentType(HealthComponent::ID);
}

void DisplayEntittiesSystem::updateComponents(BaseComponent **components)
{
    PositionComponent *pos = static_cast<PositionComponent *>(components[0]);
    VelocityComponent *vel = static_cast<VelocityComponent *>(components[1]);
    HealthComponent *health = static_cast<HealthComponent *>(components[2]);

    if (health->_health > 0.0)
    {
        iRenderer::getInstance().drawRectangle(pos->_position._x, pos->_position._y, 2, 2);
    }
}
