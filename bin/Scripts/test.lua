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

	Utility.CheckPress(Input.GAMEPAD_X, SimulateInput.KEY_Q)
	Utility.CheckPress(Input.GAMEPAD_A, SimulateInput.KEY_W)
	Utility.CheckPress(Input.GAMEPAD_B, SimulateInput.KEY_E)
	Utility.CheckPress(Input.GAMEPAD_Y, SimulateInput.KEY_R)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_BUTTON, SimulateInput.MOUSE_RIGHT)
	Utility.CheckPress(Input.GAMEPAD_BACK, SimulateInput.KEY_B)
	Utility.CheckPress(Input.GAMEPAD_START, SimulateInput.KEY_B)
end