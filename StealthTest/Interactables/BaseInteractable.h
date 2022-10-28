// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StealthTest/Interfaces/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "BaseInteractable.generated.h"

UCLASS()
class STEALTHTEST_API ABaseInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractable();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInteracting = false;

	/** Returns Mesh Component **/
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetMesh() const { return Mesh; }

	/** Set this object to highlight when can interact with */
	void SetHighlight(const bool Value) const;

	// Interaction interface
	virtual void Interaction_Implementation(AActor* InteractInstigator) override;
	virtual void CancelInteraction_Implementation(AActor* InteractInstigator) override;

protected:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

private:

	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;
};