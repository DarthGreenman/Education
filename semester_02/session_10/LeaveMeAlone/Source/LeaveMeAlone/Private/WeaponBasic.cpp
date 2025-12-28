// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "WeaponBasic.h"

#include <limits>
#include <Kismet/GameplayStatics.h>

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

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

void AWeaponBasic::Shoot(int32 FiringRange)
{
	if (FiringRange > 0)
	{
		ShowTracer(FiringRange);
	}

	bBullethInTheChamber = false;
}

// Called when the game starts or when spawned
void AWeaponBasic::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBasic::PickUpTheShutter()
{
	bBullethInTheChamber = true;
}

/* DrawDebugLine(Super::GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f); */
/* DrawDebugSphere(Super::GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f); */
void AWeaponBasic::ShowTracer(int32 FiringRange)
{
	const FTransform SocketTransform{Weapon->GetSocketTransform("Muzzle")};
	const FVector TraceStart{SocketTransform.GetLocation()};
	const FVector ShootDirection{SocketTransform.GetRotation().GetForwardVector()};
	const FVector TraceEnd{TraceStart + ShootDirection * FiringRange};
	FHitResult HitResult{};

	Super::GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	SpawnTrace(TraceStart, HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);
}

void AWeaponBasic::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
	if (TraceFX)
	{
		TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
	}
}
