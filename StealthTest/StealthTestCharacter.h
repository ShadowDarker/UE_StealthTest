// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StealthTest/Interfaces//InteractionInterface.h"
#include "GameFramework/Character.h"
#include "StealthTestCharacter.generated.h"

UCLASS(config=Game)
class AStealthTestCharacter : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Interaction Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InteractionComponent, meta = (AllowPrivateAccess = "true"))
	class UInteractionComponent* InteractionComponent;

	/** Scene Component for reference of throwable items */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ThrowableOrigin, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ThrowableOrigin;
	
public:
	AStealthTestCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:

	virtual void Tick(float DeltaSeconds) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Called via input to interact with objects */
	void Interact();

	/** Called via input to cancel the interaction with objects */
	void CancelInteract();

	/** Called via input to aim when having a throwable item */
	void Aim();

	/** Called via input to cancel the aim when having a throwable item */
	void CancelAim();

	/** Called via input to throw an item */
	void Throw();

	/** Called via input to exit from game */
	void ExitGame();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Throwables
	TObjectPtr<class AThrowable> Throwable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Throw, meta = (AllowPrivateAccess = "true") )
	float ThrowSpeed = 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Throw, meta = (AllowPrivateAccess = "true") )
	float ThrowAngle = 30.f;

	/** Projects the trajectory of the launch */
	void AimProjection();
	
	bool bIsAiming : 1;

public:
	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Returns Interaction Component **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	/** Returns Throwable Scene Component **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE USceneComponent* GetThrowableOrigin() const { return ThrowableOrigin; }

	/** Returns Throwable Object **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AThrowable* GetThrowable() const { return Throwable; }

	/** Sets Throwable Object **/
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetThrowable(class AThrowable* NewThrowable) { Throwable = NewThrowable; }
};