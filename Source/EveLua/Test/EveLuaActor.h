// Copyright Night Gamer. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EveLuaActor.generated.h"

UCLASS()
class EVELUA_API AEveLuaActor : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AEveLuaActor();

	UFUNCTION(BlueprintCallable, Category = "LuaReflection")
	int32 Add(int32 A, int32 B);
};