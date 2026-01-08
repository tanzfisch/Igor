function onUpdate(self, dt)
    local velocityComp = self.entity:getVelocityComponent()
    local userDataComp = self.entity:getUserDataComponent()

    local walkSpeedFactor = 1.0

    if userDataComp then
        walkSpeedFactor = userDataComp:getFloat64("walkSpeedFactor")
    end

    con_endl("walkSpeedFactor " .. tostring(walkSpeedFactor));

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
        vel = vel * walkSpeedFactor
        velocityComp:setVelocity(vel)
    end
end