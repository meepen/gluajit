hook = require "hook"

hook.Add("LibOpen", "DisAllowBase", function(state, type)
    return true
end, true)