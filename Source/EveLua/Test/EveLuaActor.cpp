// Copyright Night Gamer. All Rights Reserved.

#include "EveLuaActor.h"
#include "Kismet/KismetSystemLibrary.h"

// 构造函数实现
AEveLuaActor::AEveLuaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MyIntVariable = 0;
	MyStringVariable = TEXT("Default String");
}

// 打印消息函数实现
void AEveLuaActor::PrintMessage(const FString& Message)
{
	UKismetSystemLibrary::PrintString(this, Message);
}

// 加法函数实现
int32 AEveLuaActor::Add(int32 A, int32 B)
{
	return A + B;
}