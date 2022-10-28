// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectPlayer.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHTEST_API UBTService_DetectPlayer : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_DetectPlayer();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	UPROPERTY(EditAnywhere, Category = "BT Service DetectPlayer", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "BT Service DetectPlayer", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector RadiusKey;

	/** Checks if location A is inside range of location B based on a range */
	static bool IsLocationInRange(const FVector Location, const FVector Origin, const float Radius);
};