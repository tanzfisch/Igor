function onInit(self)
    local velocityComp = self.entity:getVelocityComponent()
    if velocityComp then
        local angVel = velocityComp:getAngularVelocity()
        angVel.z = 0.01
        velocityComp:setAngularVelocity(angVel)
    end
end

function onUpdate(self, dt)
    local transformComp = self.entity:getTransformComponent()
    local velocityComp = self.entity:getVelocityComponent()

    if transformComp and velocityComp then
        local pos = transformComp:getPosition()
        local vel = igor.Vector3()

        if igor.Keyboard.keyPressed(igor.KeyCode.W) then
            vel.y = vel.y - 1.0
        end

        if igor.Keyboard.keyPressed(igor.KeyCode.A) then
            vel.x = vel.x - 1.0
        end

        if igor.Keyboard.keyPressed(igor.KeyCode.S) then
            vel.y = vel.y + 1.0
        end

        if igor.Keyboard.keyPressed(igor.KeyCode.D) then
            vel.x = vel.x + 1.0
        end

        vel:normalize()
        vel = vel * 0.1
        velocityComp:setVelocity(vel)
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