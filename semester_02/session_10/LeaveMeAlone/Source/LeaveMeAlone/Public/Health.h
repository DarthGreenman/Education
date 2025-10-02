// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "Health.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath) // Делегат, который будет оповещать все заинтересованные классы о том, что персонаж умер.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealth();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return !(Health); }

	// Прибавляет количество жизней и возвращает значение true, если здоровье было успешно прибавлено.
	bool AddHealth(float NewHealth);
	//  Проверяет, полное ли здоровье в настоящее время.
	bool IsHealthFull() const { return FMath::IsNearlyEqual(Health, MaxHealth); }

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth{100.0f};

public:
private:
	float Health{};
	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
