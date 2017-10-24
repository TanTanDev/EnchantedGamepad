-- 0: In game, 1: shopping
Mode = 0
print("Screen X: " .. AppSettings.ScreenResolutionX)
print("Screen Y: " .. AppSettings.ScreenResolutionY)
SimulateInput.PressKey(SimulateInput.KEY_A)
function Update(dt)

	if Input.ButtonPressed(Input.GAMEPAD_START) then
		Mode = not Mode
	end
	--local x = Input.GetStickLY()
	--if math.abs(x) > 0.4 then
	--	SimulateInput.MoveMouse(x*10,0*dt)
	--end
	--if Input.ButtonPressed(Input.GAMEPAD_A) then
	--	print "pressed A, simulating A on Keyboard"
	--	SimulateInput.PressKey(SimulateInput.KEY_A)
	--end
end