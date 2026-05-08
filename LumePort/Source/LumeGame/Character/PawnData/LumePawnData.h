// Copyright Yikai Zhu.

#pragma once

#include "Engine/DataAsset.h"

#include "LumePawnData.generated.h"

#define UE_API LUMEGAME_API

class APawn;
class ULumeAbilitySet;
class ULumeAbilityTagRelationshipMapping;

/**
 * 
 */
UCLASS(MinimalAPI, Const, Meta = (DisplayName = "Lume Pawn Data", ShortTooltip = "Basic data asset used to define a Pawn."))
class ULumePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UE_API ULumePawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from ALumePawn or ALumeCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lume|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lume|Abilities")
	TArray<TObjectPtr<ULumeAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Lume|Abilities")
	TObjectPtr<ULumeAbilityTagRelationshipMapping> TagRelationshipMapping;
};

#undef UE_API