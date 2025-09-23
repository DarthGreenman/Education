// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UMyHealth;
class UAnimMontage;

struct CameraParameters
{
	float FieldOfView;
	float SpringArmLength;
	float RotationAroundAxisY;
};

USTRUCT(BlueprintType)
struct FSprintParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration;
};

USTRUCT(BlueprintType)
struct FStaminaParameters
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DepletionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryRate;
};

UCLASS()
class LEAVEMEALONE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UMyHealth* GetHealthComponent() const { return Health; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	FSprintParameters GetSprintParameters() const { return SprintParameters; }

	UFUNCTION(BlueprintCallable)
	void ShowActorInformation() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera{};

	UPROPERTY()
	UDecalComponent* CurrentCursor{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize{20.0f, 40.0f, 40.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	UMyHealth* Health{};

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage{};

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void SprintStart();
	void SprintStop();
	void Sprint(float Speed, float Acceleration = 1.0f, float Stamina = 1.0f);
	auto IsSprint() const { return Super::GetCharacterMovement()->GetMaxSpeed() > SprintParameters.WalkSpeed; }

	float StaminaDepletion();
	void StaminaRecovery();
	auto StaminaRestored() const { return Stamina == StaminaParameters.Stamina; }
	auto StaminaLowerLimit() const { return StaminaParameters.Stamina / SprintParameters.Acceleration; } 

	void OnDeath();
	void OnHealthChanged(float NewHealth);
	void RotationPlayerOnCursor();

	CameraParameters CameraParams{55.0f, 1400.0f, -45.0f};
	const FSprintParameters SprintParameters{500.0f, 2.0f};
	const FStaminaParameters StaminaParameters{100.0f, 0.1f, 10.0f};
	float Stamina{};
};
