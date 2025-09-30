// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FirstAidKit.generated.h"

class USphereComponent;
class ABasicCharacter;

USTRUCT(BlueprintType)
struct FHealthPickup
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float RespawnTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float Health{};
};

UCLASS()
class LEAVEMEALONE_API AFirstAidKit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstAidKit();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* Sphere{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FHealthPickup Pickup{5.0f, 100.0f};

private:
	// Отвечает за то, взяли ли мы пикап.
	bool GivePickup(const ABasicCharacter* Character) const;
	// Запускает таймер респауна и скрывает наш пикап на временной промежуток, после взятия пикапа.
	void PickupWasTaken();
	// Возвращает видимость пикапа по прошествии определенного количества времени.
	void RespawnPickup();
};
