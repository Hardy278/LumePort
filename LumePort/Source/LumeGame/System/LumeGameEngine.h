// Copyright Yikai Zhu.

#pragma once

#include "Engine/GameEngine.h"

#include "LumeGameEngine.generated.h"

class IEngineLoop;
class UObject;

/**
 * 
 */
UCLASS()
class ULumeGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:

	ULumeGameEngine(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void Init(IEngineLoop* InEngineLoop) override;
};
