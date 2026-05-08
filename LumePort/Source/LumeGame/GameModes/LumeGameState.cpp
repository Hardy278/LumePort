// Copyright Yikai Zhu.

#include "GameModes/LumeGameState.h"

#include "GameModes/LumeExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeGameState)

ALumeGameState::ALumeGameState(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceManagerComponent = CreateDefaultSubobject<ULumeExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
