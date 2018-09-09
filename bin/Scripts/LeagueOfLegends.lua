function Start()
	Utility = require("Scripts/Utility" )
	TimerManager = require("Scripts/Libraries/TimerManager")
	isBlueSide = Bind("isBlueSide", true, "ToggleBox")
	prevIsBlueSide = isBlueSide
	dist = Bind("dist", 190, "SliderFloat", 0, 200)
	
	spellDistanceQ = Bind("spellDistanceQ", 150, "SliderFloat", 0, 600)
	spellDistanceW = Bind("spellDistanceW", 400, "SliderFloat", 0, 600)
	spellDistanceE = Bind("spellDistanceE", 130, "SliderFloat", 0, 600)
	spellDistanceR = Bind("spellDistanceR", 150, "SliderFloat", 0, 600)
	
	spellDistanceF = Bind("spellDistanceR", 250, "SliderFloat", 0, 600)
	 
	-- JARVAN SETTINGS
	--spellDistanceQ = Bind("spellDistanceQ", 450, "SliderFloat",0,600)
	--spellDistanceW = Bind("spellDistanceW", 300, "SliderFloat",0,600)
	--spellDistanceE = Bind("spellDistanceE", 500, "SliderFloat",0,600)
	--spellDistanceR = Bind("spellDistanceR", 300, "SliderFloat",0,600)
	
	--fromX = 400
	--fromY = 400
	--centerX = Bind("centerX", 400, "SliderFloat", 0, 1800)
	--centerY = Bind("centerY", 400, "SliderFloat", 0, 1280)
	--mouseSpeed = Bind("mouseSpeed", 0.1,"SliderFloat",0,3)
	targetSpeed = Bind("targetSpeed", 300,"SliderFloat",100,1000)
	mapSpeed = Bind("mapSpeed", 366,"SliderFloat",100,1000)
	--daBool = Bind("daBool", true)
	
	currentCenterPos = SimulateInput.GetMousePos()
	targetPos = nil
	leagueRect = GetWindowRect("League of Legends (TM) Client")
	leagueRectCenter = leagueRect:Center()
	
	-- TOP SIDE OFFSET
	--leagueCharacterOffset = Vector(50, -120)
	
	-- blue side offset
	leagueCharacterOffset = Vector(-70, -30)
	
	leagueCenterOffset = leagueRectCenter + leagueCharacterOffset
	
	-- initialize global
	currentDistance = 100
	
	distanceTimer = 0
	
	isShoppingMode = false
	ClearLog()
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
			leagueCharacterOffset = Vector(-70, -30)
		else
			print("side: not blue lul")
			leagueCharacterOffset = Vector(50, -120)
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
	-- always no matter what mode
	-- summoner spells, recalling
	Utility.CheckPress(Input.GAMEPAD_LEFT_TRIGGER, SimulateInput.KEY_D)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_TRIGGER, SimulateInput.KEY_F)
end

function ShoppingInput(dt)
	--Utility.CheckPress(Input.GAMEPAD_BACK, SimulateInput.KEY_P)
	Utility.CheckPress(Input.GAMEPAD_LEFT_BUTTON, SimulateInput.MOUSE_LEFT)
	Utility.CheckPress(Input.GAMEPAD_RIGHT_BUTTON, SimulateInput.MOUSE_LEFT)

	if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
		Utility.SetMousePosCenterOut(leagueCenterOffset, 250, 250, Input.GAMEPAD_LEFT_STICK)
	else
		local mouseSpeed = 500
		Utility.MoveMouse(Input.GAMEPAD_LEFT_STICK, mouseSpeed*dt)
	end

	if Input.ButtonHeld(Input.GAMEPAD_JOYSTICK_UP, Input.GAMEPAD_RIGHT_STICK) then
		SimulateInput.ScrollMouse(10)
	end
	if Input.ButtonHeld(Input.GAMEPAD_JOYSTICK_DOWN, Input.GAMEPAD_RIGHT_STICK) then
		SimulateInput.ScrollMouse(-10)
	end
end

function GameplayInput(dt)
	--ClearLog()
	Utility.CheckPress(Input.GAMEPAD_BACK, SimulateInput.KEY_B)
	local distTimeHODL = 0.3 
	if distanceTimer < distTimeHODL then
		distanceTimer = distanceTimer + dt
		if distanceTimer >= distTimeHODL then
			currentDistance = dist
		end
	end
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
	SpellDistanceModifier()

	-- TODO: see if this works
	Utility.ClampPointInRect(currentCenterPos, leagueRect, 300, 300)
	-- clamp it baby
--	currentCenterPos:x(math.max(currentCenterPos:x(), leagueRect:X()+300))
--	currentCenterPos:x(math.min(currentCenterPos:x(), leagueRect:X()+leagueRect:Width()-300))
--	currentCenterPos:y(math.max(currentCenterPos:y(), leagueRect:Y()+300))
--	currentCenterPos:y(math.min(currentCenterPos:y(), leagueRect:Y()+leagueRect:Height()-300))

	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_BUTTON) then
		targetPos = SimulateInput.GetMousePos()
		SimulateInput.PressKey(SimulateInput.MOUSE_RIGHT)

		TimerManager.SetTimer(0.1, true
		,function(timerRef)
			if Input.ButtonHeld(Input.GAMEPAD_RIGHT_BUTTON) then
				targetPos = SimulateInput.GetMousePos()
				SimulateInput.PressKey(SimulateInput.MOUSE_RIGHT)
			else
				TimerManager.StopTimer(timerRef)
			end
		end)

	end

	if Input.ButtonPressed(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.KeyDown(SimulateInput.KEY_SPACE)
		currentCenterPos = Vector(leagueCenterOffset:x(), leagueCenterOffset:y())
	elseif Input.ButtonReleased(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.KeyUp(SimulateInput.KEY_SPACE)
	end

	-- if joystick is resting, AKA under trigger threshold, centerout won't move mouse, let's move it...
	if Input.ButtonPressed(Input.GAMEPAD_LEFT_BUTTON) then
		SimulateInput.SetMousePos(leagueCenterOffset)
	end

	if Input.ButtonHeld(Input.GAMEPAD_LEFT_BUTTON) then
		Utility.SetMousePosCenterOut(leagueCenterOffset, currentDistance, currentDistance, Input.GAMEPAD_LEFT_STICK)
	else
		Utility.SetMousePosCenterOut(currentCenterPos, currentDistance, currentDistance, Input.GAMEPAD_LEFT_STICK)
	
		local targetSpeed = 300
		-- move centerPos towards target
		if targetPos ~= nil then
			local centerToTarget = targetPos - currentCenterPos
			local distToTarget = centerToTarget:Length()
			centerToTarget:Normalize()
			local moveVector = centerToTarget*dt*targetSpeed
			if moveVector:Length() > distToTarget then
				currentCenterPos = targetPos
				targetPos = nil
			else
				currentCenterPos = currentCenterPos + moveVector
			end
			-- clamp it so we don't overshoot
		end
	end
	
	if Input.ButtonPressed(Input.GAMEPAD_RIGHT_THUMB) then	
		SimulateInput.KeyDown(SimulateInput.MOUSE_LEFT)
	elseif Input.ButtonReleased(Input.GAMEPAD_RIGHT_THUMB) then
		SimulateInput.KeyUp(SimulateInput.MOUSE_LEFT)
		currentCenterPos = Vector(leagueCenterOffset:x(), leagueCenterOffset:y())
		SimulateInput.SetMousePos(currentCenterPos)
	end

	if Input.ButtonPressed(Input.GAMEPAD_LEFT_THUMB) then
		SimulateInput.PressKey(SimulateInput.KEY_A)
	end

	if Input.ButtonHeld(Input.GAMEPAD_RIGHT_THUMB) then
		local rightDownCorner = Vector(leagueRect:X() + leagueRect:Width(),leagueRect:Y() + leagueRect:Height())
		local mapLeftUpCorner = rightDownCorner - Vector(200, 200)
		local centerMap = rightDownCorner - Vector(100,100)
		local finalPos = Utility.GetPosCenterOut(centerMap, 100, 100, Input.GAMEPAD_RIGHT_STICK, 0)

		-- clamp it baby
		finalPos:x(Utility.Clamp(finalPos:x(), mapLeftUpCorner:x()+20, rightDownCorner:x()-20))
		finalPos:y(Utility.Clamp(finalPos:y(), mapLeftUpCorner:y()+20, rightDownCorner:y()-20))

		SimulateInput.SetMousePos(finalPos)
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