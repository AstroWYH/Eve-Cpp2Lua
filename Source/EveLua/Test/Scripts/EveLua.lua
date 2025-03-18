-- Copyright Night Gamer. All Rights Reserved.

-- 示例1
-- 调用 Lua 的 Print 函数，在 UE 日志中输出信息
Print("[Eve-Log-Lua] Hello from Lua!")

-- 示例2
-- 调用 Lua 的 SpawnActor 函数，生成一个 AEveLuaActor 实例
local actor = SpawnActor()
if actor then
    -- 调用 Lua 的 CallMemberFunction 函数，调用 AEveLuaActor 的 Add 函数
    local result = CallMemberFunction(actor, "Add", 2, 3)
    if result then
        -- 输出 Add 函数的返回值
        Print("[Eve-Log-Lua] Add result: " .. result)
    else
        -- 调用失败，输出错误信息
        Print("[Eve-Log-Lua] CallMemberFunction returned nil.")
    end
else
    -- 生成 Actor 失败，输出错误信息
    Print("[Eve-Log-Lua] Failed to spawn actor.")
end    