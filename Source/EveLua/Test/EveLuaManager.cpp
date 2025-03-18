// Copyright Night Gamer. All Rights Reserved.

#include "EveLuaManager.h"
#include "EveLuaActor.h"
#include "Kismet/GameplayStatics.h"

// 构造函数
EveLuaManager::EveLuaManager()
    : LuaState(nullptr)
{
}

// 析构函数
EveLuaManager::~EveLuaManager()
{
    // 关闭 Lua 状态
    Shutdown();
}

// 初始化 Lua 状态
bool EveLuaManager::Initialize()
{
    // 创建新的 Lua 状态
    LuaState = luaL_newstate();
    if (!LuaState)
    {
        // 创建失败返回 false
        return false;
    }
    // 打开 Lua 标准库
    luaL_openlibs(LuaState);

    // 重定向 Lua 的 print 函数
    lua_register(LuaState, "Print", Lua_Print);
    lua_register(LuaState, "CallMemberFunction", Lua_CallMemberFunction);
    lua_register(LuaState, "SpawnActor", Lua_SpawnActor);

    return true;
}

// 执行 Lua 脚本
bool EveLuaManager::ExecuteLuaScript(const char* Script)
{
    if (!LuaState)
    {
        // Lua 状态未初始化，返回 false
        return false;
    }

    // 执行 Lua 脚本
    int result = luaL_dostring(LuaState, Script);
    if (result != LUA_OK)
    {
        // 获取错误信息
        const char* errorMsg = lua_tostring(LuaState, -1);
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Lua error: %s"), UTF8_TO_TCHAR(errorMsg));
        // 弹出错误信息
        lua_pop(LuaState, 1);
        return false;
    }
    return true;
}

// 关闭 Lua 状态
void EveLuaManager::Shutdown()
{
    if (LuaState)
    {
        // 关闭 Lua 状态
        lua_close(LuaState);
        LuaState = nullptr;
    }
}

// Lua 的 Print 函数实现
int EveLuaManager::Lua_Print(lua_State* L)
{
    // 获取栈上参数的数量
    int n = lua_gettop(L);
    FString Output;
    for (int i = 1; i <= n; i++)
    {
        if (i > 1) Output += " ";
        if (lua_isstring(L, i))
        {
            // 获取字符串参数
            const char* str = lua_tostring(L, i);
            Output += UTF8_TO_TCHAR(str);
        }
        else if (lua_isnumber(L, i))
        {
            // 获取数字参数
            lua_Number num = lua_tonumber(L, i);
            Output += FString::SanitizeFloat(num);
        }
        else if (lua_isboolean(L, i))
        {
            // 获取布尔参数
            Output += lua_toboolean(L, i) ? "true" : "false";
        }
        else
        {
            // 获取参数类型名称
            Output += lua_typename(L, lua_type(L, i));
        }
    }
    // 输出日志
    UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
    return 0;
}

// Lua 的 CallMemberFunction 函数实现
int EveLuaManager::Lua_CallMemberFunction(lua_State* L)
{
    // 获取 Lua 栈上的参数
    void* Object = lua_touserdata(L, 1); // 第一个参数是对象实例指针
    if (!Object)
    {
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Invalid object pointer passed to CallMemberFunction"));
        // 返回 nil 表示错误
        lua_pushnil(L);
        return 1;
    }

    const char* FunctionNameStr = lua_tostring(L, 2); // 第二个参数是函数名
    int NumParams = lua_gettop(L) - 2; // 剩余的参数是函数调用的参数

    // 获取对象的类
    AActor* Actor = static_cast<AActor*>(Object);
    if (!Actor)
    {
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Failed to cast object pointer to AActor"));
        // 返回 nil 表示错误
        lua_pushnil(L);
        return 1;
    }

    UClass* Class = Actor->GetClass();

    // 查找函数
    UFunction* Function = Class->FindFunctionByName(FName(FunctionNameStr));
    if (!Function)
    {
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Function %s not found in class %s"), UTF8_TO_TCHAR(FunctionNameStr), *Class->GetName());
        // 返回 nil 表示函数未找到
        lua_pushnil(L);
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
            UE_LOG(LogTemp, Log, TEXT("[Eve-Log-Cpp] Param %d: %d"), Index, *Param->ContainerPtrToValuePtr<int32>(Params));
            Index++;
        }
    }

    // 调用函数
    UE_LOG(LogTemp, Log, TEXT("[Eve-Log-Cpp] Calling Function: %s, NumParams: %d"), *Function->GetName(), NumParams);
    Actor->ProcessEvent(Function, Params);

    // 处理返回值
    FProperty* ReturnProp = Function->GetReturnProperty();
    if (ReturnProp)
    {
        if (ReturnProp->IsA(FIntProperty::StaticClass()))
        {
            int32 ReturnValue = *ReturnProp->ContainerPtrToValuePtr<int32>(Params);
            // 将返回值压入 Lua 栈
            lua_pushinteger(L, ReturnValue);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Unsupported return type: %s"), *ReturnProp->GetClass()->GetName());
            // 不支持的返回值类型，返回 nil
            lua_pushnil(L);
        }
    }
    else
    {
        // 函数没有返回值，返回 nil
        lua_pushnil(L);
    }

    // 释放参数内存
    FMemory::Free(Params);

    return 1; // 返回 1 表示有一个返回值压入了 Lua 栈
}

// Lua 的 SpawnActor 函数实现
int EveLuaManager::Lua_SpawnActor(lua_State* L)
{
    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] World is null"));
        lua_pushnil(L);
        return 1;
    }

    // 生成 AEveLuaActor
    AActor* NewActor = World->SpawnActor<AEveLuaActor>();
    if (!NewActor)
    {
        UE_LOG(LogTemp, Error, TEXT("[Eve-Log-Cpp] Failed to spawn AEveLuaActor"));
        lua_pushnil(L);
        return 1;
    }

    // 将 Actor 指针压入 Lua 栈
    lua_pushlightuserdata(L, NewActor);
    return 1;
}
