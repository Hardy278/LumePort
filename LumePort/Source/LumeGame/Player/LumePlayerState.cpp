// Copyright Yikai Zhu.

#include "Player/LumePlayerState.h"
#include "Player/LumePlayerController.h"
#include "AbilitySystem/LumeAbilitySystemComponent.h"
#include "AbilitySystem/LumeAbilitySet.h"
#include "Character/PawnData/LumePawnData.h"
#include "Character/Components/LumePawnExtensionComponent.h"
#include "GameModes/LumeExperienceManagerComponent.h"
#include "GameModes/LumeExperienceDefinition.h"
#include "GameModes/LumeGameMode.h"
#include "LumeLogChannels.h"

#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumePlayerState)

const FName ALumePlayerState::NAME_LumeAbilityReady("LumeAbilitiesReady");

ALumePlayerState::ALumePlayerState(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULumeAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

ALumePlayerController* ALumePlayerState::GetLumePlayerController() const
{
	return Cast<ALumePlayerController>(GetOwner());
}

UAbilitySystemComponent* ALumePlayerState::GetAbilitySystemComponent() const
{
	return GetLumeAbilitySystemComponent();
}

void ALumePlayerState::SetPawnData(const ULumePawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLume, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const ULumeAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LumeAbilityReady);

	ForceNetUpdate();
}

void ALumePlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALumePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		ULumeExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULumeExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLumeExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ALumePlayerState::Reset()
{
	Super::Reset();
}

void ALumePlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);

	if (ULumePawnExtensionComponent* PawnExtComp = ULumePawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

void ALumePlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	//@TODO: Copy stats
}

void ALumePlayerState::OnDeactivated()
{
	bool bDestroyDeactivatedPlayerState = false;

	switch (GetPlayerConnectionType())
	{
	case ELumePlayerConnectionType::Player:
	case ELumePlayerConnectionType::InactivePlayer:
		//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
		// (e.g., for long running servers where they might build up if lots of players cycle through)
		bDestroyDeactivatedPlayerState = true;
		break;
	default:
		bDestroyDeactivatedPlayerState = true;
		break;
	}

	SetPlayerConnectionType(ELumePlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void ALumePlayerState::OnReactivated()
{
	if (GetPlayerConnectionType() == ELumePlayerConnectionType::InactivePlayer)
	{
		SetPlayerConnectionType(ELumePlayerConnectionType::Player);
	}
}

void ALumePlayerState::OnExperienceLoaded(const ULumeExperienceDefinition* /*CurrentExperience*/)
{
	if (ALumeGameMode* LumeGameMode = GetWorld()->GetAuthGameMode<ALumeGameMode>())
	{
		if (const ULumePawnData* NewPawnData = LumeGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogLume, Error, TEXT("ALumePlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void ALumePlayerState::SetPlayerConnectionType(ELumePlayerConnectionType NewType)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyPlayerConnectionType, this);
	MyPlayerConnectionType = NewType;
}

void ALumePlayerState::OnRep_PawnData()
{
}

void ALumePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
}
