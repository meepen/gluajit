local hook = {
    table = {},
    rotable = {}
}
local LuaState = require "LuaState"

function hook:GetTable()
    return self.rotable, self.table
end

function hook:Add(event, identifier, func, returnallowed)
    local tbl
    if (returnallowed) then
        tbl = select(2, self:GetTable())
    else
        tbl = self:GetTable()
    end
    if (not tbl[event]) then
        tbl[event] = {}
    end
    tbl[event][identifier] = func
end

local function pk(...)
    return {n = select("#", ...), ...}
end
local function unpk(t)
    return unpack(t, 1, t.n)
end

local function handle_call(...)
    if (select("#", ...) > 0) then
        return true, pk(...)
    end
    return false
end

function hook:Call(event, stateaddr, ...)
    local roevents, events = self:GetTable()
    roevents, events = roevents[event], events[event]
    if (roevents) then
        for k,v in pairs(roevents) do
            v(state, ...)
        end
    end
    if (events) then
        for k,v in pairs(events) do
            local ret, rets = handle_call(v(state, ...))
            if (ret) then
                return unpk(rets)
            end
        end
    end
end

return {
    Call = function(...)
        return hook:Call(...)
    end,
    Add = function(...)
        return hook:Add(...)
    end,
    GetTable = function()
        return hook:GetTable()
    end,
    _INTERNAL = hook
}