-- VERSION 0
function Start()
	timerManager = require("Scripts/Libraries/TimerManager")

	loopTime = 0.6
end

function Update(dt)
	timerManager.Update(dt)

	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_BUTTON) then
		print("start new timer")
		-- continious loop until player release GAMEPAD_RIGHT_BUTTON
		timerManager.SetTimer(loopTime, true
		,function(timerHandle)
			if Input.ButtonHeld(Input.GAMEPAD_RIGHT_BUTTON) then
				print("will loop")
			else
				print("abort")
				timerManager.StopTimer(timerHandle)
			end
		end)
	end
end