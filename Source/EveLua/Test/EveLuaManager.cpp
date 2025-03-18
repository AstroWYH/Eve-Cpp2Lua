#include "EveLuaManager.h"
#include "EveLuaActor.h"
#include "Kismet/GameplayStatics.h"

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
    lua_register(LuaState, "Print", Lua_Print);
    lua_register(LuaState, "CallMemberFunction", Lua_CallMemberFunction);
    lua_register(LuaState, "SpawnActor", Lua_SpawnActor);

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
        UE_LOG(LogTemp, Error, TEXT("[UELog] Lua error: %s"), UTF8_TO_TCHAR(errorMsg));
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
    UE_LOG(LogTemp, Display, TEXT("[UELog] %s"), *Output);
    return 0;
}

int EveLuaManager::Lua_CallMemberFunction(lua_State* L)
{
    // 获取 Lua 栈上的参数
    void* Object = lua_touserdata(L, 1); // 第一个参数是对象实例指针
    if (!Object)
    {
        UE_LOG(LogTemp, Error, TEXT("[UELog] Invalid object pointer passed to CallMemberFunction"));
        lua_pushnil(L); // 返回 nil 表示错误
        return 1;
    }

    const char* FunctionNameStr = lua_tostring(L, 2); // 第二个参数是函数名
    int NumParams = lua_gettop(L) - 2; // 剩余的参数是函数调用的参数

    // 获取对象的类
    AActor* Actor = static_cast<AActor*>(Object);
    if (!Actor)
    {
        UE_LOG(LogTemp, Error, TEXT("[UELog] Failed to cast object pointer to AActor"));
        lua_pushnil(L); // 返回 nil 表示错误
        return 1;
    }

    UClass* Class = Actor->GetClass();

    // 查找函数
    UFunction* Function = Class->FindFunctionByName(FName(FunctionNameStr));
    if (!Function)
    {
        UE_LOG(LogTemp, Error, TEXT("[UELog] Function %s not found in class %s"), UTF8_TO_TCHAR(FunctionNameStr), *Class->GetName());
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
            UE_LOG(LogTemp, Log, TEXT("[UELog] Param %d: %d"), Index, *Param->ContainerPtrToValuePtr<int32>(Params));
            Index++;
        }
    }

    // 调用函数
    UE_LOG(LogTemp, Log, TEXT("[UELog] Calling Function: %s, NumParams: %d"), *Function->GetName(), NumParams);
    Actor->ProcessEvent(Function, Params);

    // 处理返回值
    FProperty* ReturnProp = Function->GetReturnProperty();
    if (ReturnProp)
    {
        if (ReturnProp->IsA(FIntProperty::StaticClass()))
        {
            int32 ReturnValue = *ReturnProp->ContainerPtrToValuePtr<int32>(Params);
            lua_pushinteger(L, ReturnValue); // 将返回值压入 Lua 栈
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[UELog] Unsupported return type: %s"), *ReturnProp->GetClass()->GetName());
            lua_pushnil(L); // 不支持的返回值类型，返回 nil
        }
    }
    else
    {
        lua_pushnil(L); // 函数没有返回值，返回 nil
    }

    // 释放参数内存
    FMemory::Free(Params);

    return 1; // 返回 1 表示有一个返回值压入了 Lua 栈
}

int EveLuaManager::Lua_SpawnActor(lua_State* L)
{
    UWorld* World = GWorld;
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[UELog] World is null"));
        lua_pushnil(L);
        return 1;
    }

    AActor* NewActor = World->SpawnActor<AEveLuaActor>();
    if (!NewActor)
    {
        UE_LOG(LogTemp, Error, TEXT("[UELog] Failed to spawn AEveLuaActor"));
        lua_pushnil(L);
        return 1;
    }

    lua_pushlightuserdata(L, NewActor);
    return 1;
}

void EveLuaManager::PushObjectToLua(AActor* Object)
{
    if (LuaState && Object)
    {
        lua_pushlightuserdata(LuaState, Object);
    }
}