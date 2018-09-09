-- VERSION 0
function Start()
	Utility = require("Scripts/Utility")
	TimerManager = require("Scripts/Libraries/TimerManager")

	Config = {}
	Config.containers = {3,3}
	containers = {[0]=0, [1]=nil, [2]=2}

	for i, v in pairs(Config.containers) do
		print(i)
		table.insert(containers, {obj = v})
	end
	print("length: " .. #containers)


end

function Update(dt)
	TimerManager.Update(dt)

	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_BUTTON) then
		print("start new timer")
		-- continious loop every 0.4 sec until player release GAMEPAD_RIGHT_BUTTON
		TimerManager.SetTimer(0.4, true
		,function(timerHandle)
			if Input.ButtonHeld(Input.GAMEPAD_RIGHT_BUTTON) then
				print("will loop")
			else
				print("abort")
				TimerManager.StopTimer(timerHandle)
			end
		end)
	end
end