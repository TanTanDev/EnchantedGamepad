local Utility = {}

-- does both input check and simulation action
-- "press key down if gamepad key is down, vice versa for key up"
-- joystickSide is optional if you want to check for joystick "presses"
function Utility.CheckPress(gamepadButton, simulatedKey, joystickSide)
	if Input.ButtonPressed(gamepadButton, joystickSide) then
		SimulateInput.KeyDown(simulatedKey)
	elseif Input.ButtonReleased(gamepadButton, joystickSide) then
		SimulateInput.KeyUp(simulatedKey)
	end
end

-- same as checkpress but also execuses a function provided if said button is held
function Utility.CheckPressEx(gamepadButton, simulatedKey, joystickSide, funcOnHeld)
	if Input.ButtonPressed(gamepadButton, joystickSide) then
		SimulateInput.KeyDown(simulatedKey)
	elseif Input.ButtonReleased(gamepadButton, joystickSide) then
		SimulateInput.KeyUp(simulatedKey)
	end
	if Input.ButtonHeld(gamepadButton, joystickSide) then
		funcOnHeld()
	end
end

-- to press multiple windows keys bound to 1 gamepad button
function Utility.CheckPressMultiple(gamepadButton, simulatedKey1, simulatedKey2)
	if Input.ButtonPressed(gamepadButton) then
		SimulateInput.PressKey(simulatedKey1, simulatedKey2)
	elseif Input.ButtonReleased(gamepadButton, joystickSide) then
		SimulateInput.KeyUp(simulatedKey1, simulatedKey2)
	end
end

-- Move the mouse to 'fromPos', then offets the mouse with the joystick direction
function Utility.SetMousePosCenterOut(fromPos, xDistance, yDistance, stickSide)
	-- initialize variable prevLengthWasAbove to Utility table
	-- used to make sure we won't lock the mouse position
	if prevLengthWasAbove == nil then
		prevLengthWasAbove = false
	end

	local stickInput = Input.GetStick(stickSide)
	-- reset position when joystick is 'resting'
	if stickInput:Length() <= 0 then
		if forceSet or prevLengthWasAbove then
			SimulateInput.SetMousePos(fromPos)
		end
		prevLengthWasAbove = false
		return
	end
	prevLengthWasAbove = true
	local x = fromPos:x() + stickInput:x() * xDistance
	local y = fromPos:y() - stickInput:y() * yDistance
	SimulateInput.SetMousePos(x, y)
end

function Utility.GetPosCenterOut(fromPos, xDistance, yDistance, stickSide, threshold)
	local stickInput = Input.GetStick(stickSide, threshold)
	local x = fromPos:x() + stickInput:x() * xDistance
	local y = fromPos:y() - stickInput:y() * yDistance
	return Vector(x, y)
end

function Utility.ClampPointInRect(point, rect, extraPaddingX, extraPaddingY)
	point:x(math.max(point:x(), rect:X()+extraPaddingX))
	point:x(math.min(point:x(), rect:X()+rect:Width()-extraPaddingX))
	point:y(math.max(point:y(), rect:Y()+extraPaddingY))
	point:y(math.min(point:y(), rect:Y()+rect:Height()-extraPaddingY))
end

function Utility.Clamp(v, min, max)
	return math.max(math.min(v, max), min)
end

function Utility.MoveMouse(stickSide, speed)
	local move = Input.GetStick(stickSide)
	if move:Length() > 0 then
		SimulateInput.MoveMouse(move:x()*speed, -move:y()*speed)
	end
end

return Utility