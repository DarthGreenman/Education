// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "WeaponBasic.h"

#include <limits>

// Sets default values
AWeaponBasic::AWeaponBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = UObject::CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Super::SetRootComponent(Weapon);
}

// Called every frame
void AWeaponBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBasic::Shoot(int32 FiringRange, int32 RateOfFire)
{
	AActor::GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeaponBasic::PickUpTheShutter, RateOfFire, true);
	if (FiringRange > 0)
	{
		ShowTracer(FiringRange);
	}

	bBullethInTheChamber_ = false;
}

void AWeaponBasic::ReleaseTheTrigger()
{
	AActor::GetWorldTimerManager().ClearTimer(TimerHandle);
}

// Called when the game starts or when spawned
void AWeaponBasic::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBasic::PickUpTheShutter()
{
	bBullethInTheChamber_ = true;
}

void AWeaponBasic::ShowTracer(int32 FiringRange)
{
	const FTransform SocketTransform{Weapon->GetSocketTransform("Muzzle")};
	const FVector TraceStart{SocketTransform.GetLocation()};
	const FVector ShootDirection{SocketTransform.GetRotation().GetForwardVector()};
	const FVector TraceEnd{TraceStart + ShootDirection * FiringRange};

	DrawDebugLine(Super::GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult{};
	Super::GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(Super::GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
}
