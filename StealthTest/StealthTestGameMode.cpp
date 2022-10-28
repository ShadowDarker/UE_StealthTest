// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthTestGameMode.h"
#include "StealthTestCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AStealthTestGameMode::AStealthTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

/** Opens the menu level */
void AStealthTestGameMode::ResetGame()
{
	UGameplayStatics::OpenLevel(this, "Menu");
}