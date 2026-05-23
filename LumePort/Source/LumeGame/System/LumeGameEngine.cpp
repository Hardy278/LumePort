// Copyright Yikai Zhu.

#include "System/LumeGameEngine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeGameEngine)

class IEngineLoop;

ULumeGameEngine::ULumeGameEngine(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void ULumeGameEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);
}
