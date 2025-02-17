#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// 添加 Lua 头文件包含
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "EveTest_LuaLib.generated.h"

UCLASS()
class EVELUA_API AEveTest_LuaLib : public AActor
{
	GENERATED_BODY()
    
public:    
	AEveTest_LuaLib();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;
	
	static int Lua_Print(lua_State* L);

	void UseLua();
};