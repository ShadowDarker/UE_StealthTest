// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearKeyValue.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTEST_API UBTTask_ClearKeyValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	UBTTask_ClearKeyValue();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "BT Task ClearKeyValue", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector KeyValue;
};