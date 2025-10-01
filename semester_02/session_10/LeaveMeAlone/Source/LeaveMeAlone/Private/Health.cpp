// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	Super::PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UHealth::AddHealth(float NewHealth)
{
	if (IsDead() || IsHealthFull())
	{
		return false;
	}
	*Health = FMath::Clamp(*Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(*Health);
	return true;
}

// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	Health = std::make_unique<float>(MaxHealth);
	OnHealthChanged.Broadcast(*Health);

	if (AActor * Owner{Super::GetOwner()}; IsValid(Owner))
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealth::OnTakeAnyDamage);
	}
}

void UHealth::OnTakeAnyDamage(
	AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead())
	{
		return;
	}

	*Health = FMath::Clamp(*Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(*Health);
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}
