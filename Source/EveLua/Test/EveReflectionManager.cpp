#include "EveReflectionManager.h"
#include "UObject/UnrealTypePrivate.h"

void FEveReflectionManager::RegisterClass(UClass* Class) {
	if (Class) {
		TArray<FEveMemberVariableInfo>& Variables = ClassVariableMap.FindOrAdd(Class);
		for (TFieldIterator<UProperty> It(Class); It; ++It) {
			UProperty* Property = *It;
			Variables.Emplace(Property->GetFName(), Property);
		}
	}
}

const TArray<FEveMemberVariableInfo>* FEveReflectionManager::GetClassVariables(UClass* Class) const {
	return ClassVariableMap.Find(Class);
}
