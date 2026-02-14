// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "HeroController.h"

AHeroController::AHeroController() {}

void AHeroController::BeginSpectatingState()
{
	AController::SetControlRotation(FRotator{-45.0f, 0.0f, 0.0f});
	Super::BeginSpectatingState();
}

void AHeroController::BeginPlay()
{
	Super::BeginPlay();
	
	//Super::SetInputMode(FInputModeGameAndUI());
	//Super::bShowMouseCursor = true;

	Super::SetInputMode(FInputModeGameOnly());
	Super::bShowMouseCursor = false;
}
