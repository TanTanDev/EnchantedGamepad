local Utility = require "Scripts/Utility"

print("Screen X: " .. AppSettings.ScreenResolutionX)
print("Screen Y: " .. AppSettings.ScreenResolutionY)

local ScreenResolution = Vector(AppSettings.ScreenResolutionX, AppSettings.ScreenResolutionY)

function Update(dt)

	if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
		Utility.SetMousePosCenterOut(ScreenResolution*0.5, 400, 400, Input.GAMEPAD_LEFT_STICK)
	else
		Utility.MoveMouse(Input.GAMEPAD_LEFT_STICK, 1500*dt)
	end

	-- look screen on left button
	Utility.CheckPress(Input.GAMEPAD_LEFT_BUTTON, SimulateInput.KEY_SPACE)

	-- X,Y,A,B changing depending on dpad
	if Input.ButtonHeld(Input.GAMEPAD_DPAD_RIGHT) then
		-- level spells
		Utility.CheckPressMultiple(Input.GAMEPAD_X, SimulateInput.KEY_CTRL, SimulateInput.KEY_Q)
		Utility.CheckPressMultiple(Input.GAMEPAD_A, SimulateInput.KEY_CTRL, SimulateInput.KEY_W)
		Utility.CheckPressMultiple(Input.GAMEPAD_B, SimulateInput.KEY_CTRL, SimulateInput.KEY_E)
		Utility.CheckPressMultiple(Input.GAMEPAD_Y, SimulateInput.KEY_CTRL, SimulateInput.KEY_R)

	elseif Input.ButtonHeld(Input.GAMEPAD_DPAD_LEFT) then
		-- use items
		Utility.CheckPress(Input.GAMEPAD_X, SimulateInput.KEY_1)
		Utility.CheckPress(Input.GAMEPAD_A, SimulateInput.KEY_2)
		Utility.CheckPress(Input.GAMEPAD_B, SimulateInput.KEY_3)
		Utility.CheckPress(Input.GAMEPAD_Y, SimulateInput.KEY_4)

	elseif Input.ButtonHeld(Input.GAMEPAD_DPAD_UP) then
		Utility.CheckPressMultiple(Input.GAMEPAD_X, SimulateInput.KEY_CTRL, SimulateInput.KEY_1)
		Utility.CheckPressMultiple(Input.GAMEPAD_A, SimulateInput.KEY_CTRL, SimulateInput.KEY_2)
		Utility.CheckPressMultiple(Input.GAMEPAD_B, SimulateInput.KEY_CTRL, SimulateInput.KEY_3)
		Utility.CheckPressMultiple(Input.GAMEPAD_Y, SimulateInput.KEY_CTRL, SimulateInput.KEY_4)
	elseif Input.ButtonHeld(Input.GAMEPAD_DPAD_DOWN) then

		if Input.ButtonPressed(Input.GAMEPAD_X) then
			SimulateInput.KeyDown(SimulateInput.KEY_MENU)
			SimulateInput.KeyDown(SimulateInput.MOUSE_LEFT)
			-- do this a bit later
			--Utility.Delay(funciton() 
			--	SimulateInput.KeyUp(SimulateInput.MOUSE_LEFT)
			--	SimulateInput.KeyUp(SimulateInput.KEY_MENU)
			--end)
		end
		if Input.ButtonReleased(Input.GAMEPAD_X) then
		end
		--Utility.CheckPressMultiple(Input.GAMEPAD_X, SimulateInput.KEY_CTRL, SimulateInput.KEY_1)
		--Utility.CheckPressMultiple(Input.GAMEPAD_A, SimulateInput.KEY_CTRL, SimulateInput.KEY_2)
		--Utility.CheckPressMultiple(Input.GAMEPAD_B, SimulateInput.KEY_CTRL, SimulateInput.KEY_3)
		--Utility.CheckPressMultiple(Input.GAMEPAD_Y, SimulateInput.KEY_CTRL, SimulateInput.KEY_4)
	else
		-- default: use spells
		Utility.CheckPress(Input.GAMEPAD_X, SimulateInput.KEY_Q)
		Utility.CheckPress(Input.GAMEPAD_A, SimulateInput.KEY_W)
		Utility.CheckPress(Input.GAMEPAD_B, SimulateInput.KEY_E)
		Utility.CheckPress(Input.GAMEPAD_Y, SimulateInput.KEY_R)
	end

	Utility.CheckPress(Input.GAMEPAD_LEFT_TRIGGER, SimulateInput.KEY_D)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_TRIGGER, SimulateInput.KEY_F)

	Utility.CheckPress(Input.GAMEPAD_RIGHT_BUTTON, SimulateInput.MOUSE_RIGHT)
	Utility.CheckPress(Input.GAMEPAD_BACK, SimulateInput.KEY_B)
	Utility.CheckPress(Input.GAMEPAD_START, SimulateInput.KEY_B)

	Utility.CheckPress(Input.GAMEPAD_JOYSTICK_LEFT, SimulateInput.KEY_LEFT, Input.GAMEPAD_RIGHT_STICK)
	Utility.CheckPress(Input.GAMEPAD_JOYSTICK_RIGHT, SimulateInput.KEY_RIGHT, Input.GAMEPAD_RIGHT_STICK)
	Utility.CheckPress(Input.GAMEPAD_JOYSTICK_UP, SimulateInput.KEY_UP, Input.GAMEPAD_RIGHT_STICK)
	Utility.CheckPress(Input.GAMEPAD_JOYSTICK_DOWN, SimulateInput.KEY_DOWN, Input.GAMEPAD_RIGHT_STICK)

end