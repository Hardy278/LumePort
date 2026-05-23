// Copyright Yikai Zhu.

#pragma once

#include "CommonGameInstance.h"
#include "LumeGameInstance.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game)
class ULumeGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
	
public:

	UE_API ULumeGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};

#undef UE_API