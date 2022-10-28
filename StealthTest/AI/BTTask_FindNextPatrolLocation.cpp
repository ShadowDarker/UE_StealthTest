// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthTest/AI/BTTask_FindNextPatrolLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindNextPatrolLocation::UBTTask_FindNextPatrolLocation()
{
	NodeName = TEXT("Find Next Patrol Location");
}

// Tries to find a location inside a range of navigable radius from a origin point
EBTNodeResult::Type UBTTask_FindNextPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const TObjectPtr<UBlackboardComponent> BlackboardComponent = OwnerComp.GetBlackboardComponent();

	const FVector Origin = BlackboardComponent->GetValueAsVector(OriginKey.SelectedKeyName);
	const float Radius = BlackboardComponent->GetValueAsFloat(RadiusKey.SelectedKeyName);

	FNavLocation NavLocation;

	if (UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(Origin, Radius, NavLocation))
	{
		BlackboardComponent->SetValueAsVector(PatrolLocationKey.SelectedKeyName, NavLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}