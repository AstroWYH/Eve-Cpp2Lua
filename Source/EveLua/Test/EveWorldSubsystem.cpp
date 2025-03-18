// Copyright Night Gamer. All Rights Reserved.

#include "EveWorldSubsystem.h"

#include "EveLuaManager.h"

#include "EveLuaActor.h"

void UEveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!(InWorld.GetName() == "DefaultMap")) return;

	EveLuaManager LuaManager;

	if (LuaManager.Initialize())
	{
		// 示例1
		const char* LuaScript = "Print('[LuaLog] Hello from Lua!')";
		if (LuaManager.ExecuteLuaScript(LuaScript))
		{
			UE_LOG(LogTemp, Display, TEXT("[UELog] OK"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("[UELog] Not OK"));
		}

		// 示例2
		// 创建 AEveLuaActor 类的实例
		UWorld* World = GetWorld();
		if (World)
		{
			AEveLuaActor* ReflectionInstance = World->SpawnActor<AEveLuaActor>();
			if (ReflectionInstance)
			{
				LuaManager.PushObjectToLua(ReflectionInstance);
				lua_setglobal(LuaManager.GetLuaState(), "reflectionInstance");

				const char* LuaScriptObj = R"(
					if reflectionInstance then
					    local result = CallMemberFunction(reflectionInstance, "Add", 2, 3)
					    if result then
					        print("[LuaLog] Add result: " .. result)
					    else
					        print("[LuaLog] CallMemberFunction returned nil.")
					    end
					else
					    print("[LuaLog] reflectionInstance is nil.")
					end
                )";

				if (LuaManager.ExecuteLuaScript(LuaScriptObj))
				{
					UE_LOG(LogTemp, Display, TEXT("[UELog] OK"));
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("[UELog] Not OK"));
				}
			}
		}

		LuaManager.Shutdown();
	}
}