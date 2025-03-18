// Copyright Night Gamer. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 添加 Lua 头文件包含
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/**
 * EveLuaManager 类，用于管理 Lua 状态和执行 Lua 脚本。
 */
class EveLuaManager
{
public:
	// 构造函数
	EveLuaManager();
	// 析构函数
	~EveLuaManager();

	/**
	 * 初始化 Lua 状态。
	 * @return 初始化成功返回 true，失败返回 false
	 */
	bool Initialize();
	/**
	 * 执行 Lua 脚本。
	 * @param Script 要执行的 Lua 脚本字符串
	 * @return 执行成功返回 true，失败返回 false
	 */
	bool ExecuteLuaScript(const char* Script);
	/**
	 * 关闭 Lua 状态。
	 */
	void Shutdown();

	/**
	 * 获取 Lua 状态。
	 * @return Lua 状态指针
	 */
	lua_State* GetLuaState() const
	{
		return LuaState;
	}

	/**
	 * Lua 的 Print 函数，用于在 UE 日志中输出信息。
	 * @param L Lua 状态指针
	 * @return 0
	 */
	static int Lua_Print(lua_State* L);
	/**
	 * Lua 的 CallMemberFunction 函数，用于调用 C++ 对象的成员函数。
	 * @param L Lua 状态指针
	 * @return 压入 Lua 栈的返回值数量
	 */
	static int Lua_CallMemberFunction(lua_State* L);
	/**
	 * Lua 的 SpawnActor 函数，用于在 UE 世界中生成 Actor。
	 * @param L Lua 状态指针
	 * @return 压入 Lua 栈的返回值数量
	 */
	static int Lua_SpawnActor(lua_State* L);

	/**
	 * 将 C++ 对象指针压入 Lua 栈。
	 * @param Object 要压入的 C++ 对象指针
	 */
	void PushObjectToLua(AActor* Object);

private:
	// Lua 状态指针
	lua_State* LuaState;
};    