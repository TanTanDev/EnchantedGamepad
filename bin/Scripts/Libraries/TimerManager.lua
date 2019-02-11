local TimerData = {}
TimerData.__index = TimerData

function TimerData:New(timerRef, time, loop, callback)
   local newTimerData = {}
   setmetatable(newTimerData,TimerData)

   -- default param
   loop = loop or false

   newTimerData.TimerRef = timerRef
   newTimerData.DoneTime = time
   newTimerData.TimerValue = 0
   newTimerData.Loop = loop
   newTimerData.Callback = callback
   return newTimerData
end

-- return: should remove timer?
function TimerData:Update(dt)
	self.TimerValue = self.TimerValue + dt
	if self.TimerValue  >= self.DoneTime then
		--print("callback called")
		self.Callback(self.TimerRef)
		if self.Loop then
			self.TimerValue = self.TimerValue - self.TimerValue
			return false
		end
		return true
	end
	return false
end

local TimerManager = {}

TimerManager.Timers = {}
TimerManager.RefCount = 0

function TimerManager.Update(dt)
	for i = #TimerManager.Timers-1, 0, -1 do
		local timer = TimerManager.Timers[i+1]
		local shouldRemove = timer:Update(dt)
		if shouldRemove then
			table.remove(TimerManager.Timers, i)
		end
	end
end

-- start a timer, run callback when done
function TimerManager.SetTimer(time, loop, callback)
	TimerManager.RefCount = TimerManager.RefCount + 1
	local timerRef = TimerManager.RefCount
	local newTimer = TimerData:New(timerRef, time, loop, callback)
	table.insert(TimerManager.Timers, newTimer)
end

function TimerManager.StopTimer(timerRef)
	for i, timer in ipairs(TimerManager.Timers) do
		if timer.TimerRef == timerRef then
			table.remove(TimerManager.Timers, i)
			return
		end
	end
end

return TimerManager