// Copyright Night Gamer. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EveWorldSubsystem.generated.h"

/**
 * UEveWorldSubsystem 类，继承自 UWorldSubsystem，用于在世界开始播放时执行 Lua 脚本。
 */
UCLASS()
class EVELUA_API UEveWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * 世界开始播放时调用。
	 * @param InWorld 当前世界引用
	 */
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	/**
	 * 加载 Lua 脚本文件。
	 * @param FilePath 脚本文件路径
	 * @param OutScript 输出的脚本内容
	 * @return 加载成功返回 true，失败返回 false
	 */
	bool LoadLuaScript(const FString& FilePath, FString& OutScript);
};    