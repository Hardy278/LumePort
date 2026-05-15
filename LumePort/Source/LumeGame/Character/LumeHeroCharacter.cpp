// Copyright Yikai Zhu.

#include "Character/LumeHeroCharacter.h"
#include "Character/Components/LumeHeroComponent.h"
#include "Character/Components/LumePawnExtensionComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeHeroCharacter)

ALumeHeroCharacter::ALumeHeroCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	HeroComponent = CreateDefaultSubobject<ULumeHeroComponent>(TEXT("HeroComponent"));
}

void ALumeHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PawnExtComponent->SetupPlayerInputComponent();
}
