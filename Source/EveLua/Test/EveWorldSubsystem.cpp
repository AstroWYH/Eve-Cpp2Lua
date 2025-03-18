// Copyright Night Gamer. All Rights Reserved.

#include "EveWorldSubsystem.h"
#include "EveLuaManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

// 世界开始播放时调用
void UEveWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// 只在 DefaultMap 中执行 Lua 脚本
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

		// 关闭 Lua 状态
		LuaManager.Shutdown();
	}
}

// 加载 Lua 脚本文件
bool UEveWorldSubsystem::LoadLuaScript(const FString& FilePath, FString& OutScript)
{
	// 使用 FileHelper 加载文件内容到字符串
	return FFileHelper::LoadFileToString(OutScript, *FilePath);
}    