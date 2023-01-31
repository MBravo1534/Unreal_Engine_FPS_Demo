// Developed by Manuel Bravo


#include "ElevatorOperator.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UElevatorOperator::UElevatorOperator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UElevatorOperator::BeginPlay()
{
	Super::BeginPlay();

	CurrentElevatorPosition = GetOwner()->GetActorLocation();
	StartingElevatorPosition = GetOwner()->GetActorLocation();

	if (StartingUp)
	{
		TargetElevatorZ = CurrentElevatorPosition.Z + moveDistance;
		GoingUp = true;
	}
	else
	{
		TargetElevatorZ = CurrentElevatorPosition.Z - moveDistance;
		GoingUp = false;
	}

	FindPressurePlate();

	ActorThatOperates = GetWorld()->GetFirstPlayerController()->GetPawn();

	FindAudioComponent();
}

void UElevatorOperator::FindLeaveTrigger()
{
	if (!LeaveTrigger)
	{
		UE_LOG(LogTemp, Warning, TEXT("The Elevator actor: %s is missing a leave trigger,\nelevator will not be disabled after leaving"), *GetOwner()->GetName());
	}
}
void UElevatorOperator::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("The Actor: %s is missing a PressurePlate!"), *GetOwner()->GetName());
	}
}

void UElevatorOperator::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing an audio component"), *GetOwner()->GetName())
	}
}

// Called every frame
void UElevatorOperator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get Current Game Time for reference
	float CurrentGameTime = GetWorld()->GetTimeSeconds();

	if (PressurePlate->IsOverlappingActor(ActorThatOperates) && !PlayerLeftElevator)
	{
		if (GoingUp)
		{
			ElevatorUp(DeltaTime);
			ElevatorLastUsed = GetWorld()->GetTimeSeconds();
		}
		else
		{
			ElevatorDown(DeltaTime);
			ElevatorLastUsed = GetWorld()->GetTimeSeconds();
		}
	}
}

void UElevatorOperator::ElevatorUp(float DeltaTime)
{
	if (!HasSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		HasSoundPlayed = true;
	}
	CurrentElevatorPosition.Z = FMath::Lerp(CurrentElevatorPosition.Z, TargetElevatorZ, DeltaTime * ElevatorSpeed);

	GetOwner()->SetActorLocation(CurrentElevatorPosition);
	if (CurrentElevatorPosition.Z == TargetElevatorZ)
	{
		GoingUp = false;
		TargetElevatorZ = StartingElevatorPosition.Z - moveDistance;
	}
}

void UElevatorOperator::ElevatorDown(float DeltaTime)
{
	if (!HasSoundPlayed && AudioComponent)
	{
		AudioComponent->Play();
		HasSoundPlayed = true;
	}
	CurrentElevatorPosition.Z = FMath::Lerp(CurrentElevatorPosition.Z, TargetElevatorZ, DeltaTime * ElevatorSpeed);

	GetOwner()->SetActorLocation(CurrentElevatorPosition);
	if (CurrentElevatorPosition.Z == TargetElevatorZ)
	{
		GoingUp = true;
		TargetElevatorZ = StartingElevatorPosition.Z;
	}
	
}

void UElevatorOperator::LeaveTriggerFunction()
{
	if (!LeaveTrigger) { return; }
	TArray<AActor*> OverlappingActors;
	LeaveTrigger->GetOverlappingActors(OverlappingActors);

	for (AActor* InTriggerCounter : OverlappingActors)
	{
		if (InTriggerCounter == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			PlayerLeftElevator = true;
			return;
		}
	}
	PlayerLeftElevator = false;
}

