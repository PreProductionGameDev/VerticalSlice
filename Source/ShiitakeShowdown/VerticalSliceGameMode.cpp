// Copyright Epic Games, Inc. All Rights Reserved.

#include "VerticalSliceGameMode.h"
#include "VerticalSliceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVerticalSliceGameMode::AVerticalSliceGameMode()
{
	// NOTE: THIS CAUSED A CIRCULAR INCLUDE, NOT ENTIRELY SURE WHY
	// COMMENTING IT OUT FIXED OUR PROBLEMS. ALSO DONT ASK WHY
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ShiitakeSorcerers/Character/BP_FPCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}

void AVerticalSliceGameMode::ServerTravel(FString Map)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Map);
	UWorld* World = GetWorld();
	bUseSeamlessTravel = true;
	World->ServerTravel(Map);
}

TArray<TSubclassOf<ABWeapon>> AVerticalSliceGameMode::GetDefaultInventory()
{
	return DefaultInventoryWeapons;
}
