// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TimerHandle.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BasicCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UHealth;
class UAnimMontage;
class UWeaponSpecific;

namespace Helper
{
struct CameraParameters
{
	float FieldOfView;
	float SpringArmLength;
	float RotationAroundAxisY;
};

struct SprintParameters
{
	float WalkSpeed;
	float Acceleration;
};

struct StaminaParameters
{
	float UpperBound;
	float LowerBound;
	float DepletionRate;
	float RecoveryRate;
};
} // namespace Helper

UCLASS()
class LEAVEMEALONE_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	UHealth* GetHealthComponent() const { return Health; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UHealth* Health{};

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWeaponSpecific* Weapon{};

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	/**/
	void SprintStart();
	void SprintStop();
	void Sprinted(float Speed, float Acceleration = 1.0f, float Stamina = 100.0f);
	bool IsMoving() const;

	/**/
	float StaminaDepletion();
	void StaminaRecovery();
	bool StaminaRestored() const;
	bool StillHaveStamina() const;

	/**/
	void OnDeath();
	void OnHealthChanged(float NewHealth);
	void RotationPlayerOnCursor();

	/**/
	FTimerHandle TimerHandle{};
	const Helper::CameraParameters CameraParams{55.0f, 1400.0f, -45.0f};
	const Helper::SprintParameters SprintParams{500.0f, 2.0f};
	const Helper::StaminaParameters StaminaParams{100.0f, 30.f, 0.1f, 10.0f};

	float Stamina{};
	bool bIsSprint{};
};
