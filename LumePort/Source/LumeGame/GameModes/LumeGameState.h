// Copyright Yikai Zhu.

#pragma once

#include "ModularGameState.h"

#include "LumeGameState.generated.h"

#define UE_API LUMEGAME_API

class APlayerState;
class UAbilitySystemComponent;
class UObject;

class ULumeExperienceManagerComponent;

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game)
class ALumeGameState : public AModularGameStateBase
{
	GENERATED_BODY()
	
public:

	UE_API ALumeGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	// Handles loading and managing the current gameplay experience
	UPROPERTY()
	TObjectPtr<ULumeExperienceManagerComponent> ExperienceManagerComponent;
};

#undef UE_API