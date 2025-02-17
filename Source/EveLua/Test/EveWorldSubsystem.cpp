// Fill out your copyright notice in the Description page of Project Settings.


#include "EveWorldSubsystem.h"

#include "EveTest_LuaLib.h"
#include "EveLuaManager.h"

#include "EveTest_LuaReflection.h"

void UEveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!(InWorld.GetName() == "DefaultMap")) return;
	
	EveLuaManager LuaManager;

	if (LuaManager.Initialize())
	{
		// 示例1
		// const char* LuaScript = "print('[wyh] Hello from Lua!')";
		// if (LuaManager.ExecuteLuaScript(LuaScript))
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("[wyh] OK"));
		// }
		// else
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("[wyh] Not OK"));
		// }

		// 示例2
		// 创建 AEveTest_LuaReflection 类的实例
		UWorld* World = GetWorld();
		if (World)
		{
			AEveTest_LuaReflection* ReflectionInstance = World->SpawnActor<AEveTest_LuaReflection>();
			if (ReflectionInstance)
			{
				LuaManager.PushObjectToLua(ReflectionInstance);
				lua_setglobal(LuaManager.GetLuaState(), "reflectionInstance");

				const char* LuaScriptObj = R"(
					if reflectionInstance then
					    local result = CallMemberFunction(reflectionInstance, "Add", 2, 3)
					    if result then
					        print("[wyh] Add result: " .. result)
					    else
					        print("[wyh] CallMemberFunction returned nil.")
					    end
					else
					    print("[wyh] reflectionInstance is nil.")
					end
                )";

				if (LuaManager.ExecuteLuaScript(LuaScriptObj))
				{
					UE_LOG(LogTemp, Display, TEXT("[wyh] OK"));
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("[wyh] Not OK"));
				}
			}
		}

		LuaManager.Shutdown();
	}
}