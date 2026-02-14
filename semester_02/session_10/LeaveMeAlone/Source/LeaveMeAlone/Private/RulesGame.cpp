// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "RulesGame.h"

#include "Hero.h"

#include "HeroController.h"

ARulesGame::ARulesGame() 
{
	AGameModeBase::DefaultPawnClass = AHero::StaticClass();
	AGameModeBase::PlayerControllerClass = AHeroController::StaticClass();
}
