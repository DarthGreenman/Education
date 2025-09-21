// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyDamage.generated.h"

class USphereComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FAreaDamage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius{};
};

UCLASS()
class LEAVEMEALONE_API AMyDamage : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyDamage();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereDamage{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FAreaDamage RadialDamage{5.0f, 100.0f};
};
