#include "Systems.h"

PawnSystem::PawnSystem()
    : iEntitySystem("Pawn")
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
        pos->_position += vel->_direction * vel->_speed;
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
    PositionComponent *pos = static_cast<PositionComponent *>(components[0]);
    VelocityComponent *vel = static_cast<VelocityComponent *>(components[1]);
    HealthComponent *health = static_cast<HealthComponent *>(components[2]);
    VisualComponent *visual = static_cast<VisualComponent *>(components[3]);

    if (health->_health > 0.0)
    {
        iRenderer::getInstance().setColor(0.0, 0.0, 0.0, 0.6);
        iRenderer::getInstance().drawTexture(pos->_position._x - 20, pos->_position._y - 10, 40.0, 20.0, _shadow);

        iRenderer::getInstance().setColor(1.0, 1.0, 1.0, 1.0);
        iRenderer::getInstance().drawTexture(pos->_position._x - 20, pos->_position._y - 40, 40.0, 40.0, visual->_character);

    }
}
