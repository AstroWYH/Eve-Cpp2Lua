// Copyright Night Gamer. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EveLuaActor.generated.h"

/**
 * AEveLuaActor 类，继承自 AActor，用于演示 Lua 反射调用 C++ 函数。
 */
UCLASS()
class EVELUA_API AEveLuaActor : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AEveLuaActor();

	/**
	 * 加法函数，可在蓝图中调用。
	 * @param A 第一个整数参数
	 * @param B 第二个整数参数
	 * @return 两个整数的和
	 */
	UFUNCTION(BlueprintCallable, Category = "LuaReflection")
	int32 Add(int32 A, int32 B);
};    