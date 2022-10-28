// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNextPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTEST_API UBTTask_FindNextPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	UBTTask_FindNextPatrolLocation();
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "BT Task FindNextPatrolLocation", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector OriginKey;

	UPROPERTY(EditAnywhere, Category = "BT Task FindNextPatrolLocation", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector RadiusKey;

	UPROPERTY(EditAnywhere, Category = "BT Task FindNextPatrolLocation", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector PatrolLocationKey;
};