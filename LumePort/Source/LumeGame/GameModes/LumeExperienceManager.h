// Copyright Yikai Zhu.

#pragma once

#include "Subsystems/EngineSubsystem.h"

#include "LumeExperienceManager.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(MinimalAPI)
class ULumeExperienceManager : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	UE_API void OnPlayInEditorBegun();

	static void NotifyOfPluginActivation(const FString PluginURL);
	static bool RequestToDeactivatePlugin(const FString PluginURL);
#else
	static void NotifyOfPluginActivation(const FString PluginURL) {}
	static bool RequestToDeactivatePlugin(const FString PluginURL) { return true; }
#endif

private:
	// The map of requests to active count for a given game feature plugin
	// (to allow first in, last out activation management during PIE)
	TMap<FString, int32> GameFeaturePluginRequestCountMap;
};

#undef UE_API