// Copyright Yikai Zhu.

#include "Character/Components/LumeHeroComponent.h"
#include "Character/Components/LumePawnExtensionComponent.h"
#include "Character/PawnData/LumeHeroData.h"
#include "Input/LumeInputConfig.h"
#include "Input/LumeInputComponent.h"
#include "AbilitySystem/LumeAbilitySystemComponent.h"
#include "Player/LumePlayerController.h"
#include "Player/LumePlayerState.h"
#include "Player/LumeLocalPlayer.h"
#include "LumeGameplayTags.h"
#include "LumeLogChannels.h"

#include "Components/GameFrameworkComponentManager.h"
#include "Components/GameFrameworkComponentDelegates.h"

#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputMappingContext.h"

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeHeroComponent)

namespace LumeHero
{
	static const float LookYawRate = 300.0f;
	static const float LookPitchRate = 165.0f;
};

const FName ULumeHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName ULumeHeroComponent::NAME_ActorFeatureName("Hero");

ULumeHeroComponent::ULumeHeroComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

void ULumeHeroComponent::AddAdditionalInputConfig(const ULumeInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const ULumePawnExtensionComponent* PawnExtComp = ULumePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		ULumeInputComponent* LumeIC = Pawn->FindComponentByClass<ULumeInputComponent>();
		if (ensureMsgf(LumeIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULumeInputComponent or a subclass of it.")))
		{
			LumeIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void ULumeHeroComponent::RemoveAdditionalInputConfig(const ULumeInputConfig* InputConfig)
{
	//@TODO: Implement it.
}

bool ULumeHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void ULumeHeroComponent::InitializeAbilitySystem(ULumeAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	// 1.check if the ASC should be initialize.
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		// The ability system component hasn't changed.
		return;
	}

	if (AbilitySystemComponent)
	{
		// Clean up the old ability system component.
		UninitializeAbilitySystem();
	}

	// 2. set which actor the ASC belong to. (Avatar / Owner)
	APawn* Pawn = GetPawnChecked<APawn>(); // The pawn the ASC belong to.
	AActor* ExistingAvatar = InASC->GetAvatarActor();

	UE_LOG(LogLume, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		UE_LOG(LogLume, Log, TEXT("Existing avatar (authority=%d)"), ExistingAvatar->HasAuthority() ? 1 : 0);

		// There is already a pawn acting as the ASC's avatar, so we need to kick it out
		// This can happen on clients if they're lagged: their new pawn is spawned + possessed before the dead one is removed
		ensure(!ExistingAvatar->HasAuthority());

		if (ULumeHeroComponent* OtherHeroComponent = FindHeroComponent(ExistingAvatar))
		{
			OtherHeroComponent->UninitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	// 3. set TagRelationshipMapping.
	ULumePawnExtensionComponent* ExtensionComponent = ULumePawnExtensionComponent::FindPawnExtensionComponent(Pawn);
	const ULumePawnData* PawnData = ExtensionComponent->GetPawnData<ULumePawnData>();
	if (ensure(PawnData))
	{
		InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	}

	// 4. broadcast the delegate when initialization finished.
	OnAbilitySystemInitialized.Broadcast();
}

void ULumeHeroComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		/*FGameplayTagContainer AbilityTypesToIgnore;
		AbilityTypesToIgnore.AddTag(LumeGameplayTags::Ability_Behavior_SurvivesDeath);

		AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);*/
		//AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// If the ASC doesn't have a valid owner, we need to clear *all* actor info, not just the avatar pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

bool ULumeHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == LumeGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == LumeGameplayTags::InitState_Spawned && DesiredState == LumeGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		ALumePlayerState* PS = GetPlayerState<ALumePlayerState>();
		if (!PS)
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ALumePlayerController* LumePC = GetController<ALumePlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !LumePC || !LumePC->GetLocalPlayer())
			{
				return false;
			}
		}
		return true;
	}
	else if (CurrentState == LumeGameplayTags::InitState_DataAvailable && DesiredState == LumeGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ALumePlayerState* LumePS = GetPlayerState<ALumePlayerState>();

		return LumePS && Manager->HasFeatureReachedInitState(Pawn, ULumePawnExtensionComponent::NAME_ActorFeatureName, LumeGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == LumeGameplayTags::InitState_DataInitialized && DesiredState == LumeGameplayTags::InitState_GameplayReady)
	{
		// TODO add ability initialization checks?
		return true;
	}

	return false;
}

void ULumeHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == LumeGameplayTags::InitState_DataAvailable && DesiredState == LumeGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ALumePlayerState* LumePS = GetPlayerState<ALumePlayerState>();
		if (!ensure(Pawn && LumePS))
		{
			return;
		}

		const ULumePawnData* PawnData = nullptr;

		if (ULumePawnExtensionComponent* PawnExtComp = ULumePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<ULumePawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			InitializeAbilitySystem(LumePS->GetLumeAbilitySystemComponent(), LumePS);

			PawnExtComp->OnControllerChangeDelegate.AddDynamic(this, &ThisClass::OnControllerChanged);
		}

		if (ALumePlayerController* LumePC = GetController<ALumePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		// Hook up the delegate for all pawns, in case we spectate later
		/*if (PawnData)
		{
			if (ULumeCameraComponent* CameraComponent = ULumeCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}*/
	}
}

void ULumeHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == ULumePawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == LumeGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void ULumeHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		LumeGameplayTags::InitState_Spawned,
		LumeGameplayTags::InitState_DataAvailable,
		LumeGameplayTags::InitState_DataInitialized,
		LumeGameplayTags::InitState_GameplayReady
	};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	FGameplayTag Tag = ContinueInitStateChain(StateChain);
	UE_LOG(LogTemp, Log, TEXT("Checked default initialization for hero component, current state is %s"), *Tag.ToString());
}

void ULumeHeroComponent::OnControllerChanged()
{
	if (AbilitySystemComponent && (AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>()))
	{
		ensure(AbilitySystemComponent->AbilityActorInfo->OwnerActor == AbilitySystemComponent->GetOwnerActor());
		if (AbilitySystemComponent->GetOwnerActor() == nullptr)
		{
			UninitializeAbilitySystem();
		}
		else
		{
			AbilitySystemComponent->RefreshAbilityActorInfo();
		}
	}
}

void ULumeHeroComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void ULumeHeroComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}

void ULumeHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogLume, Error, TEXT("[ULumeHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("LumeHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("LumeHeroComponent");

			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));

			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void ULumeHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(ULumePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(LumeGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void ULumeHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeAbilitySystem();
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void ULumeHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULumeLocalPlayer* LP = Cast<ULumeLocalPlayer>(PC->GetLocalPlayer());
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (const ULumePawnExtensionComponent* PawnExtComp = ULumePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const ULumeHeroData* HeroData = Cast<ULumeHeroData>(PawnExtComp->GetPawnData<ULumePawnData>()))
		{
			if (const ULumeInputConfig* InputConfig = HeroData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.LoadSynchronous())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}

							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}

				// The Lume Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the ULumeInputComponent or modify this component accordingly.
				ULumeInputComponent* LumeIC = Cast<ULumeInputComponent>(PlayerInputComponent);
				if (ensureMsgf(LumeIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to ULumeInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					LumeIC->AddInputMappings(InputConfig, Subsystem);

					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					LumeIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);

					LumeIC->BindNativeAction(InputConfig, LumeGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					LumeIC->BindNativeAction(InputConfig, LumeGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
					LumeIC->BindNativeAction(InputConfig, LumeGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, false);
				}
			}
		}
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void ULumeHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (ULumeAbilitySystemComponent* LumeASC = GetLumeAbilitySystemComponent())
		{
			LumeASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void ULumeHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (ULumeAbilitySystemComponent* LumeASC = GetLumeAbilitySystemComponent())
	{
		LumeASC->AbilityInputTagReleased(InputTag);
	}
}

void ULumeHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}
		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void ULumeHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const float Value = InputActionValue.Get<float>();

	if (Value!= 0.0f)
	{
		Pawn->AddControllerYawInput(Value);
	}
}

void ULumeHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const float Value = InputActionValue.Get<float>();

	const UWorld* World = GetWorld();
	check(World);
	if (Value != 0.0f)
	{
		Pawn->AddControllerYawInput(Value * LumeHero::LookYawRate * World->GetDeltaSeconds());
	}
}