// Developed by Manuel Bravo

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSCharacter.generated.h"

UCLASS()
class BUILDING_ESCAPE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//this will handle character movement forward and backwards
	UFUNCTION()
		void MoveForward(float Distance);

	//this will handle sprinting
	UFUNCTION()
		void StartSprint();

	UFUNCTION()
		void EndSprint();

	//this will handle character movement right and left
	UFUNCTION()
		void MoveRight(float Distance);

	//Sets Jump flag when key is Pressed
	UFUNCTION()
		void StartJump();

	//Sets Jump flag when key is Released
	UFUNCTION()
		void StopJump();

	//This will handle shooting
	UFUNCTION()
		void Fire();

	//*This will handle crouching
	UFUNCTION()
		void StartCrouch();
	
	UFUNCTION()
		void EndCrouch();

	//FPS Camera
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Qualities")
		float MaxSprintTime = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Qualities")
		float SprintWaitTime = 4.f;

	//Gun Muzzle's offset from the camera location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

private:
	bool CanSprint = true;
	bool IsSprinting = false;
	float SprintTimeElapsed;
	float SprintStopTime;
};
