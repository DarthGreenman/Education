// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#include "Hero.h"

#include "Health.h"
#include "WeaponSpecific.h"

#include <limits>

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSprint, All, All);

// Sets default values
AHero::AHero()
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

	Health = UObject::CreateDefaultSubobject<UHealth>("HealthComponent");
	Weapon = UObject::CreateDefaultSubobject<UWeaponSpecific>("Weapon");
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(Health->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	if (bIsSprint && StillHaveStamina())
	{
		Sprinted(SprintParams.WalkSpeed, SprintParams.Acceleration, StaminaDepletion());
	}

	if (StaminaRestored())
	{
		AActor::GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

// Called to bind functionality to input
void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (Super::SetupPlayerInputComponent(PlayerInputComponent); IsValid(PlayerInputComponent))
	{
		PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AHero::SprintStart);
		PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AHero::SprintStop);
		PlayerInputComponent->BindAction("Fire", IE_Pressed, Weapon, &UWeaponSpecific::Fire);
		PlayerInputComponent->BindAction("Fire", IE_Released, Weapon, &UWeaponSpecific::ReleaseTheTrigger);
		PlayerInputComponent->BindAction("Reload", IE_Pressed, Weapon, &UWeaponSpecific::Reload);

		PlayerInputComponent->BindAxis("MoveForward", this, &AHero::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AHero::MoveRight);
	}
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	/// В данной логике мы проверяем задан ли материал для нашего курсора и если он задан, тогда мы используем функцию,
	/// реализованную в библиотеке GameplayStatics, которая называется SpawnDecalAtLocation.
	/// В качестве параметров она принимает указатель на мир, материал, размеры курсора, а также точку спауну.
	/// В настоящее время мы размещаем курсор в нулевой координате нашей игры.

	if (IsValid(CursorMaterial))
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(Super::GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	Health->OnDeath.AddUObject(this, &AHero::OnDeath);
	OnHealthChanged(Health->GetHealth());
	Health->OnHealthChanged.AddUObject(this, &AHero::OnHealthChanged);

	Stamina = StaminaParams.UpperBound;
}

void AHero::ShowActorInformation() const
{
	UE_LOG(LogSprint, Display, TEXT("Скорость:%f"), Super::GetCharacterMovement()->GetMaxSpeed());
	UE_LOG(LogSprint, Display, TEXT("Выносливость:%f"), Stamina / 100);
}
void AHero::MoveForward(float AxisValue)
{
	APawn::AddMovementInput(AActor::GetActorForwardVector(), AxisValue);
}

void AHero::MoveRight(float AxisValue)
{
	APawn::AddMovementInput(AActor::GetActorRightVector(), AxisValue);
}

void AHero::SprintStart()
{
	if (IsMoving() && StaminaRestored())
	{
		Sprinted(SprintParams.WalkSpeed, SprintParams.Acceleration);
	}
}

void AHero::SprintStop()
{
	Sprinted(SprintParams.WalkSpeed);
	bIsSprint = false;
	AActor::GetWorldTimerManager().SetTimer(TimerHandle, this, &AHero::StaminaRecovery, 0.50f, true);
}

void AHero::Sprinted(float Speed, float Acceleration, float CurrentStamina)
{
	if (auto MovementComponent = Super::GetCharacterMovement(); IsValid(MovementComponent))
	{
		MovementComponent->MaxWalkSpeed = Speed * Acceleration * CurrentStamina / StaminaParams.UpperBound;
		bIsSprint = true;
	}
}

bool AHero::IsMoving() const
{
	return true; // Реализовать логику
}

float AHero::StaminaDepletion()
{
	Stamina -= StaminaParams.DepletionRate;
	if (Stamina < StaminaParams.LowerBound)
	{
		Stamina = StaminaParams.LowerBound;
	}
	return Stamina;
}

void AHero::StaminaRecovery()
{
	Stamina += StaminaParams.RecoveryRate;
	if (Stamina > StaminaParams.UpperBound)
	{
		Stamina = StaminaParams.UpperBound;
	}
}

bool AHero::StaminaRestored() const
{
	const static float Epsilon{std::numeric_limits<float>::epsilon()};
	return std::abs(Stamina - StaminaParams.UpperBound) < Epsilon || (Stamina - StaminaParams.UpperBound) > Epsilon;
}

bool AHero::StillHaveStamina() const
{
	const static float Epsilon{std::numeric_limits<float>::epsilon()};
	return (Stamina - StaminaParams.LowerBound) > Epsilon;
}

void AHero::OnDeath()
{
	if (IsValid(CurrentCursor))
	{
		CurrentCursor->DestroyRenderState_Concurrent();
	}

	ACharacter::PlayAnimMontage(DeathMontage);
	ACharacter::GetCharacterMovement()->DisableMovement();
	AActor::SetLifeSpan(5.0f);

	if (TObjectPtr<AController> & controller{APawn::Controller}; IsValid(controller))
	{
		controller->ChangeState(NAME_Spectating);
	}
}

void AHero::OnHealthChanged(float NewHealth)
{
	// Таким образом, вывод жизней на экран будет происходить в начале игры, а также при каждом изменении показателей.
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void AHero::RotationPlayerOnCursor()
{
	if (APlayerController * PlayerController{UGameplayStatics::GetPlayerController(GetWorld(), 0)}; IsValid(PlayerController))
	{
		FHitResult ResultHit{};
		PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		const double FindRotatorResultYaw{UKismetMathLibrary::FindLookAtRotation(Super::GetActorLocation(), ResultHit.Location).Yaw};
		Super::SetActorRotation(FQuat{FRotator{0.0f, FindRotatorResultYaw, 0.0f}});
		if (IsValid(CurrentCursor))
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}
