// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MyWeapon.generated.h"

class AMyBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API UMyWeapon : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AMyBaseWeapon> WeaponClass;

	UPROPERTY()
	AMyBaseWeapon* Weapon{};

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SpawnWeapon();
};
