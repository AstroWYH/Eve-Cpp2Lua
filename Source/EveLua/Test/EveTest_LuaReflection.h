#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EveTest_LuaReflection.generated.h"

UCLASS()
class EVELUA_API AEveTest_LuaReflection : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数
	AEveTest_LuaReflection();

	// 成员变量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LuaReflection")
	int32 MyIntVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LuaReflection")
	FString MyStringVariable;

	// 成员函数
	UFUNCTION(BlueprintCallable, Category = "LuaReflection")
	void PrintMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "LuaReflection")
	int32 Add(int32 A, int32 B);
};