// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyHealthPickup.h"
#include "MyCharacter.h"
#include "MyHealth.h"

#include "Components/SphereComponent.h"

// Sets default values
AMyHealthPickup::AMyHealthPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;
	// В самом начала, необходимо инициализировать сферический компонент.
	// Кроме того зададим условия перекрытия через функцию настройки коллизии SetCollisionResponseToAllChannels.
	Sphere = UObject::CreateDefaultSubobject<USphereComponent>("SphereComponent");
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Super::SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void AMyHealthPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyHealthPickup::NotifyActorBeginOverlap(AActor* OtherActor) 
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (const auto Charcter = Cast<AMyCharacter>(OtherActor); GivePickup(Charcter))
		PickupWasTaken();
}

bool AMyHealthPickup::GivePickup(AMyCharacter* Character) const
{
	const auto Health = Character->GetHealthComponent();
	if (!Health)
		return false;
	return Health->AddHealth(Pickup.Health);
}

void AMyHealthPickup::PickupWasTaken()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Super::GetRootComponent()->SetVisibility(false, true);

	FTimerHandle RespawnTimerHandle;
	Super::GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMyHealthPickup::RespawnPickup, Pickup.RespawnTime);
}

void AMyHealthPickup::RespawnPickup()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Super::GetRootComponent()->SetVisibility(true, true);
}
