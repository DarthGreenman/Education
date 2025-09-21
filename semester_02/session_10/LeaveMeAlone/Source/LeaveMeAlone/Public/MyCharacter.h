// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include <memory>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

struct SprintParameters
{
	float Speed;
	float Acceleration;
	float Stamina;
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
	void MoveForward(float Value); /// будет отвечать за движение персонажа по оси X
	void MoveRight(float Value);   /// будет отвечать за движение персонажа по оси Y
	void SprintStart();
	void SprintStop();
	void SprintControl(float Speed, float Acceleration = 1.0f);
	void OnDeath();
	void OnHealthChanged(float NewHealth);
	void RotationPlayerOnCursor();

	CameraParameters CameraParams{55.0f, 1400.0f, -45.0f};
	std::unique_ptr<SprintParameters> WalkSpeed{};
	bool IsSprint{};
};
