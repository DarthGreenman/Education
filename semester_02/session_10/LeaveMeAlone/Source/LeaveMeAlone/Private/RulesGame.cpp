// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "RulesGame.h"

#include "BasicCharacter.h"

#include "GamePlayerController.h"

ARulesGame::ARulesGame() 
{
	AGameModeBase::DefaultPawnClass = ABasicCharacter::StaticClass();
	AGameModeBase::PlayerControllerClass = AGamePlayerController::StaticClass();
}
