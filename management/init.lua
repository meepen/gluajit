hook = require "hook"

hook.Add("LibOpen", "DisAllowBase", function(state, type)
    return type ~= 3
end, true)