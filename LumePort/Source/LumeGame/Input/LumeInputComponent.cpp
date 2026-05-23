// Copyright Yikai Zhu.

#include "Input/LumeInputComponent.h"

#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeInputComponent)

class ULumeInputConfig;

ULumeInputComponent::ULumeInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void ULumeInputComponent::AddInputMappings(const ULumeInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}

void ULumeInputComponent::RemoveInputMappings(const ULumeInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);
}

void ULumeInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
