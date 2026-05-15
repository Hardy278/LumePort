// Copyright Yikai Zhu.

#pragma once

#include "Character/LumeCharacter.h"

#include "LumeEnemyCharacter.generated.h"

#define UE_API LUMEGAME_API

/**
 * 
 */
UCLASS(Abstract, MinimalAPI, Config = Game)
class ALumeEnemyCharacter : public ALumeCharacter
{
	GENERATED_BODY()
	
};

#undef UE_API