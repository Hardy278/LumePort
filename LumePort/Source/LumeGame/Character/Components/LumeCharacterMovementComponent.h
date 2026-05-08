// Copyright Yikai Zhu.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"

#include "LumeCharacterMovementComponent.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game)
class ULumeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UE_API ULumeCharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	
protected:

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;
};

#undef UE_API