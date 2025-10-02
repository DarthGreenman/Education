// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "WeaponBasic.h"
#include "WeaponInterface.h"

#include <memory>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"

#include "WeaponAutomatic.generated.h"

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite{};
};

namespace Helper
{
struct WeaponParameters
{
	int32 FiringRange{};
	float RateOfFire{};
};
} // namespace Helper

UCLASS()
class LEAVEMEALONE_API AWeaponAutomatic : public AWeaponBasic, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponAutomatic();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire() override;
	virtual void ReleaseTheTrigger() override;
	virtual void Reload() override;

	bool IsClipEmpty() const;
	bool IsCombatLoadEmpty() const;
	FAmmoWeapon GetAmmoWeapon() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void ShowWeaponInformation() const;

	const Helper::WeaponParameters Kalashnikov{1000, 60.0f / 100};
	const FAmmoWeapon FullCombatLoad{30, 19, true};
	std::unique_ptr<FAmmoWeapon> Ammo{};
	
	FTimerHandle TimerHandle{};
};