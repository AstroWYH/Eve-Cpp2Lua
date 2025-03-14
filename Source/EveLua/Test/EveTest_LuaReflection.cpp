// Copyright Night Gamer, Inc. All Rights Reserved.

#include "EveTest_LuaReflection.h"
#include "Kismet/KismetSystemLibrary.h"

// 构造函数实现
AEveTest_LuaReflection::AEveTest_LuaReflection()
{
	PrimaryActorTick.bCanEverTick = false;

	MyIntVariable = 0;
	MyStringVariable = TEXT("Default String");
}

// 打印消息函数实现
void AEveTest_LuaReflection::PrintMessage(const FString& Message)
{
	UKismetSystemLibrary::PrintString(this, Message);
}

// 加法函数实现
int32 AEveTest_LuaReflection::Add(int32 A, int32 B)
{
	return A + B;
}