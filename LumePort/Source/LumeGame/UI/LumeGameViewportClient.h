// Copyright Yikai Zhu.

#pragma once

#include "CommonGameViewportClient.h"

#include "LumeGameViewportClient.generated.h"

class UGameInstance;
class UObject;

/**
 * 
 */
UCLASS(BlueprintType)
class ULumeGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	ULumeGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
