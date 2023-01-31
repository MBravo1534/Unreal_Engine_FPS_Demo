// Developed by Manuel Bravo

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "ElevatorOperator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UElevatorOperator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElevatorOperator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FindAudioComponent();
	void FindPressurePlate();
	void FindLeaveTrigger();
	bool PlayerLeftElevator = false;
	bool HasSoundPlayed = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elevator Properties")
		bool StartingUp = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Elevator Properties")
		float moveDistance = 500;
private:
	float TargetElevatorZ;
	FVector CurrentElevatorPosition;
	FVector StartingElevatorPosition;

	bool GoingUp = false;

	float ElevatorLastUsed = 0.f;

	UPROPERTY(EditAnywhere)
		float ElevatorActivationDelay = 1.f;

	UPROPERTY(EditAnywhere)
		float ElevatorSpeed = 1.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* LeaveTrigger = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOperates = nullptr;

	void ElevatorUp(float DetlaTime);
	void ElevatorDown(float DeltaTime);
	void LeaveTriggerFunction();

	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;
};
