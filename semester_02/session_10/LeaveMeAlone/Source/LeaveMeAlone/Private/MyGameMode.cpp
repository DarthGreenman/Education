// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode() 
{
	AGameModeBase::DefaultPawnClass = AMyCharacter::StaticClass();
	AGameModeBase::PlayerControllerClass = AMyPlayerController::StaticClass();
}
