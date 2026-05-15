// Copyright Yikai Zhu.

#pragma once

#include "ModularCharacter.h"

#include "LumeCharacter.generated.h"

#define UE_API LUMEGAME_API

class ULumePawnExtensionComponent;

/**
 * 
 */
UCLASS(Abstract, MinimalAPI, Config = Game, Meta = (ShortTooltip = "The base character pawn class used by this project."))
class ALumeCharacter : public AModularCharacter
{
	GENERATED_BODY()
	
public:

	UE_API ALumeCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	UE_API virtual void OnRep_Controller() override;
	UE_API virtual void OnRep_PlayerState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lume|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULumePawnExtensionComponent> PawnExtComponent;
};

#undef UE_API