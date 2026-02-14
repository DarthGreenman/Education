// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "EnemyController.h"
#include "Enemy.h"

void AEnemyController::OnPossess(APawn* InPawn) 
{
	Super::OnPossess(InPawn);
	const auto AICharacter = Cast<AEnemy>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTree);
	}
}
