// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyDamage.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyDamage::AMyDamage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;

	SphereDamage = UObject::CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereDamage->SetSphereRadius(RadialDamage.Radius);
	Super::SetRootComponent(SphereDamage);
	
	StaticMesh = UObject::CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMesh->SetupAttachment(SphereDamage);
	StaticMesh->SetRelativeScale3D(FVector{2.0f, 2.0f, 0.05f});
	StaticMesh->SetRelativeLocation(FVector{0.0f, 0.0f, -4.0f});
}

// Called every frame
void AMyDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(), RadialDamage.Damage, GetActorLocation(), RadialDamage.Radius, nullptr, {}, this, nullptr, false);
}

// Called when the game starts or when spawned
void AMyDamage::BeginPlay()
{
	Super::BeginPlay();
}
