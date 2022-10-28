// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	GetCharacterMovement()->MaxWalkSpeed = 300;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SensingInterval = 0.05f;
	PawnSensingComponent->SightRadius = 1000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	HearingConfig->HearingRange = 1000.f;
	HearingConfig->DetectionByAffiliation.DetectAllFlags();

	AIPerceptionComponent->ConfigureSense(*HearingConfig);
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	
	if (BehaviorTree)
	{
		if (const TObjectPtr<AAIController> AIController = Cast<AAIController>(GetController()))
		{
			AIController->RunBehaviorTree(BehaviorTree);
			
			AIController->GetBlackboardComponent()->SetValueAsVector("StartLocation", GetActorLocation());
			AIController->GetBlackboardComponent()->SetValueAsVector("NoiseLocation", FVector::ZeroVector);
			AIController->GetBlackboardComponent()->SetValueAsFloat("PatrolRadius", PatrolRadius);
			AIController->GetBlackboardComponent()->SetValueAsFloat("SightRadius", PawnSensingComponent->SightRadius);
			AIController->GetBlackboardComponent()->SetValueAsFloat("SightAngle", PawnSensingComponent->GetPeripheralVisionAngle());
		}
	}
}

void AAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AAICharacter::Chase);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICharacter::CheckNoise);
}

void AAICharacter::Chase(APawn* Pawn)
{
	const TObjectPtr<AAIController> AIController = Cast<AAIController>(GetController());

	if (IsValid(AIController))
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerRef"), Pawn);
	}
}

void AAICharacter::CheckNoise(AActor* Actor, FAIStimulus Stimulus)
{
	const TObjectPtr<AAIController> AIController = Cast<AAIController>(GetController());

	if (IsValid(AIController))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(TEXT("NoiseLocation"), Stimulus.StimulusLocation);
	}
}