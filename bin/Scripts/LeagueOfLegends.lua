function Start()
	Utility = require("Scripts/Utility" )
	TimerManager = require("Scripts/Libraries/TimerManager")

	isBlueSide = Bind("isBlueSide", true, "ToggleBox")
	prevIsBlueSide = isBlueSide
	mouseDistFree = Bind("mouseDistFree", 190, "SliderFloat", 0, 600)
	mouseDistCentered = Bind("mouseDistCentered", 250, "SliderFloat", 0, 600)
	
	spellDistanceQ = Bind("spellDistanceQ", 150, "SliderFloat", 0, 600)
	spellDistanceW = Bind("spellDistanceW", 400, "SliderFloat", 0, 600)
	spellDistanceE = Bind("spellDistanceE", 130, "SliderFloat", 0, 600)
	spellDistanceR = Bind("spellDistanceR", 150, "SliderFloat", 0, 600)
	spellDistanceF = Bind("spellDistanceF", 250, "SliderFloat", 0, 600)

	targetSpeed = Bind("targetSpeed", 300, "SliderFloat", 100, 1000)
	mapSpeed = Bind("mapSpeed", 366, "SliderFloat", 100, 1000)

	shopMouseSpeed = 500
	 
	currentCenterPos = SimulateInput.GetMousePos()

	-- important to make sure the bindings work for any resolution
	leagueRect = GetWindowRect("League of Legends (TM) Client")
	minimapSize = Vector(0.138*leagueRect:Width(), 0.277*leagueRect:Height())
	mapCenter = Vector(0.9317*leagueRect:Width(), 0.8277*leagueRect:Height())
	mapMoveRange = Vector(0.09*leagueRect:Width(), 0.103*leagueRect:Height())
	leagueRectCenter = leagueRect:Center()
	
	-- Where on the screen the player is when the camera follows the player
	leagueCenterOffset = leagueRectCenter + Vector(-0.0364*leagueRect:Width(), -0.02777*leagueRect:Height())
	
	-- will represent a Vector where the centerpos wants to be
	targetPos = nil

	-- max mouse joystick distance
	currentDistance = mouseDistFree

	-- keeps time of when the currentDistance should be reset after a spell changed it
	distanceTimer = 0
	isShoppingMode = false
end

function lerp(a, b, t)
	return a + (b - a) * t
end

function UpdateSideOffsetValue()
	-- did we change the value from editor?
	if prevIsBlueSide ~= isBlueSide then
		ClearLog()
		if isBlueSide then
			print("side: blue")
			leagueCharacterOffset = Vector(-0.0365*leagueRect:Width(), -0.02777*leagueRect:Height())
		else
			print("side: not blue lul")
			leagueCharacterOffset = Vector(0.030*leagueRect:Width(), -0.119375*leagueRect:Height())
		end
		-- update the centerpoint
		leagueCenterOffset = leagueRectCenter + leagueCharacterOffset
	end
	prevIsBlueSide = isBlueSide
end

function Update(dt)
	TimerManager.Update(dt)
	UpdateSideOffsetValue()
	-- Start button reserved for mode changing
	if Input.ButtonPressed(Input.GAMEPAD_START) then
		isShoppingMode = not isShoppingMode
		ClearLog()
		if isShoppingMode then
			print("mode: shopping")
			-- toggle shop window
			SimulateInput.PressKey(Input.GAMEPAD_BACK, SimulateInput.KEY_P)
		else
			print("mode: gameplay")
			-- toggle shop window
			SimulateInput.PressKey(Input.GAMEPAD_BACK, SimulateInput.KEY_P)
		end
	end

	if isShoppingMode then
		ShoppingInput(dt)
	else
		GameplayInput(dt)
	end

	-- summoner spells & recalling will work any mode
	Utility.CheckPress(Input.GAMEPAD_LEFT_TRIGGER, SimulateInput.KEY_D)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_TRIGGER, SimulateInput.KEY_F)
end

function ShoppingInput(dt)
	-- mouse tap
	Utility.CheckPress(Input.GAMEPAD_LEFT_BUTTON, SimulateInput.MOUSE_LEFT)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_BUTTON, SimulateInput.MOUSE_LEFT)

	if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
		Utility.SetMousePosCenterOut(leagueCenterOffset, mouseDistCentered, mouseDistCentered, Input.GAMEPAD_LEFT_STICK)
	else
		Utility.MoveMouse(Input.GAMEPAD_LEFT_STICK, shopMouseSpeed*dt)
	end

	-- scrolling
	if Input.ButtonHeld(Input.GAMEPAD_JOYSTICK_UP, Input.GAMEPAD_RIGHT_STICK) then
		SimulateInput.ScrollMouse(10)
	end
	if Input.ButtonHeld(Input.GAMEPAD_JOYSTICK_DOWN, Input.GAMEPAD_RIGHT_STICK) then
		SimulateInput.ScrollMouse(-10)
	end
end

function GameplayInput(dt)
	-- recall
	Utility.CheckPress(Input.GAMEPAD_BACK, SimulateInput.KEY_B)

	-- left, right, up, down
	MoveMapArrows(dt)

	-- q, w, e, r
	if Input.ButtonHeld(Input.GAMEPAD_DPAD_LEFT) then
		LevelSpells()
	elseif Input.ButtonHeld(Input.GAMEPAD_DPAD_DOWN) then
		Items()
	elseif Input.ButtonHeld(Input.GAMEPAD_DPAD_RIGHT) then
		Emote()
	else
		Spells()
	end

	-- make sure the mouse isn't outside the league rect
	Utility.ClampPointInRect(currentCenterPos, leagueRect, 300, 300)

	-- right clicking, but repeat if held
	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_BUTTON) then
		-- update the target position so the center position is on the character 
		targetPos = SimulateInput.GetMousePos()

		-- initial right click
		SimulateInput.PressKey(SimulateInput.MOUSE_RIGHT)

		-- Continiously right click every 0,1 sec
		TimerManager.SetTimer(0.1, true
		, function(timerRef)
			if Input.ButtonHeld(Input.GAMEPAD_RIGHT_BUTTON) then
				targetPos = SimulateInput.GetMousePos()
				SimulateInput.PressKey(SimulateInput.MOUSE_RIGHT)
			else
				TimerManager.StopTimer(timerRef)
			end
		end)

	end

	-- centering the lol camera
	if Input.ButtonPressed(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.KeyDown(SimulateInput.KEY_SPACE)
		currentCenterPos = Vector(leagueCenterOffset:x(), leagueCenterOffset:y())
	elseif Input.ButtonReleased(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.KeyUp(SimulateInput.KEY_SPACE)

		-- TEST ADVANCED USER FEATURE
		currentCenterPos = Utility.GetPosCenterOut(currentCenterPos, currentDistance, currentDistance, Input.GAMEPAD_LEFT_STICK,0)
		targetPos = nil
	end

	-- if joystick is resting, AKA under trigger threshold, centerout won't move mouse, let's move it...
	if Input.ButtonPressed(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.SetMousePos(leagueCenterOffset)
	end

	if Input.ButtonPressed(Input.GAMEPAD_LEFT_BUTTON) then
		currentDistance = mouseDistCentered
	elseif Input.ButtonReleased(Input.GAMEPAD_LEFT_BUTTON) then
		currentDistance = mouseDistFree
	end
	
	local timeToHoldDistance = 0.3 
	if distanceTimer < timeToHoldDistance then
		distanceTimer = distanceTimer + dt
		if distanceTimer >= timeToHoldDistance then
			if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
				currentDistance = mouseDistCentered
			else
				currentDistance = mouseDistFree
			end
		end
	end

	-- update the currentDistance if we use a spell
	SpellDistanceModifier()

	if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
		Utility.SetMousePosCenterOutClamped(leagueCenterOffset, currentDistance, currentDistance, Input.GAMEPAD_LEFT_STICK, leagueRect, 20, 60)
	else	
		-- move centerPos towards target
		if targetPos ~= nil then
			local centerToTarget = targetPos - currentCenterPos
			local distToTarget = centerToTarget:Length()
			centerToTarget:Normalize()
			local moveVector = centerToTarget*dt*targetSpeed
			-- Close enough to the target, I know this is ugly
			if distToTarget > moveVector then
				currentCenterPos = targetPos
				targetPos = nil
			else
				currentCenterPos = currentCenterPos + moveVector
			end
		end
		local movedMouse = Utility.SetMousePosCenterOutClamped(currentCenterPos, currentDistance, currentDistance, Input.GAMEPAD_LEFT_STICK, leagueRect, 20, 60)
		if not movedMouse and targetPos ~= nil then
			SimulateInput.SetMousePos(currentCenterPos)
		end
	end

	-- Attack move
	if Input.ButtonPressed(Input.GAMEPAD_LEFT_THUMB) then
		SimulateInput.PressKey(SimulateInput.KEY_A)
	end

	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_THUMB) then
		SimulateInput.KeyDown(SimulateInput.MOUSE_LEFT)
	end
	-- MAP THUMB
	if Input.ButtonHeld(Input.GAMEPAD_RIGHT_THUMB) then
		local rightDownCorner = Vector(leagueRect:X() + leagueRect:Width(),leagueRect:Y() + leagueRect:Height())
		--local mapLeftUpCorner = rightDownCorner - Vector (265, 300)

		local mapLeftUpCorner = rightDownCorner - minimapSize
		local mapPos = Utility.GetPosCenterOut(mapCenter, mapMoveRange:x(), mapMoveRange:y(), Input.GAMEPAD_RIGHT_STICK, 0)

		-- clamp it baby
		mapPos:x(Utility.Clamp(mapPos:x(), mapLeftUpCorner:x()+20, rightDownCorner:x()-20))
		mapPos:y(Utility.Clamp(mapPos:y(), mapLeftUpCorner:y()+20, rightDownCorner:y()-20))

		SimulateInput.SetMousePos(mapPos)
	elseif Input.ButtonReleased(Input.GAMEPAD_RIGHT_THUMB) then
		SimulateInput.KeyUp(SimulateInput.MOUSE_LEFT)
		currentCenterPos = Vector(leagueCenterOffset:x(), leagueCenterOffset:y())
		SimulateInput.SetMousePos(currentCenterPos)		
	end
end

function Spells()
	Utility.CheckPress(Input.GAMEPAD_X, SimulateInput.KEY_Q)
	Utility.CheckPress(Input.GAMEPAD_A, SimulateInput.KEY_W)
	Utility.CheckPress(Input.GAMEPAD_B, SimulateInput.KEY_E)
	Utility.CheckPress(Input.GAMEPAD_Y, SimulateInput.KEY_R)
end

function LevelSpells()
	Utility.CheckPressMultiple(Input.GAMEPAD_X, SimulateInput.KEY_CTRL, SimulateInput.KEY_Q)
	Utility.CheckPressMultiple(Input.GAMEPAD_A, SimulateInput.KEY_CTRL, SimulateInput.KEY_W)
	Utility.CheckPressMultiple(Input.GAMEPAD_B, SimulateInput.KEY_CTRL, SimulateInput.KEY_E)
	Utility.CheckPressMultiple(Input.GAMEPAD_Y, SimulateInput.KEY_CTRL, SimulateInput.KEY_R)
end

function Emote()
	Utility.CheckPressMultiple(Input.GAMEPAD_X, SimulateInput.KEY_CTRL, SimulateInput.KEY_1)
	Utility.CheckPressMultiple(Input.GAMEPAD_A, SimulateInput.KEY_CTRL, SimulateInput.KEY_2)
	Utility.CheckPressMultiple(Input.GAMEPAD_B, SimulateInput.KEY_CTRL, SimulateInput.KEY_3)
	Utility.CheckPressMultiple(Input.GAMEPAD_Y, SimulateInput.KEY_CTRL, SimulateInput.KEY_4)
end

function Items()
	Utility.CheckPress(Input.GAMEPAD_X, SimulateInput.KEY_1)
	Utility.CheckPress(Input.GAMEPAD_A, SimulateInput.KEY_2)
	Utility.CheckPress(Input.GAMEPAD_B, SimulateInput.KEY_3)
	Utility.CheckPress(Input.GAMEPAD_Y, SimulateInput.KEY_4)
end

function SpellDistanceModifier()
	if Input.ButtonHeld(Input.GAMEPAD_X) then
		currentDistance = spellDistanceQ
		distanceTimer = 0
	end
	if Input.ButtonHeld(Input.GAMEPAD_A) then
		currentDistance = spellDistanceW
		distanceTimer = 0
	end
	if Input.ButtonHeld(Input.GAMEPAD_B) then
		currentDistance = spellDistanceE
		distanceTimer = 0
	end
	if Input.ButtonHeld(Input.GAMEPAD_Y) then
		currentDistance = spellDistanceR
		distanceTimer = 0
	end
	if Input.ButtonHeld(Input.GAMEPAD_RIGHT_TRIGGER) then
		currentDistance = spellDistanceF
		distanceTimer = 0
	end

	-- Range for placing ward
	if Input.ButtonHeld(Input.GAMEPAD_DPAD_DOWN) then
		currentDistance = 300
		distanceTimer = 0
	end
end

function MoveCenterPoint(x, y, dt)
	local xPos = currentCenterPos:x()+mapSpeed*dt*x
	local yPos = currentCenterPos:y()+mapSpeed*dt*y
	currentCenterPos:Set(xPos, yPos)
	SimulateInput.SetMousePos(currentCenterPos)
end

-- not only press the arrow keys, but move the center pos
-- example: if we move right, the center pos will move right aswell, like that point was a part of the world
function MoveMapArrows(dt)
	Utility.CheckPressEx(Input.GAMEPAD_JOYSTICK_LEFT, SimulateInput.KEY_LEFT, Input.GAMEPAD_RIGHT_STICK, function() MoveCenterPoint(1, 0, dt) end)
	Utility.CheckPressEx(Input.GAMEPAD_JOYSTICK_RIGHT, SimulateInput.KEY_RIGHT, Input.GAMEPAD_RIGHT_STICK, function() MoveCenterPoint(-1, 0, dt) end)
	Utility.CheckPressEx(Input.GAMEPAD_JOYSTICK_UP, SimulateInput.KEY_UP, Input.GAMEPAD_RIGHT_STICK, function() MoveCenterPoint(0, 1, dt) end)
	Utility.CheckPressEx(Input.GAMEPAD_JOYSTICK_DOWN, SimulateInput.KEY_DOWN, Input.GAMEPAD_RIGHT_STICK, function() MoveCenterPoint(0, -1, dt) end)
end