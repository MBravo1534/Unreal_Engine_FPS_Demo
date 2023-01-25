// Developed by Manuel Bravo

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "UObject/ObjectMacros.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();
	void FindLeaveTrigger();
	bool PlayerWalkedThroughDoor = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Open Status")
		bool HasSoundPlayed = false;

private:
	float InitialYaw;
	float CurrentYaw;
	float TargetDoorY;
	float TargetDoorX;
	FVector CurrentDoorPosition;
	FRotator CurrentDoorRotation;

	UPROPERTY(EditAnywhere)
		bool SlidingDoor;

	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
		float MassToOpenDoor = 50.f;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
		float SlideDoorOpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
		float RotateDoorOpenSpeed = .5f;

	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 3.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* LeaveRoomTrigger = nullptr;

	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens = nullptr;

	void OpenDoor(float DetlaTime);
	void CloseDoor(float DeltaTime);
	void LeaveTrigger();

	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;
};
