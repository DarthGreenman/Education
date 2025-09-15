// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperBall.h"

// Sets default values
ASuperBall::ASuperBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;
	Mesh = UObject::CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Super::SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ASuperBall::BeginPlay()
{
	Super::BeginPlay();
}

void ASuperBall::ShowActorInformation() const
{
	UE_LOG(LogTemp, Display, TEXT("Координаты актора x:%f, y:%f, z:%f"), Super::GetActorLocation().X, Super::GetActorLocation().Y,
		Super::GetActorLocation().Y);
}

FVector ASuperBall::MoveLawMotion(const FVector& Location, const FMovementParameters& Parameters) const
{
	const auto Phase = Parameters.Frequency * Super::GetWorld()->GetTimeSeconds();
	return FVector{Location.X, Location.Y + Phase, Location.Z + Parameters.Amplitude * sin(Phase)};
}

// Called every frame
void ASuperBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
