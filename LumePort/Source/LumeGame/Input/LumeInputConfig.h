// Copyright Yikai Zhu.

#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "LumeInputConfig.generated.h"

class UInputAction;

/**
 * @struct FLumeInputAction
 * @brief Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FLumeInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 * @class ULumeInputConfig
 * @brief Non-mutable data asset that contains input configuration properties.
 */
UCLASS(BlueprintType, Const)
class ULumeInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	ULumeInputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lume|Pawn")
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "Lume|Pawn")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

public:
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLumeInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FLumeInputAction> AbilityInputActions;
	
};
