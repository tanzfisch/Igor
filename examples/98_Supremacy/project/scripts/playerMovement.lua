function onUpdate(self, dt)
    local velocityComp = self.entity:getVelocityComponent()

    if velocityComp then
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
        vel = vel * 1.0
        velocityComp:setVelocity(vel)
    end
end