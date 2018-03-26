--dist = Bind("dist", 4, "InputFloat")
--fromX = Bind("fromX", 400, "")
--fromY = Bind("fromY", 400)
--daBool = Bind("daBool", true)



test1 = Bind("test1", 1, "InputFloat")
test2 = Bind("test2", 1, "DragFloat")
test3 = Bind("test3", 1, "SliderFloat")
test4 = Bind("test4", 1, "InputInt")
test5 = Bind("test5", 1, "DragInt")
test6 = Bind("test6", 1, "SliderInt")
test7 = Bind("test7", 1, "ToggleBox")
test8 = Bind("test8", 1, "ToggleButton")
--test9 = Bind("test9", 1, "")

function Update(dt)
	--SetMousePosCenterOut(Vector(fromX,fromY), dist,dist,Input.GAMEPAD_LEFT_STICK)
	ClearLog()
	print("test1 :" .. test1)
	print("test2 :" .. test2)
	print("test3 :" .. test3)
	print("test4 :" .. test4)
	print("test5 :" .. test5)
	print("test6 :" .. test6)
	print("test7 :" .. test7)
	print("test8 :" .. test8)
	--print("test9 :" .. test9)
end


function SetMousePosCenterOut(fromPos, xDistance, yDistance, stickSide)
	local stick = Input.GetStick(stickSide)
	if prevLengthWasAbove == nil then
		prevLengthWasAbove = false
	end
	-- Reset position
	if stick:Length() <= 0 then
		if prevLengthWasAbove then
			SimulateInput.SetMousePos(fromPos)
		end
		prevLengthWasAbove = false
		return
	end
	prevLengthWasAbove = true
	local x = fromPos:x() + stick:x() * xDistance
	local y = fromPos:y() - stick:y() * yDistance
	SimulateInput.SetMousePos(x, y)
end