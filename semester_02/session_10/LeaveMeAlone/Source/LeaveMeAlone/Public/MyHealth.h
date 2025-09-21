// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include <memory>

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MyHealth.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath) // Делегат, который будет оповещать все заинтересованные классы о том, что персонаж умер.
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API UMyHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyHealth();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth{100.0f};

public:
	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return *Health; }

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return !(*Health); }

	// Прибавляет количество жизней и возвращает значение true, если здоровье было успешно прибавлено.
	bool AddHealth(float NewHealth);
	//  Проверяет, полное ли здоровье в настоящее время.
	bool IsHealthFull() const { return FMath::IsNearlyEqual(*Health, MaxHealth); }

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

private:
	std::unique_ptr<float> Health{};
	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
