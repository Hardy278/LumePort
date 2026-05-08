// Copyright Yikai Zhu.

#pragma once

#include "GameFramework/GameSession.h"

#include "LumeGameSession.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class ALumeGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	ALumeGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
