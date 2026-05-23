// Copyright Yikai Zhu.

#include "Player/LumePlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LumePlayerController)

ALumePlayerController::ALumePlayerController(const FObjectInitializer& ObjectInitializer) :
    Super(ObjectInitializer)
{
}

void ALumePlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Show mouse cursor
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // Setup input mode (Game and UI)
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);

    SetInputMode(InputMode);
}
