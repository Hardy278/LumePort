// Copyright Yikai Zhu.

#pragma once

#include "Logging/LogMacros.h"

LUMEGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLume, Log, All);
LUMEGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLumeExperience, Log, All);
LUMEGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLumeAbilitySystem, Log, All);
LUMEGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogLumeTeams, Log, All);

LUMEGAME_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
