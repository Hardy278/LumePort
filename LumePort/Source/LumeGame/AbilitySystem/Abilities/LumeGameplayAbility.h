// Copyright Yikai Zhu.

#pragma once

#include "Abilities/GameplayAbility.h"

#include "LumeGameplayAbility.generated.h"

#define UE_API LUMEGAME_API

class ALumeCharacter;
class ALumePlayerController;
class ULumeAbilityCost;
class ULumeAbilitySystemComponent;

struct FGameplayAbilityActorInfo;
struct FGameplayEffectSpec;
struct FGameplayEventData;

/**
 * 
 */
UCLASS(MinimalAPI, Abstract, HideCategories = Input, Meta = (ShortTooltip = "The base gameplay ability class used by this project."))
class ULumeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	friend class ULumeAbilitySystemComponent;

public:

	UE_API ULumeGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

#undef UE_API