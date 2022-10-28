// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "Throwable.generated.h"

UCLASS()
class STEALTHTEST_API AThrowable : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowable();

private:

	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ProjectileMovementComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

public:
	
	virtual void PostInitializeComponents() override;

	/** Called to throw this item */
	UFUNCTION(BlueprintCallable)
	void Throw(const FVector Direction, const float InitialSpeed, const float MaxSpeed) const;

	/** Called to reset functionality when finish launch and bounce */
	UFUNCTION(BlueprintCallable)    
	void ResetThrowable(const FHitResult& ImpactResult);

	/** Generate noise when hits and bounces */
	UFUNCTION(BlueprintCallable)
	void GenerateNoise(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	// Interact interface
	
	virtual void Interaction_Implementation(AActor* InteractInstigator) override;

	/** Returns Projectile Movement Component **/
	FORCEINLINE TObjectPtr<class UProjectileMovementComponent> GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
};