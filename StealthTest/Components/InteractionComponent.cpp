// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "StealthTest/Interactables/BaseInteractable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateBestInteractable();
}

void UInteractionComponent::UpdateBestInteractable()
{
	if (Interactables.Num() > 0)
	{
		GetBestInteractable();
	}
	else
	{
		SetBestInteractable(nullptr);
	}
}

void UInteractionComponent::GetBestInteractable()
{
	TObjectPtr<ABaseInteractable> ActiveInteractable;

	float DpValue = -1.f;
	
	for (const TObjectPtr<ABaseInteractable> Interactable : Interactables)
	{
		FVector CenterMass = Interactable->GetMesh()->GetCenterOfMass();
		FVector Location = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
		FVector FwdVector = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();

		FVector Distance = (Location - CenterMass).GetSafeNormal();
		
		const float DP = -FVector::DotProduct(Distance, FwdVector);
		
		if (DP >= DotProductValue && DP >= DpValue)
		{
			DpValue = DP;
			ActiveInteractable = Interactable;
		}
	}

	if (ActiveInteractable)
	{
		SetBestInteractable(ActiveInteractable);
	}
	else
	{
		SetBestInteractable(nullptr);
	}
}

void UInteractionComponent::SetBestInteractable(TObjectPtr<ABaseInteractable> Interactable)
{
	if (IsValid(Interactable))
	{
		if (Interactable != SelectedInteractable)
		{
			if (SelectedInteractable)
			{
				SelectedInteractable->SetHighlight(false);
				SelectedInteractable = Interactable;
			}

			SelectedInteractable = Interactable;
		}

		if (!SelectedInteractable->bIsInteracting)
		{
			SelectedInteractable->SetHighlight(true);
		}
		else
		{
			SelectedInteractable->SetHighlight(false);
		}
	}
	else
	{
		if (IsValid(SelectedInteractable))
		{
			SelectedInteractable->SetHighlight(false);
			SelectedInteractable = nullptr;
		}
	}
}

void UInteractionComponent::HandleInteract() const
{
	if (IsValid(SelectedInteractable) && !SelectedInteractable->bIsInteracting)
	{
		SelectedInteractable->Execute_Interaction(SelectedInteractable, GetOwner());
	}
}

void UInteractionComponent::HandleCancelInteract() const
{
	if (IsValid(SelectedInteractable) && SelectedInteractable->bIsInteracting)
	{
		SelectedInteractable->Execute_CancelInteraction(SelectedInteractable, GetOwner());
	}
}

void UInteractionComponent::RegisterInteractable_Implementation(ABaseInteractable* Interactable)
{
	IInteractionInterface::RegisterInteractable_Implementation(Interactable);

	if (IsValid(Interactable))
	{
		Interactables.AddUnique(Interactable);
	}
}

void UInteractionComponent::UnregisterInteractable_Implementation(ABaseInteractable* Interactable)
{
	IInteractionInterface::UnregisterInteractable_Implementation(Interactable);

	if (IsValid(Interactable))
	{
		Interactable->SetHighlight(false);
		Interactables.Remove(Interactable);
	}
}