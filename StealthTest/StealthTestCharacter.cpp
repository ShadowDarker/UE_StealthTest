// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthTestCharacter.h"
#include "StealthTest/Components/InteractionComponent.h"
#include "StealthTest/Interactables/Throwable.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AStealthTestCharacter

AStealthTestCharacter::AStealthTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetMesh(), "EyesSocket"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = true;

	// Create Interaction Component
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));

	// Create Throwable Scene Component
	ThrowableOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowableOrigin"));
	ThrowableOrigin->SetupAttachment(RootComponent);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStealthTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AStealthTestCharacter::Interact);
	PlayerInputComponent->BindAction("CancelInteract", IE_Pressed, this, &AStealthTestCharacter::CancelInteract);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AStealthTestCharacter::Aim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AStealthTestCharacter::CancelAim);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AStealthTestCharacter::Throw);
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &AStealthTestCharacter::ExitGame);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AStealthTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AStealthTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AStealthTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AStealthTestCharacter::LookUpAtRate);
}

void AStealthTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AStealthTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AStealthTestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsAiming)
	{
		AimProjection();
	}
}

void AStealthTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AStealthTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AStealthTestCharacter::Interact()
{
	GetInteractionComponent()->HandleInteract();
}

void AStealthTestCharacter::CancelInteract()
{
	GetInteractionComponent()->HandleCancelInteract();
}

void AStealthTestCharacter::Aim()
{
	if (IsValid(Throwable))
	{
		bIsAiming = true;
	}
}

void AStealthTestCharacter::CancelAim()
{
	if (IsValid(Throwable))
	{
		bIsAiming = false;
	}
}

void AStealthTestCharacter::Throw()
{
	if (IsValid(Throwable) && bIsAiming)
	{
		Throwable->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		const FRotator Rotation = GetFollowCamera()->GetForwardVector().Rotation() + FRotator(30.f, 0.f, 0.f);
		const FVector Direction = Rotation.Vector();
		
		Throwable->Throw(Direction, ThrowSpeed, ThrowSpeed);
		bIsAiming = false;
		Throwable = nullptr;
	}
}

void AStealthTestCharacter::ExitGame()
{
	UGameplayStatics::GetPlayerController(this, 0)->ConsoleCommand("quit");
}

void AStealthTestCharacter::AimProjection()
{
	const FRotator Rotation = GetFollowCamera()->GetForwardVector().Rotation() + FRotator(30.f, 0.f, 0.f);
	const FVector Direction = Rotation.Vector();
	
	FPredictProjectilePathParams ProjectilePathParams;
	ProjectilePathParams.StartLocation = GetThrowableOrigin()->GetComponentLocation();
	ProjectilePathParams.LaunchVelocity = Direction * ThrowSpeed;
	ProjectilePathParams.bTraceWithCollision = true;
	ProjectilePathParams.ProjectileRadius = 5.0f;
	ProjectilePathParams.TraceChannel = ECC_Visibility;
	ProjectilePathParams.ActorsToIgnore.AddUnique(this);
	ProjectilePathParams.ActorsToIgnore.AddUnique(Throwable);
	ProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	
	FPredictProjectilePathResult ProjectilePathResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), ProjectilePathParams, ProjectilePathResult);
}
