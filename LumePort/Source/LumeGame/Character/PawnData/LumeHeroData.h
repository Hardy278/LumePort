// Copyright Yikai Zhu.

#pragma once

#include "Character/PawnData/LumePawnData.h"
#include "GameplayTagContainer.h"

#include "LumeHeroData.generated.h"

#define UE_API LUMEGAME_API

class ULumeInputConfig;

/**
 * 
 */
UCLASS(MinimalAPI, Const, Meta = (DisplayName = "Lume Hero Data", ShortTooltip = "Data asset used to define a HeroCharacter."))
class ULumeHeroData : public ULumePawnData
{
	GENERATED_BODY()

public:

	UE_API ULumeHeroData(const FObjectInitializer& ObjectInitializer);

public:

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lume|Input")
	TObjectPtr<ULumeInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lume|Camera")
	FGameplayTag CameraMode;
};

#undef UE_API