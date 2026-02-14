// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LEAVEMEALONE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
