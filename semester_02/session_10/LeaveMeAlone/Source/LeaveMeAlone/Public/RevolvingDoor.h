// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "DoorInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RevolvingDoor.generated.h"

UCLASS()
class LEAVEMEALONE_API ARevolvingDoor : public AActor, public IDoorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARevolvingDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetupParameter(float MoveStep, EAxis::Type AxisSpin = EAxis::Z);

	UFUNCTION(BlueprintCallable)
	void Open() override;

	UFUNCTION(BlueprintCallable)
	void Close() override;

private:
	void Rotation();
	FTimerHandle TimerHandle{};
	// Pitch (Тангаж) - Вращение вокруг оси Y.
	// Управляет движением вверх и вниз. В коде C++ доступен через член структуры Pitch.
	// Yaw (Рыскание) - Вращение вокруг оси Z.
	// Управляет поворотом влево и вправо(по сторонам). В коде C++ доступен через член структуры Yaw.
	// Roll (Крен) - Вращение вокруг оси X.
	// Управляет наклоном из стороны в сторону. В коде C++ доступен через член структуры Roll.
	FRotator Parameters{};
	int Counter{};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Leaf")
	UStaticMeshComponent* LeafRight{};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Leaf")
	UStaticMeshComponent* LeafLeft{};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Frame")
	UStaticMeshComponent* Frame{};
};
