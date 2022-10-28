// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthTest/AI/BTService_DetectPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.1f;
}

// If detects player sets his position, otherwise we clear his reference
void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const TObjectPtr<APawn> PlayerPawn = Cast<APawn>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));

	if (IsValid(PlayerPawn))
	{
		const FVector OriginLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		const FVector PlayerLocation = PlayerPawn->GetActorLocation();
		const float Radius = BlackboardComponent->GetValueAsFloat(RadiusKey.SelectedKeyName);
		
		if (IsLocationInRange(PlayerLocation, OriginLocation, Radius))
		{
			BlackboardComponent->SetValueAsVector("TargetLocation", PlayerLocation);
		}
		else
		{
			BlackboardComponent->ClearValue(TargetKey.SelectedKeyName);
		}
	}
}

bool UBTService_DetectPlayer::IsLocationInRange(const FVector Location, const FVector Origin, const float Radius)
{
	return FVector::DistSquared(Origin, Location) <= FMath::Square(Radius);
}