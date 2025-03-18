#include "EveWorldSubsystem.h"
#include "EveLuaManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UEveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!(InWorld.GetName() == "DefaultMap")) return;

	EveLuaManager LuaManager;

	if (LuaManager.Initialize())
	{
		FString LuaScript;
		if (LoadLuaScript(FPaths::ProjectDir() + TEXT("Source/EveLua/Test/Scripts/EveLua.lua"), LuaScript))
		{
			if (LuaManager.ExecuteLuaScript(TCHAR_TO_UTF8(*LuaScript)))
			{
				UE_LOG(LogTemp, Display, TEXT("[UELog] Lua script executed successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[UELog] Failed to execute Lua script"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[UELog] Failed to load Lua script"));
		}

		LuaManager.Shutdown();
	}
}

bool UEveWorldSubsystem::LoadLuaScript(const FString& FilePath, FString& OutScript)
{
	return FFileHelper::LoadFileToString(OutScript, *FilePath);
}