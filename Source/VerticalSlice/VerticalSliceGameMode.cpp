// Copyright Epic Games, Inc. All Rights Reserved.

#include "VerticalSliceGameMode.h"
#include "VerticalSliceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVerticalSliceGameMode::AVerticalSliceGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ShiitakeSorcerers/Character/BP_FPCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AVerticalSliceGameMode::ServerTravel(FString Map)
{
	UWorld* World = GetWorld();
	bUseSeamlessTravel = true;
	World->ServerTravel(Map );
}
