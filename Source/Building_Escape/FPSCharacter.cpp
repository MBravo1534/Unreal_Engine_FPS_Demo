// Developed by Manuel Bravo

#include "Building_Escape.h"
#include "Grabber.h"
#include "FPSCharacter.h"
#include "Projectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create FPS Camera
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//Attach Camera to capsule
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	//Position camera slightlu above the eyes
	FPSCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight));
	//Allow the pawn to control camera rotation
	FPSCameraComponent->bUsePawnControlRotation = true;

	//Configuration of the skeletal mesh

	//Create first person mesh component for the owning person (arms)
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FPSMesh->SetOnlyOwnerSee(true);

	//Attach FPSMesh (arms) to Camera
	FPSMesh->SetupAttachment(FPSCameraComponent);

	//Mesh is not visible to player
	GetMesh()->SetOwnerNoSee(true);
	//Do not have Main mesh cast shadows
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;

	//Arm meshes do not cast shadows
	FPSMesh->SetVisibility(true);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("We are using FPSCharacter."));
	
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//bind actions to input
	//This dictactes the movement of the character
	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSCharacter::EndSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSCharacter::EndCrouch);

	//This dictates the "looking" for the character
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
}


void AFPSCharacter::MoveForward(float Distance)
{
	//Determine right vector relative to viewpoint and move in that direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Distance);
}

void AFPSCharacter::StartSprint()
{
	if (CanSprint)
	{
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed += 400;
		SprintTimeElapsed = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("I can sprint"));
	}
	else
	{
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600;
		UE_LOG(LogTemp, Warning, TEXT("I cannot sprint"));
	}
}

void AFPSCharacter::EndSprint()
{
	if (GetCharacterMovement()->MaxWalkSpeed > 600)
	{
		GetCharacterMovement()->MaxWalkSpeed -= 400;
	}
}

void AFPSCharacter::MoveRight(float Distance)
{
	//Determine Forward vector relative to viewpoint and move in that direction
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Distance);
}

void AFPSCharacter::StartJump()
{
	//set Jump bool for blueprint to true
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	//set Jump bool for blueprint to false
	bPressedJump = false;
}

void AFPSCharacter::Fire()
{
	if (ProjectileClass)
	{
		//get camera transform
		FVector CameraLocation;

		FRotator CameraRotation;

		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		//skew the aim to be slightly upwards
		MuzzleRotation.Pitch += 10.f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			//Spawn projectile at the muzzle
			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile)
			{
				//Set the projectile's initial trajectory
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void AFPSCharacter::StartCrouch()
{
	if(!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed -= 170;
		//UE_LOG(LogTemp, Warning, TEXT("my current walk speed is: %i"), GetCharacterMovement()->MaxWalkSpeed);
	}
}

void AFPSCharacter::EndCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
		GetCharacterMovement()->MaxWalkSpeed += 170;
		//UE_LOG(LogTemp, Warning, TEXT("my current walk speed is: %i"), GetCharacterMovement()->MaxWalkSpeed);
	}
}


// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (IsSprinting && CurrentTime - SprintTimeElapsed >= MaxSprintTime)
	{
		CanSprint = false;
		EndSprint();
		SprintStopTime = GetWorld()->GetTimeSeconds();
	}
	else if (!CanSprint && CurrentTime - SprintStopTime >= SprintWaitTime)
	{
		CanSprint = true;
	}
}

