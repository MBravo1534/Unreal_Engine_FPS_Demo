// Developed by Manuel Bravo

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame. You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//Door Slide Open initialization
	//Capture the Door's starting position and set targets
	CurrentDoorPosition = GetOwner()->GetActorLocation();
	TargetDoorY = CurrentDoorPosition.Y + 25;
	TargetDoorX = CurrentDoorPosition.X + 150;

	//Door Rotate Open initialization
	//Capture the Door's starting rotation and set targets
	CurrentDoorRotation = GetOwner()->GetActorRotation();
	InitialYaw = CurrentDoorRotation.Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = InitialYaw - OpenAngle;

	FindPressurePlate();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	FindAudioComponent();
}

void UOpenDoor::FindLeaveTrigger()
{
	if (!LeaveRoomTrigger)
	{
		UE_LOG(LogTemp, Warning, TEXT("The door actor: %s is missing a leave room trigger volume,\nno special event can occur after leaving room"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor: %s is missing a PressurePlate!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an audio component"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentGameTime = GetWorld()->GetTimeSeconds();

		if (LeaveRoomTrigger)
		{
			DoorCloseDelay = .05f;
			DoorCloseSpeed = 4.f;
		}
		else
		{
			DoorCloseDelay = 1.f;
			DoorCloseSpeed = 3.f;
		}

		if (TotalMassOfActors() > MassToOpenDoor && !PlayerWalkedThroughDoor || PressurePlate->IsOverlappingActor(ActorThatOpens))
		{
			OpenDoor(DeltaTime);
			//Get the time of opening in seconds
			DoorLastOpened = GetWorld()->GetTimeSeconds();
			LeaveTrigger();
		}
		else if (CurrentGameTime - DoorLastOpened >= DoorCloseDelay)
		{
			//If the door has been open longer than DoorCloseDelay
			CloseDoor(DeltaTime);
		}
		/*else if (PlayerWalkedThroughDoor && HasSoundPlayed)
		{
			CloseDoor(DeltaTime);
		}*/
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	if (!HasSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		HasSoundPlayed = true;
	}
	//UE_LOG(LogTemp, Warning, TEXT("The Rotation of the Door is: %s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("The Yaw of the Door is: %f"), GetOwner()->GetActorRotation().Yaw);
	if (SlidingDoor)
	{
		//Open the door by pushing back and sliding to the left
		CurrentDoorPosition.Y = FMath::Lerp(CurrentDoorPosition.Y, TargetDoorY, DeltaTime * SlideDoorOpenSpeed);
		GetOwner()->SetActorLocation(CurrentDoorPosition);
		CurrentDoorPosition.X = FMath::Lerp(CurrentDoorPosition.X, TargetDoorX, DeltaTime * SlideDoorOpenSpeed);
		GetOwner()->SetActorLocation(CurrentDoorPosition);
	}
	else
	{
		//Rotate the Door Open
		CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * RotateDoorOpenSpeed);
		CurrentDoorRotation.Yaw = CurrentYaw;
		GetOwner()->SetActorRotation(CurrentDoorRotation);
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	if (HasSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		HasSoundPlayed = false;
	}
	if (SlidingDoor)
	{
		CurrentDoorPosition.Y = FMath::Lerp(CurrentDoorPosition.Y, TargetDoorY - 25, DeltaTime * DoorCloseSpeed);
		GetOwner()->SetActorLocation(CurrentDoorPosition);
		CurrentDoorPosition.X = FMath::Lerp(CurrentDoorPosition.X, TargetDoorX - 150, DeltaTime * DoorCloseSpeed);
		GetOwner()->SetActorLocation(CurrentDoorPosition);
	}
	else
	{
		//Rotate the Door Closed
		CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
		CurrentDoorRotation.Yaw = CurrentYaw;
		GetOwner()->SetActorRotation(CurrentDoorRotation);
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//If there is no Pressure Plate Set, exit function
	if (!PressurePlate) { return TotalMass; }

	//Find All Overlapping Actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	//TArray<UPrimitiveComponent*> ActorComponents;

	//Add Up Their Masses
	/*for(int32 ActorCounter = 0; ActorCounter < OverlappingActors.Num(); ActorCounter++)
	{
		OverlappingActors[ActorCounter] -> GetComponents<UPrimitiveComponent>(OUT ActorComponents);
		for (int32 MassCompCounter = 0; MassCompCounter < ActorComponents.Num(); MassCompCounter++)
		{
			TotalMass += ActorComponents[MassCompCounter]->CalculateMass();
		}
	}*/

	for(AActor* Actor : OverlappingActors)
	{
		TotalMass+= Actor->FindComponentByClass<UPrimitiveComponent>() -> GetMass();
	}

	/*UE_LOG(LogTemp, Warning, TEXT("The Total items in the trigger are: %i"), OverlappingActors.Num());
	//UE_LOG(LogTemp, Warning, TEXT("The Total masses in the trigger are: %i"), ActorComponents.Num());
	UE_LOG(LogTemp, Warning, TEXT("The Total mass in the trigger is: %f"), TotalMass);*/

	return TotalMass;
}

void UOpenDoor::LeaveTrigger()
{
	if (!LeaveRoomTrigger) { return; }
	TArray<AActor*> OverlappingActors;
	LeaveRoomTrigger->GetOverlappingActors(OverlappingActors);

	for (AActor* InTriggerCounter : OverlappingActors)
	{
		if (InTriggerCounter == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			PlayerWalkedThroughDoor = true;
			return;
		}
	}
	PlayerWalkedThroughDoor = false;
}