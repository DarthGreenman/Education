// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "MyCharacter.h"
#include "MyHealth.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::PrimaryActorTick.bCanEverTick = true;

	SpringArm = UObject::CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArm->SetupAttachment(GetRootComponent());
	/// Данное условие не позволит нашей камере поворачиваться в момент поворота персонажа.
	/// В жанре Top - Down Shooter, камера обычно находится статично над игроком.
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = CameraParams.SpringArmLength;
	/// Cтруктура FRotator хранит аргументы в следующей последовательности: Pitch, Yaw, Roll.
	/// Так как нам необходимо определить значения по оси Y, мы устанавливаем Pitch аргумент.
	SpringArm->SetRelativeRotation(FRotator(CameraParams.RotationAroundAxisY, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;

	Camera = UObject::CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	Camera->SetupAttachment(SpringArm);
	Camera->SetFieldOfView(CameraParams.FieldOfView);
	/// Данное условие запрещаем камере вращаться относительно SpringArmComponent
	Camera->bUsePawnControlRotation = false;

	/// Запретим нашему персонажу поворачиваться в сторону камеры
	APawn::bUseControllerRotationPitch = false;
	APawn::bUseControllerRotationYaw = false;
	APawn::bUseControllerRotationRoll = false;

	Health = CreateDefaultSubobject<UMyHealth>("HealthComponent");
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

	WalkSpeed = std::make_unique<SprintParameters>(GetCharacterMovement()->MaxWalkSpeed, 2.0f, 1.0f);

	Health->OnDeath.AddUObject(this, &AMyCharacter::OnDeath);
	OnHealthChanged(Health->GetHealth());
	Health->OnHealthChanged.AddUObject(this, &AMyCharacter::OnHealthChanged);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(Health->IsDead()))
		RotationPlayerOnCursor();

	if (IsSprint)
	{
		WalkSpeed->Stamina -= 0.005f;
		SprintControl(Super::GetCharacterMovement()->MaxWalkSpeed, WalkSpeed->Acceleration * WalkSpeed->Stamina);
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::SprintStop);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}

void AMyCharacter::MoveForward(float Value)
{
	APawn::AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::MoveRight(float Value)
{
	APawn::AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::SprintStart()
{
	const auto& [Speed, Acceleration, Stamina] = *WalkSpeed;
	SprintControl(Speed, Acceleration);
	IsSprint = true;
}

void AMyCharacter::SprintStop()
{
	SprintControl(WalkSpeed->Speed);
	IsSprint = false;
	WalkSpeed->Stamina = 1.0f;
}

void AMyCharacter::SprintControl(float Speed, float Acceleration)
{
	if (auto MovementComponent = Super::GetCharacterMovement(); MovementComponent)
		MovementComponent->MaxWalkSpeed = Speed * Acceleration;
}

void AMyCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();

	ACharacter::PlayAnimMontage(DeathMontage);
	ACharacter::GetCharacterMovement()->DisableMovement();
	AActor::SetLifeSpan(5.0f);

	if (auto& controller = APawn::Controller; controller)
		controller->ChangeState(NAME_Spectating);
}

void AMyCharacter::OnHealthChanged(float NewHealth)
{
	// Таким образом, вывод жизней на экран будет происходить в начале игры, а также при каждом изменении показателей.
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void AMyCharacter::RotationPlayerOnCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		FHitResult ResultHit;
		PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		const float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(Super::GetActorLocation(), ResultHit.Location).Yaw;
		Super::SetActorRotation(FQuat{FRotator{0.0f, FindRotatorResultYaw, 0.0f}});
		if (CurrentCursor)
			CurrentCursor->SetWorldLocation(ResultHit.Location);
	}
}
