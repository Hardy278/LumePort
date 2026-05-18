// Copyright Yikai Zhu.

#pragma once

#include "NativeGameplayTags.h"

#define UE_API LUMEGAME_API

namespace LumeGameplayTags
{
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);

	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	UE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);
}

#undef UE_API