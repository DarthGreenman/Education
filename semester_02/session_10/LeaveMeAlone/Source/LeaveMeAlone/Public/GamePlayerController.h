// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 *
 */
UCLASS()
class LEAVEMEALONE_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AGamePlayerController();

protected:
	virtual void BeginSpectatingState() override;
	virtual void BeginPlay() override;
};
