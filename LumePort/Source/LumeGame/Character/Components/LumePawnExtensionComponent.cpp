// Copyright Yikai Zhu.

#include "Character/Components/LumePawnExtensionComponent.h"
#include "Character/PawnData/LumePawnData.h"
#include "AbilitySystem/LumeAbilitySystemComponent.h"
#include "LumeGameplayTags.h"
#include "LumeLogChannels.h"

#include "Components/GameFrameworkComponentDelegates.h"
#include "Components/GameFrameworkComponentManager.h"

#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumePawnExtensionComponent)

const FName ULumePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

ULumePawnExtensionComponent::ULumePawnExtensionComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

void ULumePawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULumePawnExtensionComponent, PawnData);
}

bool ULumePawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if (!CurrentState.IsValid() && DesiredState == LumeGameplayTags::InitState_Spawned)
	{
		// As long as we are on a valid pawn, we count as spawned
		if (Pawn)
		{
			return true;
		}
	}
	if (CurrentState == LumeGameplayTags::InitState_Spawned && DesiredState == LumeGameplayTags::InitState_DataAvailable)
	{
		// Pawn data is required.
		if (!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		if (bHasAuthority || bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}
		return true;
	}
	else if (CurrentState == LumeGameplayTags::InitState_DataAvailable && DesiredState == LumeGameplayTags::InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, LumeGameplayTags::InitState_DataAvailable);
	}
	else if (CurrentState == LumeGameplayTags::InitState_DataInitialized && DesiredState == LumeGameplayTags::InitState_GameplayReady)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, LumeGameplayTags::InitState_DataInitialized);
	}

	return false;
}

void ULumePawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (DesiredState == LumeGameplayTags::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change
	}
}

void ULumePawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// If another feature is now in DataAvailable, see if we should transition to DataInitialized
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		if (Params.FeatureState == LumeGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void ULumePawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {
		LumeGameplayTags::InitState_Spawned,
		LumeGameplayTags::InitState_DataAvailable,
		LumeGameplayTags::InitState_DataInitialized,
		LumeGameplayTags::InitState_GameplayReady
	};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	FGameplayTag Tag = ContinueInitStateChain(StateChain);
	UE_LOG(LogTemp, Log, TEXT("Checked default initialization for pawn extension component, current state is %s"), *Tag.ToString());
}

void ULumePawnExtensionComponent::SetPawnData(const ULumePawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLume, Error, TEXT("Trying to set PawnData [%s] on pawn [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(Pawn), *GetNameSafe(PawnData));
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::HandleControllerChanged()
{
	AController* NewController = GetController<AController>();
	UE_LOG(LogTemp, Log, TEXT("ControllerChanged to %s"), *GetNameSafe(NewController));
	OnControllerChanged.Broadcast();

	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::HandlePlayerStateReplicated()
{
	UE_LOG(LogTemp, Log, TEXT("PlayerState Replicated"));
	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::SetupPlayerInputComponent()
{
	UE_LOG(LogTemp, Log, TEXT("SetupPlayerInputComponent called"));
	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::OnRep_PawnData()
{
	UE_LOG(LogTemp, Log, TEXT("PawnData Replicated"));
	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("LumePawnExtensionComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(ULumePawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one LumePawnExtensionComponent should exist on [%s]."), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void ULumePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default initialization
	ensure(TryToChangeInitState(LumeGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void ULumePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}