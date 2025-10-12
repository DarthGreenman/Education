// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "GamePlayerController.h"

AGamePlayerController::AGamePlayerController() {}

void AGamePlayerController::BeginSpectatingState()
{
	AController::SetControlRotation(FRotator{-45.0f, 0.0f, 0.0f});
	Super::BeginSpectatingState();
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	//Super::SetInputMode(FInputModeGameAndUI());
	//Super::bShowMouseCursor = true;

	Super::SetInputMode(FInputModeGameOnly());
	Super::bShowMouseCursor = false;
}
