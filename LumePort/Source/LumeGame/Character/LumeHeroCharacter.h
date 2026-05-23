// Copyright Yikai Zhu.

#pragma once

#include "Character/LumeCharacter.h"

#include "LumeHeroCharacter.generated.h"

#define UE_API LUMEGAME_API

class USpringArmComponent;
class UCameraComponent;
class ULumeHeroComponent;

/**
 * 
 */
UCLASS(Abstract, MinimalAPI, Config = Game)
class ALumeHeroCharacter : public ALumeCharacter
{
	GENERATED_BODY()
	
public:
	
	UE_API ALumeHeroCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	UE_API virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lume|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULumeHeroComponent> HeroComponent;
};

#undef UE_API