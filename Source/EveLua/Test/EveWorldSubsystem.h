#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EveWorldSubsystem.generated.h"

UCLASS()
class EVELUA_API UEveWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	bool LoadLuaScript(const FString& FilePath, FString& OutScript);
};