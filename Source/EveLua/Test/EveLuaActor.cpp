// Copyright Night Gamer. All Rights Reserved.

#include "EveLuaActor.h"
#include "Kismet/KismetSystemLibrary.h"

// 构造函数实现
AEveLuaActor::AEveLuaActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

// 加法函数实现
int32 AEveLuaActor::Add(int32 A, int32 B)
{
	return A + B;
}