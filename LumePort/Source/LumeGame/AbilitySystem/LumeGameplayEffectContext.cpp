// Copyright Epic Games, Inc. All Rights Reserved.

#include "LumeGameplayEffectContext.h"
#include "AbilitySystem/LumeAbilitySourceInterface.h"

#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeGameplayEffectContext)

class FArchive;

FLumeGameplayEffectContext* FLumeGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FLumeGameplayEffectContext::StaticStruct()))
	{
		return (FLumeGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FLumeGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FLumeGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(LumeGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif

void FLumeGameplayEffectContext::SetAbilitySource(const ILumeAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ILumeAbilitySourceInterface* FLumeGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ILumeAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FLumeGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

