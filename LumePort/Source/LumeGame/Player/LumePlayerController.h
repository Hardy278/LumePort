// Copyright Yikai Zhu.

#pragma once

#include "CommonPlayerController.h"

#include "LumePlayerController.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class ALumePlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:

	UE_API ALumePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	UE_API virtual void BeginPlay() override;
	//~End of AActor interface
};

#undef UE_API