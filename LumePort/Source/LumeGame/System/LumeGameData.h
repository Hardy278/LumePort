// Copyright Yikai Zhu.

#pragma once

#include "Engine/DataAsset.h"

#include "LumeGameData.generated.h"

#define UE_API LUMEGAME_API

class UGameplayEffect;
class UObject;

/**
 * 
 */
UCLASS(MinimalAPI, BlueprintType, Const, Meta = (DisplayName = "Lume Game Data", ShortTooltip = "Data asset containing global game data."))
class ULumeGameData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UE_API ULumeGameData();

	// Returns the loaded game data.
	static UE_API const ULumeGameData& Get();

public:

	// Gameplay effect used to apply damage.  Uses SetByCaller for the damage magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Damage Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> DamageGameplayEffect_SetByCaller;

	// Gameplay effect used to apply healing.  Uses SetByCaller for the healing magnitude.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects", meta = (DisplayName = "Heal Gameplay Effect (SetByCaller)"))
	TSoftClassPtr<UGameplayEffect> HealGameplayEffect_SetByCaller;

	// Gameplay effect used to add and remove dynamic tags.
	UPROPERTY(EditDefaultsOnly, Category = "Default Gameplay Effects")
	TSoftClassPtr<UGameplayEffect> DynamicTagGameplayEffect;
};

#undef UE_API