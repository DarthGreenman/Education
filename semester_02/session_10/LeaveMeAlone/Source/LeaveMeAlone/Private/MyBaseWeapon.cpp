// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "MyBaseWeapon.h"

// Sets default values
AMyBaseWeapon::AMyBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = UObject::CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Super::SetRootComponent(WeaponComponent);
}

// Called when the game starts or when spawned
void AMyBaseWeapon::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AMyBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

