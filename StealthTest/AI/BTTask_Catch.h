// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Catch.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTEST_API UBTTask_Catch : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Catch();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
