function Update(dt)

	local x = Input.GetStickLY()
	if math.abs(x) > 0.4 then
		SimulateInput.MoveMouse(x*10,0*dt)
	end
	--if Input.ButtonPressed(Input.GAMEPAD_A) then
	--	print "pressed A, simulating A on Keyboard"
	--	SimulateInput.PressKey(SimulateInput.KEY_A)
	--end
end

print "Success"