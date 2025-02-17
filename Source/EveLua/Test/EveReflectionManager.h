#pragma once

#include "CoreMinimal.h"

// 成员变量信息结构体
struct FEveMemberVariableInfo {
	FName VariableName;
	UProperty* Property;
	
	FEveMemberVariableInfo(FName name, UProperty* prop) : VariableName(name), Property(prop) {}
};

// 反射管理类
class FEveReflectionManager {
public:
	void RegisterClass(UClass* Class);
	const TArray<FEveMemberVariableInfo>* GetClassVariables(UClass* Class) const;

private:
	TMap<UClass*, TArray<FEveMemberVariableInfo>> ClassVariableMap;
};

