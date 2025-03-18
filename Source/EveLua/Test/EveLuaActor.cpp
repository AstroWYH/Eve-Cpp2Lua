// Copyright Night Gamer. All Rights Reserved.

#include "EveLuaActor.h"
#include "Kismet/KismetSystemLibrary.h"

// 构造函数实现
AEveLuaActor::AEveLuaActor()
{
	// 设置该 Actor 不需要每帧 Tick
	PrimaryActorTick.bCanEverTick = false;
}

// 加法函数实现
int32 AEveLuaActor::Add(int32 A, int32 B)
{
	// 返回两个整数的和
	return A + B;
}    