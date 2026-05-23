// Copyright Yikai Zhu.

#pragma once

#include "AbilitySystem/Abilities/LumeGameplayAbility.h"

#include "LumeEnemyGameplayAbility.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(MinimalAPI, Abstract, HideCategories = Input)
class ULumeEnemyGameplayAbility : public ULumeGameplayAbility
{
	GENERATED_BODY()
	
};

#undef UE_API