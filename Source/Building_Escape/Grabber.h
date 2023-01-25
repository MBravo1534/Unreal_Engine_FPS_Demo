// Developed by Manuel Bravo

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine\InputActionDelegateBinding.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	void Grab();
	void Release();
	void FindPhysicsHandle();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float Reach = 100.f;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;
	FVector LineTraceEnd;
	FVector PlayerViewpointLocation;


	void SetUpInputComponent();
	void CalculateRayCastVector();

	//Return the first Actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;
};
