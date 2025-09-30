// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBasic.generated.h"

UCLASS()
class LEAVEMEALONE_API AWeaponBasic : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBasic();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Shoot(int32 FiringRange, int32 RateOfFire);
	void ReleaseTheTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* Weapon{};

private:
	void PickUpTheShutter();
	void ShowTracer(int32 FiringRange);

	bool bBullethInTheChamber_{};
	FTimerHandle TimerHandle{};
};
