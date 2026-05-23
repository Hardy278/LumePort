// Copyright Yikai Zhu.

#pragma once

#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"

#include "LumePlayerState.generated.h"

#define UE_API LUMEGAME_API

class ULumeAbilitySystemComponent;
class ULumeExperienceDefinition;
class ALumePlayerController;
class ULumePawnData;

class UAbilitySystemComponent;

/** Defines the types of client connected */
UENUM()
enum class ELumePlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};

/**
 * 
 */
UCLASS(MinimalAPI, Config = Game)
class ALumePlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UE_API ALumePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Lume|PlayerState")
	UE_API ALumePlayerController* GetLumePlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Lume|PlayerState")
	ULumeAbilitySystemComponent* GetLumeAbilitySystemComponent() const { return AbilitySystemComponent; }
	UE_API virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	UE_API void SetPawnData(const ULumePawnData* InPawnData);

	//~AActor interface
	UE_API virtual void PreInitializeComponents() override;
	UE_API virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	UE_API virtual void Reset() override;
	UE_API virtual void ClientInitialize(AController* C) override;
	UE_API virtual void CopyProperties(APlayerState* PlayerState) override;
	UE_API virtual void OnDeactivated() override;
	UE_API virtual void OnReactivated() override;
	//~End of APlayerState interface

	static UE_API const FName NAME_LumeAbilityReady;

	UE_API void SetPlayerConnectionType(ELumePlayerConnectionType NewType);
	ELumePlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

protected:
	UFUNCTION()
	UE_API void OnRep_PawnData();

	UE_API void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULumePawnData> PawnData;

private:
	UE_API void OnExperienceLoaded(const ULumeExperienceDefinition* CurrentExperience);

private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Lume|PlayerState")
	TObjectPtr<ULumeAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Replicated)
	ELumePlayerConnectionType MyPlayerConnectionType;
};

#undef UE_API