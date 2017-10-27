local Utility = {}

-- does both input check and simulation action
function Utility.CheckPress(gamepadButton, simulatedKey)
	if Input.ButtonPressed(gamepadButton) then
		SimulateInput.KeyDown(simulatedKey)
	elseif Input.ButtonReleased(gamepadButton) then
		SimulateInput.KeyUp(simulatedKey)
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