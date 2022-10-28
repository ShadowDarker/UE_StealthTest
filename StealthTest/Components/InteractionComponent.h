// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StealthTest/Interfaces/InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHTEST_API UInteractionComponent : public UActorComponent, public IInteractionInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Interaction interface
	virtual void RegisterInteractable_Implementation(ABaseInteractable* Interactable) override;
	virtual void UnregisterInteractable_Implementation(ABaseInteractable* Interactable) override;

	/** Called to handle interaction with object */
	void HandleInteract() const;

	/** Called to cancel interaction with the object interacting with*/
	void HandleCancelInteract() const;

	/** Returns number of current registered interactables */
	uint8 CountInteractables() { return Interactables.Num(); }

private:

	/** Distance from center of object (normalized) allowed to interact */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	float DotProductValue = 0.7f;

	// Interaction objects
	TArray<TObjectPtr<ABaseInteractable>> Interactables;
	TObjectPtr<ABaseInteractable> SelectedInteractable;
	
	void UpdateBestInteractable();
	void GetBestInteractable();
	void SetBestInteractable(TObjectPtr<ABaseInteractable> Interactable);
};