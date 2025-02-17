#include "EveTest_LuaLib.h"

AEveTest_LuaLib::AEveTest_LuaLib()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEveTest_LuaLib::BeginPlay()
{
	Super::BeginPlay();

	// UseLua();
}

void AEveTest_LuaLib::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 自定义 Lua print 函数，将输出重定向到 UE5 日志
int AEveTest_LuaLib::Lua_Print(lua_State* L)
{
	int n = lua_gettop(L); // 获取栈上参数的数量
	FString Output;
	for (int i = 1; i <= n; i++)
	{
		if (i > 1) Output += " ";
		if (lua_isstring(L, i))
		{
			const char* str = lua_tostring(L, i);
			Output += UTF8_TO_TCHAR(str);
		}
		else if (lua_isnumber(L, i))
		{
			lua_Number num = lua_tonumber(L, i);
			Output += FString::SanitizeFloat(num);
		}
		else if (lua_isboolean(L, i))
		{
			Output += lua_toboolean(L, i) ? "true" : "false";
		}
		else
		{
			Output += lua_typename(L, lua_type(L, i));
		}
	}
	UE_LOG(LogTemp, Display, TEXT("%s"), *Output);
	return 0;
}

void AEveTest_LuaLib::UseLua()
{
	lua_State* L = luaL_newstate();
	if (L)
	{
		luaL_openlibs(L);

		// 重定向 Lua 的 print 函数
		lua_pushcfunction(L, Lua_Print);
		lua_setglobal(L, "print");

		// 执行一个简单的 Lua 脚本
		const char* luaScript = "print('[wyh] Hello from Lua!')";
		int result = luaL_dostring(L, luaScript);
		if (result != LUA_OK)
		{
			const char* errorMsg = lua_tostring(L, -1);
			UE_LOG(LogTemp, Error, TEXT("Lua error: %s"), UTF8_TO_TCHAR(errorMsg));
			lua_pop(L, 1);
			UE_LOG(LogTemp, Display, TEXT("[wyh] Not OK"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("[wyh] OK"));
		}

		lua_close(L);
	}
}