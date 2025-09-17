// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController() {}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	Super::SetInputMode(FInputModeGameAndUI());
	Super::bShowMouseCursor = true;
}
