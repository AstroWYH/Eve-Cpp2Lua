#include "EveLuaManager.h"

#include "EveTest_LuaReflection.h"
#include "Logging/LogMacros.h"
#include "UObject/UnrealTypePrivate.h"

EveLuaManager::EveLuaManager()
	: LuaState(nullptr)
{
}

EveLuaManager::~EveLuaManager()
{
	Shutdown();
}

bool EveLuaManager::Initialize()
{
	LuaState = luaL_newstate();
	if (!LuaState)
	{
		return false;
	}
	luaL_openlibs(LuaState);

	// 重定向 Lua 的 print 函数
	lua_pushcfunction(LuaState, Lua_Print);
	lua_setglobal(LuaState, "print");

	lua_register(LuaState, "CallMemberFunction", LuaCallMemberFunction);

	// 注册 AEveTest_LuaReflection 类到反射管理类
	UClass* ReflectionClass = AEveTest_LuaReflection::StaticClass();
	ReflectionManager.RegisterClass(ReflectionClass);

	return true;
}

bool EveLuaManager::ExecuteLuaScript(const char* Script)
{
	if (!LuaState)
	{
		return false;
	}

	int result = luaL_dostring(LuaState, Script);
	if (result != LUA_OK)
	{
		const char* errorMsg = lua_tostring(LuaState, -1);
		UE_LOG(LogTemp, Error, TEXT("Lua error: %s"), UTF8_TO_TCHAR(errorMsg));
		lua_pop(LuaState, 1);
		return false;
	}
	return true;
}

void EveLuaManager::Shutdown()
{
	if (LuaState)
	{
		lua_close(LuaState);
		LuaState = nullptr;
	}
}

// 自定义 Lua print 函数，将输出重定向到 UE5 日志
int EveLuaManager::Lua_Print(lua_State* L)
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

int EveLuaManager::LuaCallMemberFunction(lua_State* L)
{
    // 获取 Lua 栈上的参数
    void* Object = lua_touserdata(L, 1); // 第一个参数是对象实例指针
    if (!Object)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid object pointer passed to CallMemberFunction"));
        lua_pushnil(L); // 返回 nil 表示错误
        return 1;
    }

    const char* FunctionNameStr = lua_tostring(L, 2); // 第二个参数是函数名
    int NumParams = lua_gettop(L) - 2; // 剩余的参数是函数调用的参数

    // 获取对象的类
    AActor* Actor = static_cast<AActor*>(Object);
    if (!Actor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast object pointer to AActor"));
        lua_pushnil(L); // 返回 nil 表示错误
        return 1;
    }

    UClass* Class = Actor->GetClass();

    // 查找函数
    UFunction* Function = Class->FindFunctionByName(FName(FunctionNameStr));
    if (!Function)
    {
        UE_LOG(LogTemp, Error, TEXT("Function %s not found in class %s"), UTF8_TO_TCHAR(FunctionNameStr), *Class->GetName());
        lua_pushnil(L); // 返回 nil 表示函数未找到
        return 1;
    }

    // 分配参数内存（确保对齐）
    void* Params = FMemory::Malloc(Function->ParmsSize, Function->GetMinAlignment());
    FMemory::Memzero(Params, Function->ParmsSize);

    // 填充参数
    int Index = 0;
    for (TFieldIterator<FProperty> It(Function); It; ++It)
    {
        FProperty* Param = *It;

        // 跳过返回值
        if (Param == Function->GetReturnProperty())
        {
            continue;
        }

        // 只处理输入参数
        if (Param->IsA(FIntProperty::StaticClass()) && Index < NumParams)
        {
            int32 Value = lua_tointeger(L, Index + 3);
            *Param->ContainerPtrToValuePtr<int32>(Params) = Value;
            UE_LOG(LogTemp, Log, TEXT("[wyh] Param %d: %d"), Index, *Param->ContainerPtrToValuePtr<int32>(Params));
            Index++;
        }
    }

    // 调用函数
    UE_LOG(LogTemp, Log, TEXT("Calling Function: %s, NumParams: %d"), *Function->GetName(), NumParams);
    Actor->ProcessEvent(Function, Params);

    // 打印 Params 内存的内容
    uint8* ParamsBytes = reinterpret_cast<uint8*>(Params);
    for (int32 i = 0; i < Function->ParmsSize; ++i)
    {
        UE_LOG(LogTemp, Log, TEXT("[wyh] Begin Params[%d] = %d"), i, ParamsBytes[i]);
    }

    // 处理返回值
    FProperty* ReturnProp = Function->GetReturnProperty();
    if (ReturnProp)
    {
        UE_LOG(LogTemp, Log, TEXT("[wyh] ReturnProp Name: %s, Type: %s"), *ReturnProp->GetName(), *ReturnProp->GetClass()->GetName());
        if (ReturnProp->IsA(FIntProperty::StaticClass()))
        {
            int32 ReturnValue = *ReturnProp->ContainerPtrToValuePtr<int32>(Params);
            UE_LOG(LogTemp, Log, TEXT("[wyh] Return Value: %d"), ReturnValue);
            lua_pushinteger(L, ReturnValue); // 将返回值压入 Lua 栈
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Unsupported return type: %s"), *ReturnProp->GetClass()->GetName());
            lua_pushnil(L); // 不支持的返回值类型，返回 nil
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No return value"));
        lua_pushnil(L); // 函数没有返回值，返回 nil
    }

	// 打印 Params 内存的内容
	uint8* FinalParamsBytes = reinterpret_cast<uint8*>(Params);
	for (int32 i = 0; i < Function->ParmsSize; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("[wyh] End Params[%d] = %d"), i, FinalParamsBytes[i]);
	}
	
    // 释放参数内存
    FMemory::Free(Params);

    return 1; // 返回 1 表示有一个返回值压入了 Lua 栈
}

void EveLuaManager::PushObjectToLua(AActor* Object)
{
	if (LuaState && Object)
	{
		// void* UserData = lua_newuserdata(LuaState, sizeof(AActor*));
		// *(AActor**)UserData = Object;

		lua_pushlightuserdata(LuaState, Object);
	}
}

// void EveLuaManager::PushObjectToLua(AActor* Object)
// {
// 	if (LuaState && Object)
// 	{
// 		// 使用 lua_newuserdata 分配内存并存储指针
// 		AActor** UserData = (AActor**)lua_newuserdata(LuaState, sizeof(AActor*));
// 		*UserData = Object;
//
// 		// 绑定元表（可选，用于类型检查）
// 		luaL_getmetatable(LuaState, "AActor");
// 		lua_setmetatable(LuaState, -2);
// 	}
// }