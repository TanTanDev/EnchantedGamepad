local Utility = {}

-- does both input check and simulation action
-- joystickSide is optional if you want to check for joystick "presses"
function Utility.CheckPress(gamepadButton, simulatedKey, joystickSide)
	if Input.ButtonPressed(gamepadButton, joystickSide) then
		SimulateInput.KeyDown(simulatedKey)
	elseif Input.ButtonReleased(gamepadButton, joystickSide) then
		SimulateInput.KeyUp(simulatedKey)
	end
end

function Utility.CheckPressMultiple(gamepadButton, simulatedKey1, simulatedKey2)
	if Input.ButtonPressed(gamepadButton) then
		SimulateInput.PressKey(simulatedKey1, simulatedKey2)
	elseif Input.ButtonReleased(gamepadButton, joystickSide) then
		SimulateInput.KeyUp(simulatedKey1, simulatedKey2)
	end
end


-- Offsets a position(fromPos) based on left or right stick
function Utility.SetMousePosCenterOut(fromPos, xDistance, yDistance, stickSide)
	local stick = Input.GetStick(stickSide)

	-- Reset position
	if stick:Length() <= 0 then
		SimulateInput.SetMousePos(fromPos)
		return
	end
	local x = fromPos:x() + stick:x() * xDistance
	local y = fromPos:y() - stick:y() * yDistance
	SimulateInput.SetMousePos(x, y)
end

function Utility.MoveMouse(stickSide, speed)
	local move = Input.GetStick(stickSide)
	if move:Length() > 0 then
		SimulateInput.MoveMouse(move:x()*speed, -move:y()*speed)
	end
end

return Utility