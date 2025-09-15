// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuperBall.generated.h"

class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FMovementParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amplitude{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency{};
};

UCLASS()
class FIRSTPROJECT_API ASuperBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASuperBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ShowActorInformation() const;

	UFUNCTION(BlueprintCallable)
	FVector MoveLawMotion(const FVector& Location, const FMovementParameters& Parameters) const;

protected:
	UPROPERTY(VisibleAnywhere);
	UStaticMeshComponent* Mesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
