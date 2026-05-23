// Copyright Yikai Zhu.

#include "AbilitySystem/Abilities/LumeHeroGameplayAbility.h"
#include "Character/LumeHeroCharacter.h"
#include "Character/Components/LumeHeroComponent.h"
#include "Player/LumePlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeHeroGameplayAbility)

ALumePlayerController* ULumeHeroGameplayAbility::GetLumePlayerControllerFromActorInfo()
{
	if (!CatchedLumePlayerController.IsValid())
	{
		CatchedLumePlayerController = Cast<ALumePlayerController>(CurrentActorInfo->PlayerController);
	}

	return CatchedLumePlayerController.IsValid() ? CatchedLumePlayerController.Get() : nullptr;
}

ALumeHeroCharacter* ULumeHeroGameplayAbility::GetLumeHeroCharacterFromActorInfo()
{
	if (!CatchedLumeHeroCharacter.IsValid())
	{
		CatchedLumeHeroCharacter = Cast<ALumeHeroCharacter>(CurrentActorInfo->AvatarActor);
	}

	return CatchedLumeHeroCharacter.IsValid() ? CatchedLumeHeroCharacter.Get() : nullptr;
}

ULumeHeroComponent* ULumeHeroGameplayAbility::GetHeroComponentFromActorInfo()
{
	if (!CatchedLumeHeroCharacter.IsValid())
	{
		CatchedLumeHeroCharacter = Cast<ALumeHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CatchedLumeHeroCharacter.IsValid() ? ULumeHeroComponent::FindHeroComponent(CatchedLumeHeroCharacter.Get()) : nullptr;
}
