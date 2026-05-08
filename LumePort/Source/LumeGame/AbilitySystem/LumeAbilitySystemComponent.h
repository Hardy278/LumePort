// Copyright Yikai Zhu.

#pragma once

#include "AbilitySystemComponent.h"

#include "LumeAbilitySystemComponent.generated.h"

#define UE_API LUMEGAME_API

class ULumeAbilityTagRelationshipMapping;

/**
 * 
 */
UCLASS(MinimalAPI)
class ULumeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UE_API ULumeAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Sets the current tag relationship mapping, if null it will clear it out */
	UE_API void SetTagRelationshipMapping(ULumeAbilityTagRelationshipMapping* NewMapping);

	UE_API void AbilityInputTagPressed(const FGameplayTag& InputTag);
	UE_API void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:

	UPROPERTY()
	TObjectPtr<ULumeAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};

#undef UE_API