// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "WeaponAutomatic.h"
#include "WeaponInterface.h"

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"

#include "WeaponSpecific.generated.h"

class AWeaponAutomatic;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API UWeaponSpecific : public UActorComponent, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponSpecific();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Fire() override final;
	virtual void Reload() override final;
	virtual void ReleaseTheTrigger() override final;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponAutomatic> WeaponClass{};

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage{};

private:
	void SpawnWeapon();
	void InitAnimNotify();
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool IsNeedsRecharged() const;

	UPROPERTY()
	AWeaponAutomatic* Weapon{};
	bool bIsRunningAnim{};
};