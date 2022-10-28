// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "StealthTest/StealthTestCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Perception/AISense_Hearing.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->bSimulationEnabled = false;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->bBounceAngleAffectsFriction = true;
	ProjectileMovementComponent->Bounciness = 0.2f;
	ProjectileMovementComponent->Friction = 1.f;
}

void AThrowable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AThrowable::ResetThrowable);
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AThrowable::GenerateNoise);
}

void AThrowable::Throw(const FVector Direction, const float InitialSpeed, const float MaxSpeed) const
{
	ProjectileMovementComponent->bSimulationEnabled = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->Velocity = ProjectileMovementComponent->InitialSpeed * Direction;
}

void AThrowable::ResetThrowable(const FHitResult& ImpactResult)
{
	ProjectileMovementComponent->bSimulationEnabled = false;
	bIsInteracting = false;
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
}

void AThrowable::GenerateNoise(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), ImpactResult.Location, 1, this);
}

void AThrowable::Interaction_Implementation(AActor* InteractInstigator)
{

	const TObjectPtr<AStealthTestCharacter> Player = Cast<AStealthTestCharacter>(InteractInstigator);

	if (IsValid(Player) && !IsValid(Player->GetThrowable()))
	{
		Super::Interaction_Implementation(InteractInstigator);
		
		Player->SetThrowable(this);
		AttachToComponent(Player->GetThrowableOrigin(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}