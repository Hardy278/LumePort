// Copyright Yikai Zhu.

#pragma once

#include "NativeGameplayTags.h"

#define UE_API LUMEGAME_API

namespace LumeGameplayTags
{
	UE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	UE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	UE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	UE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	UE_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
}

#undef UE_API