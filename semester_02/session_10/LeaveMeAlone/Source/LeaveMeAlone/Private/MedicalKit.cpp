// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MedicalKit.h"

#include "Hero.h"
#include "Health.h"

#include "Components/SphereComponent.h"

// Sets default values
AMedicalKit::AMedicalKit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;
	// В самом начала, необходимо инициализировать сферический компонент.
	// Кроме того зададим условия перекрытия через функцию настройки коллизии SetCollisionResponseToAllChannels.
	Sphere = UObject::CreateDefaultSubobject<USphereComponent>("SphereComponent");
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Super::SetRootComponent(Sphere);
}

// Called every frame
void AMedicalKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AMedicalKit::BeginPlay()
{
	Super::BeginPlay();
}

void AMedicalKit::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(Super::NotifyActorBeginOverlap(OtherActor); IsValid(OtherActor))
	{
		if (const AHero * Character{Cast<AHero>(OtherActor)}; GivePickup(Character))
		{
			PickupWasTaken();
		}
	}
}

bool AMedicalKit::GivePickup(const AHero* Character) const
{
	UHealth* Health = Character->GetHealthComponent();
	if (!IsValid(Health))
	{
		return false;
	}
	return Health->AddHealth(Pickup.Health);
}

void AMedicalKit::PickupWasTaken()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Super::GetRootComponent()->SetVisibility(false, true);

	FTimerHandle RespawnTimerHandle{};
	Super::GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMedicalKit::RespawnPickup, Pickup.RespawnTime);
}

void AMedicalKit::RespawnPickup()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Super::GetRootComponent()->SetVisibility(true, true);
}
