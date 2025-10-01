// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "FirstAidKit.h"

#include "BasicCharacter.h"
#include "Health.h"

#include "Components/SphereComponent.h"

// Sets default values
AFirstAidKit::AFirstAidKit()
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
void AFirstAidKit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AFirstAidKit::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstAidKit::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(Super::NotifyActorBeginOverlap(OtherActor); IsValid(OtherActor))
	{
		if (const ABasicCharacter * Character{Cast<ABasicCharacter>(OtherActor)}; GivePickup(Character))
		{
			PickupWasTaken();
		}
	}
}

bool AFirstAidKit::GivePickup(const ABasicCharacter* Character) const
{
	UHealth* Health = Character->GetHealthComponent();
	if (!IsValid(Health))
	{
		return false;
	}
	return Health->AddHealth(Pickup.Health);
}

void AFirstAidKit::PickupWasTaken()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Super::GetRootComponent()->SetVisibility(false, true);

	FTimerHandle RespawnTimerHandle{};
	Super::GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AFirstAidKit::RespawnPickup, Pickup.RespawnTime);
}

void AFirstAidKit::RespawnPickup()
{
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Super::GetRootComponent()->SetVisibility(true, true);
}
