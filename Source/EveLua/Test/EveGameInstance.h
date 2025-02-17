// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EVELUA_API UEveGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
};