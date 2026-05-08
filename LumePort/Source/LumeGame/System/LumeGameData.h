// Copyright Yikai Zhu.

#pragma once

#include "Engine/DataAsset.h"

#include "LumeGameData.generated.h"

#define UE_API LUMEGAME_API

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
};

#undef UE_API