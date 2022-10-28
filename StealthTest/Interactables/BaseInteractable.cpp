// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractable.h"
#include "StealthTest/Components/InteractionComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseInteractable::ABaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->CustomDepthStencilValue = 2;

	RootComponent = Mesh;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->InitSphereRadius(128.f);
}

void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractable::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseInteractable::OnOverlapEnd);
}

void ABaseInteractable::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const TObjectPtr<UInteractionComponent> InteractionComponent = OtherActor->FindComponentByClass<UInteractionComponent>();
		
	if (IsValid(InteractionComponent) && InteractionComponent->Implements<UInteractionInterface>())
	{
		if (!InteractionComponent->IsActive())
		{
			InteractionComponent->SetActive(true);
		}

		if (const TObjectPtr<IInteractionInterface> InteractionInterface = Cast<IInteractionInterface>(InteractionComponent))
		{
			InteractionInterface->Execute_RegisterInteractable(InteractionComponent, this);
		}
	}
}

void ABaseInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	const TObjectPtr<UInteractionComponent> InteractionComponent = OtherActor->FindComponentByClass<UInteractionComponent>();
	
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->Execute_UnregisterInteractable(InteractionComponent, this);
		
		if (InteractionComponent->CountInteractables() <= 0)
		{
			InteractionComponent->SetActive(false);
		}
	}
}

void ABaseInteractable::Interaction_Implementation(AActor* InteractInstigator)
{
	IInteractionInterface::Interaction_Implementation(InteractInstigator);

	bIsInteracting = true;
}

void ABaseInteractable::CancelInteraction_Implementation(AActor* InteractInstigator)
{
	IInteractionInterface::CancelInteraction_Implementation(InteractInstigator);

	bIsInteracting = false;
}

void ABaseInteractable::SetHighlight(const bool Value) const
{
	Mesh->SetRenderCustomDepth(Value);
}