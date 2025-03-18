-- 示例1
Print("[LuaLog] Hello from Lua!")

-- 示例2
local actor = SpawnActor()
if actor then
    local result = CallMemberFunction(actor, "Add", 2, 3)
    if result then
        Print("[LuaLog] Add result: " .. result)
    else
        Print("[LuaLog] CallMemberFunction returned nil.")
    end
else
    Print("[LuaLog] Failed to spawn actor.")
end