// Copyright Yikai Zhu.

#pragma once

#include "AbilitySystemGlobals.h"

#include "LumeAbilitySystemGlobals.generated.h"

class UObject;
struct FGameplayEffectContext;

/**
 * 
 */
UCLASS()
class ULumeAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_UCLASS_BODY()
	
	//~UAbilitySystemGlobals interface
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	//~End of UAbilitySystemGlobals interface
};
