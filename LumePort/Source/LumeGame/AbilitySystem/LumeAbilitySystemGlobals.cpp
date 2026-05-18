// Copyright Yikai Zhu.

#include "AbilitySystem/LumeAbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeAbilitySystemGlobals)

struct FGameplayEffectContext;

ULumeAbilitySystemGlobals::ULumeAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

FGameplayEffectContext* ULumeAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return Super::AllocGameplayEffectContext();
}
