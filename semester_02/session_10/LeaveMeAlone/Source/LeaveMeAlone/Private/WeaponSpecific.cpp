// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "WeaponSpecific.h"

#include "WeaponAutomatic.h"
#include "WeaponReloadAnim.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UWeaponSpecific::UWeaponSpecific()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	Super::PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UWeaponSpecific::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponSpecific::Fire()
{
	Weapon->Fire();
}

void UWeaponSpecific::Reload()
{
	if (ACharacter * Character{Cast<ACharacter>(GetOwner())}; IsValid(Character) && IsNeedsRecharged())
	{
		Weapon->Reload();
		bIsRunningAnim = true;
		Character->PlayAnimMontage(ReloadMontage);
	}
}

void UWeaponSpecific::ReleaseTheTrigger()
{
	Weapon->ReleaseTheTrigger();
}

// Called when the game starts
void UWeaponSpecific::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

// Структура с правилами присоединения отвечают за то, как будет присоединен объект к персонажу и имеет три вариации:
// 1. KeepRelative – просчитывает относительную трансформацию присоединяемого объекта к относительной трансформации компонента к которому
//    идет присоединение
// 2. KeepWorld – просчитывает относительную трансформацию присоединяемого объекта к трансформации в мире компонента к которому идет
//    присоединение.
// 3. SnapToTarget – наследует правила трансформации компонента,к которому мы его присоединяем
void UWeaponSpecific::SpawnWeapon()
{
	Weapon = Super::GetWorld()->SpawnActor<AWeaponAutomatic>(WeaponClass);
	if (IsValid(Weapon))
	{
		if (const ACharacter * Character{Cast<ACharacter>(Super::GetOwner())}; Character)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "WeaponSocket");
		}
	}
}

void UWeaponSpecific::InitAnimNotify()
{
	if (!IsValid(ReloadMontage))
	{
		return;
	}
	const TArray<FAnimNotifyEvent>& NotifiesEvents{ReloadMontage->Notifies};
	for (const auto& NotifyEvent : NotifiesEvents)
	{
		if (UWeaponReloadAnim * ReloadFinish{Cast<UWeaponReloadAnim>(NotifyEvent.Notify)}; ReloadFinish)
		{
			ReloadFinish->OnNotifyReload.AddUObject(this, &UWeaponSpecific::OnNotifyReloadFinished);
			break;
		}
	}
}

void UWeaponSpecific::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	if (const ACharacter * Character{Cast<ACharacter>(Super::GetOwner())}; Character->GetMesh() == SkeletalMesh)
	{
		bIsRunningAnim = false;
	}
}

bool UWeaponSpecific::IsNeedsRecharged() const
{
	return Weapon->IsClipEmpty();
}
