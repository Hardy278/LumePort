// Copyright Yikai Zhu.

#include "Character/LumeCharacter.h"
#include "Character/Components/LumeCharacterMovementComponent.h"
#include "Character/Components/LumePawnExtensionComponent.h"
#include "LumeLogChannels.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumeCharacter)

static FName NAME_LumeCharacterCollisionProfile_Capsule(TEXT("LumePawnCapsule"));
static FName NAME_LumeCharacterCollisionProfile_Mesh(TEXT("LumePawnMesh"));

ALumeCharacter::ALumeCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<ULumeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	SetNetCullDistanceSquared(900000000.0f);

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComp->SetCollisionProfileName(NAME_LumeCharacterCollisionProfile_Capsule);

	USkeletalMeshComponent* MeshComp = GetMesh();
	check(MeshComp);
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));  // Rotate mesh to be X forward since it is exported as Y forward.
	MeshComp->SetCollisionProfileName(NAME_LumeCharacterCollisionProfile_Mesh);

	ULumeCharacterMovementComponent* LumeMoveComp = CastChecked<ULumeCharacterMovementComponent>(GetCharacterMovement());
	LumeMoveComp->GravityScale = 1.0f;
	LumeMoveComp->MaxAcceleration = 2400.0f;
	LumeMoveComp->BrakingFrictionFactor = 1.0f;
	LumeMoveComp->BrakingFriction = 6.0f;
	LumeMoveComp->GroundFriction = 8.0f;
	LumeMoveComp->BrakingDecelerationWalking = 1400.0f;
	LumeMoveComp->bUseControllerDesiredRotation = false;
	LumeMoveComp->bOrientRotationToMovement = false;
	LumeMoveComp->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	LumeMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	LumeMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	LumeMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	LumeMoveComp->SetCrouchedHalfHeight(65.0f);

	PawnExtComponent = CreateDefaultSubobject<ULumePawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}


void ALumeCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnExtComponent->HandleControllerChanged();
}

void ALumeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnExtComponent->HandlePlayerStateReplicated();
}