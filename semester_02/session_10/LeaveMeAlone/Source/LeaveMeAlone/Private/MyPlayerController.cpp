// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController() {}

void AMyPlayerController::BeginSpectatingState()
{
	AController::SetControlRotation(FRotator{-45.0f, 0.0f, 0.0f});
	Super::BeginSpectatingState();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Super::SetInputMode(FInputModeGameAndUI());
	Super::bShowMouseCursor = true;
}
