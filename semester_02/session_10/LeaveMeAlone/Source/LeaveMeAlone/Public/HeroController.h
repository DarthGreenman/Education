// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeroController.generated.h"

/**
 *
 */
UCLASS()
class LEAVEMEALONE_API AHeroController : public APlayerController
{
	GENERATED_BODY()
public:
	AHeroController();

protected:
	virtual void BeginSpectatingState() override;
	virtual void BeginPlay() override;
};
