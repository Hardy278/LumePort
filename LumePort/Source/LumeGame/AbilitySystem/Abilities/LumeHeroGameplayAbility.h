// Copyright Yikai Zhu.

#pragma once

#include "AbilitySystem/Abilities/LumeGameplayAbility.h"

#include "LumeHeroGameplayAbility.generated.h"

#define UE_API LUMEGAME_API

class ULumeHeroComponent;
class ALumeHeroCharacter;

/**
 * 
 */
UCLASS(MinimalAPI, Abstract, HideCategories = Input)
class ULumeHeroGameplayAbility : public ULumeGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lume|Ability")
	UE_API ALumePlayerController* GetLumePlayerControllerFromActorInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lume|Ability")
	UE_API ALumeHeroCharacter* GetLumeHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lume|Ability")
	UE_API ULumeHeroComponent* GetHeroComponentFromActorInfo();

private:

	TWeakObjectPtr<ALumeHeroCharacter> CatchedLumeHeroCharacter;
	TWeakObjectPtr<ALumePlayerController> CatchedLumePlayerController;
};

#undef UE_API