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

USTRUCT(BlueprintType)
struct FWeaponParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 FiringRange{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 RateOfFire{}; /// Темп стрельбы около 600 выстрелов в минуту
};

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
	virtual void Reload() override;
	bool IsClipEmpty() const;
	bool IsCombatLoadEmpty() const;


 void EditorKeyPressed(FKey Key, EInputEvent Event) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon FullCombatLoad{30, 19, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FWeaponParameters Kalashnikov{1000, 1};

private:
	void DecrementBullets();

	std::unique_ptr<FAmmoWeapon> Ammo{};
};