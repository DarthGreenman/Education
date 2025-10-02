// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "WeaponAutomatic.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

// Sets default values
AWeaponAutomatic::AWeaponAutomatic() : AWeaponBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;
	Ammo = std::make_unique<FAmmoWeapon>(FullCombatLoad);
}

// Called every frame
void AWeaponAutomatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponAutomatic::Fire()
{
	if (IsClipEmpty())
	{
		return;
	}
	Super::GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponAutomatic::Fire, Kalashnikov.RateOfFire, true);
	Super::Shoot(Kalashnikov.FiringRange);
	--Ammo->Bullets;
}

void AWeaponAutomatic::ReleaseTheTrigger()
{
	Super::GetWorldTimerManager().ClearTimer(TimerHandle);
}

void AWeaponAutomatic::Reload()
{
	Ammo->Bullets = FullCombatLoad.Bullets;
	--Ammo->Clips;
}

bool AWeaponAutomatic::IsClipEmpty() const
{
	return Ammo->Bullets == 0;
}

bool AWeaponAutomatic::IsCombatLoadEmpty() const
{
	return Ammo->Clips == 0;
}

FAmmoWeapon AWeaponAutomatic::GetAmmoWeapon() const
{
	return *Ammo;
}

// Called when the game starts or when spawned
void AWeaponAutomatic::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponAutomatic::ShowWeaponInformation() const
{
	UE_LOG(LogWeapon, Display, TEXT("Патронов: %d в %d магаинах"), Ammo->Bullets, Ammo->Clips);
}
