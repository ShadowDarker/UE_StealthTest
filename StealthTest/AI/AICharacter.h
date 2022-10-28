// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "AICharacter.generated.h"

UCLASS()
class STEALTHTEST_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PawnSensingComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnSensingComponent> PawnSensingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PawnSensingComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PawnSensingComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	/** Behaviour Tree asset */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	// AI Attributes
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float PatrolRadius = 1000.f;
	FVector StartLocation;
	TObjectPtr<APawn> TargetPawn;
	FPathFollowingResult HearingPathFollowingResult;
	FPathFollowingResult ChasePathFollowingResult;

	/** Called when AI sees target pawn*/
	UFUNCTION(BlueprintCallable)
	void Chase(APawn* Pawn);
	/** Called when AI hears a generated noise */
	UFUNCTION(BlueprintCallable)
	void CheckNoise(AActor* Actor, FAIStimulus Stimulus);

public:	

	/** Returns Pawn Sensing Component */
	FORCEINLINE TObjectPtr<class UPawnSensingComponent> GetPawnSensingComponent() const { return PawnSensingComponent; }
	/** Returns AI Perception Component */
	FORCEINLINE TObjectPtr<class UAIPerceptionComponent> GetAIPerceptionComponent() const { return AIPerceptionComponent; }
};