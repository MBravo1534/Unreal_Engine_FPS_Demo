// Developed by Manuel Bravo

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BUILDING_ESCAPE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = Projectile)
		float ProjectileLifeSpan = 3.f;
	UPROPERTY(EditAnywhere, Category = Projectile)
		float ProjectileSpeed = 4000.f;
	//Function will initialize the projectile's velocity in the shoot direction
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;
};
