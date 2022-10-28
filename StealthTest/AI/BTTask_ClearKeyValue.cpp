// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthTest/AI/BTTask_ClearKeyValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ClearKeyValue::UBTTask_ClearKeyValue()
{
	NodeName = TEXT("Clear Key Value");
}

// Clears the selected Key value
EBTNodeResult::Type UBTTask_ClearKeyValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->ClearValue(KeyValue.SelectedKeyName);

	return EBTNodeResult::Succeeded;
}