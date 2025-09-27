// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyWeapon.h"

#include "MyBaseWeapon.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UMyWeapon::UMyWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	Super::PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMyWeapon::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
}

// Called every frame
void UMyWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

// Структура с правилами присоединения отвечают за то, как будет присоединен объект к персонажу и имеет три вариации:
// 1. KeepRelative – просчитывает относительную трансформацию присоединяемого объекта к относительной трансформации компонента к которому
//    идет присоединение
// 2. KeepWorld – просчитывает относительную трансформацию присоединяемого объекта к трансформации в мире компонента к которому идет
//    присоединение.
// 3. SnapToTarget – наследует правила трансформации компонента,к которому мы его присоединяем
void UMyWeapon::SpawnWeapon() 
{
	Weapon = Super::GetWorld()->SpawnActor<AMyBaseWeapon>(WeaponClass);
	if (Weapon)
	{
		if (const auto Character = Cast<ACharacter>(Super::GetOwner()); Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "WeaponSocket");
		}
	}
}
