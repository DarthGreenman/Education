// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBasic.generated.h"

class USoundWave;
class UNiagaraSystem;

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

	void Shoot(int32 FiringRange);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* Weapon{};

	// Sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USoundWave* ShootWave{};

	// FX
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* TraceEffect{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FString TraceName = "Tracer";

private:
	void PickUpTheShutter();
	void ShowTracer(int32 FiringRange);
	void SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd);

	bool bBullethInTheChamber{};
};
