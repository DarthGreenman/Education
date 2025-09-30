// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "WeaponAutomatic.h"

// Sets default values
AWeaponAutomatic::AWeaponAutomatic() : AWeaponBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AWeaponAutomatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponAutomatic::Fire()
{
	for (; !IsClipEmpty(); DecrementBullets())
	{
		Super::Shoot(Kalashnikov.FiringRange, Kalashnikov.RateOfFire);
	} 	
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

void AWeaponAutomatic::EditorKeyPressed(FKey Key, EInputEvent Event)
{
	Super::EditorKeyPressed(Key, Event);
	//throw std::logic_error("The method or operation is not implemented.");
}

// Called when the game starts or when spawned
void AWeaponAutomatic::BeginPlay()
{
	Super::BeginPlay();
	Ammo = std::make_unique<FAmmoWeapon>(FullCombatLoad);
}

void AWeaponAutomatic::DecrementBullets()
{
	--Ammo->Bullets;
	UE_LOG(LogTemp, Display, TEXT("Bullets = %s"), *FString::FromInt(Ammo->Bullets));
}
