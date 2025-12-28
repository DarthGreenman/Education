// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "RevolvingDoor.h"

// Sets default values
ARevolvingDoor::ARevolvingDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeafRight = UObject::CreateDefaultSubobject<UStaticMeshComponent>("LeafRight");
	Super::SetRootComponent(LeafRight);

	LeafLeft = UObject::CreateDefaultSubobject<UStaticMeshComponent>("LeafLeft");
	Super::SetRootComponent(LeafLeft);

	Frame = UObject::CreateDefaultSubobject<UStaticMeshComponent>("Frame");
	Super::SetRootComponent(Frame);
}

// Called when the game starts or when spawned
void ARevolvingDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARevolvingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARevolvingDoor::SetupParameter(float MoveStep, EAxis::Type AxisSpin)
{
	switch (AxisSpin)
	{
	case EAxis::None:
		break;
	case EAxis::X:
		Parameters = FRotator(0.0f, 0.0f, MoveStep);
		break;
	case EAxis::Y:
		Parameters = FRotator(MoveStep, 0.0f, 0.0f);
		break;
	case EAxis::Z:
		Parameters = FRotator(0.0f, MoveStep, 0.0f);
		break;
	default:
		break;
	}
}

void ARevolvingDoor::Open()
{
	Counter = 0;
	Super::GetWorldTimerManager().SetTimer(TimerHandle, this, &ARevolvingDoor::Rotation, 0.01f, true);
}

void ARevolvingDoor::Close()
{
	Counter = 0;
	Parameters = Parameters.GetInverse();
	Super::GetWorldTimerManager().SetTimer(TimerHandle, this, &ARevolvingDoor::Rotation, 0.01f, true);
}

void ARevolvingDoor::Rotation()
{
	if (++Counter; Counter < 180)
		LeafRight->AddLocalRotation(Parameters);
	else
		Super::GetWorldTimerManager().ClearTimer(TimerHandle);
}