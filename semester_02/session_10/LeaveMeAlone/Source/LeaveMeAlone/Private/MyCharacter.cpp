// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	/// Данное условие не позволит нашей камере поворачиваться в момент поворота персонажа.
	/// В жанре Top - Down Shooter, камера обычно находится статично над игроком.
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = Camera.SpringArmLength;
	/// Cтруктура FRotator хранит аргументы в следующей последовательности: Pitch, Yaw, Roll.
	/// Так как нам необходимо определить значения по оси Y, мы устанавливаем Pitch аргумент.
	SpringArmComponent->SetRelativeRotation(FRotator(Camera.RotationAroundAxisY, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(Camera.FieldOfView);
	/// Данное условие запрещаем камере вращаться относительно SpringArmComponent
	CameraComponent->bUsePawnControlRotation = false;

	/// Запретим нашему персонажу поворачиваться в сторону камеры
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/// В данной логике мы проверяем задан ли материал для нашего курсора и если он задан, тогда мы используем функцию,
	/// реализованную в библиотеке GameplayStatics, которая называется SpawnDecalAtLocation.
	/// В качестве параметров она принимает указатель на мир, материал, размеры курсора, а также точку спауну.
	/// В настоящее время мы размещаем курсор в нулевой координате нашей игры.

	if (CursorMaterial)
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(Super::GetWorld(), CursorMaterial, CursorSize, FVector(0));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		const float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(Super::GetActorLocation(), ResultHit.Location).Yaw;
		Super::SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
			CurrentCursor->SetWorldLocation(ResultHit.Location);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}

void AMyCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
