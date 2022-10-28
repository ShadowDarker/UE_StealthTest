// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthTest/AI/BTTask_Catch.h"
#include "StealthTest/StealthTestGameMode.h"

UBTTask_Catch::UBTTask_Catch()
{
	NodeName = TEXT("Clear Key Value");
}

// Simple task to open a level, intended to call when AI reaches the player
EBTNodeResult::Type UBTTask_Catch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const TObjectPtr<AStealthTestGameMode> GameMode = Cast<AStealthTestGameMode>(GetWorld()->GetAuthGameMode());

	GameMode->ResetGame();

	return EBTNodeResult::Succeeded;
}