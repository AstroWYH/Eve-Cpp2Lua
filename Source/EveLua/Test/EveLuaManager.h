// Copyright Night Gamer. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// 添加 Lua 头文件包含
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class EveLuaManager
{
public:
	EveLuaManager();
	~EveLuaManager();

	bool Initialize();
	bool ExecuteLuaScript(const char* Script);
	void Shutdown();

	lua_State* GetLuaState() const
	{
		return LuaState;
	}

	static int Lua_Print(lua_State* L);
	static int LuaCallMemberFunction(lua_State* L);

	void PushObjectToLua(AActor* Object);

private:
	lua_State* LuaState;

};