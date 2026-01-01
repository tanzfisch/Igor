function onInit(self)
    local velocityComp = self.entity:getVelocityComponent()
    if velocityComp then
        local vel = velocityComp:getVelocity()
        local angVel = velocityComp:getAngularVelocity()
        vel.x = 0.15
        vel.y = 0.12
        angVel.z = 0.01
        velocityComp:setVelocity(vel)
        velocityComp:setAngularVelocity(angVel)
    end
end

function onUpdate(self, dt)
    local transformComp = self.entity:getTransformComponent()
    local velocityComp = self.entity:getVelocityComponent()
    if transformComp and velocityComp then
        local pos = transformComp:getPosition()
        local vel = velocityComp:getVelocity()
        local angVel = velocityComp:getAngularVelocity()
        if pos.x > 9 or pos.x < -9 then
            vel.x = -vel.x
            angVel.z = -angVel.z
        end
        if pos.y > 9 or pos.y < -9 then
            vel.y = -vel.y
            angVel.z = -angVel.z
        end
        velocityComp:setVelocity(vel)
        velocityComp:setAngularVelocity(angVel)
    end
end

function onFinal()
    con_endl("final")
end

function onMessage(self, message, sender)
    con_endl("message")
end

function onEvent(self, action)
    con_endl("input")
end