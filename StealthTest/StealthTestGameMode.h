// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthTestGameMode.generated.h"

UCLASS(minimalapi)
class AStealthTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthTestGameMode();

	/** Called when an Enemy catches us */
	UFUNCTION(BlueprintCallable)
	void ResetGame();
};