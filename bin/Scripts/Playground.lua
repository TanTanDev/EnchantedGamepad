-- VERSION 0
function Start()
	Utility = require("Scripts/Utility")
	TimerManager = require("Scripts/TimerManager")
	print("hrello")
end

function Update(dt)
	TimerManager.Update(dt)
	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_BUTTON) then
		SimulateInput.PressKey(SimulateInput.KEY_A)
		print("start new timer")
		-- continious press, loops every 0.3 sec until player release GAMEPAD_RIGHT_BUTTON
		TimerManager.SetTimer(0.4, true
		,function(timerHandle)
			if Input.ButtonHeld(Input.GAMEPAD_RIGHT_BUTTON) then
				print("will loop")
				SimulateInput.PressKey(SimulateInput.KEY_A)
			else
				print("abort")
				TimerManager.StopTimer(timerHandle)
			end
		end)
	end
end