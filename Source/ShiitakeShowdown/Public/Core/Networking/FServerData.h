// 2022 ChronoOwl Studios

#pragma once

#include "Core/Gamemodes/Lobby/LobbyInfoStructs.h"
#include "FServerData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	int32 CurrentPlayers = 0;
	uint16 MaxPlayers;

	FString CurrentGameModeInfo;
	FString CurrentMapInfo;
	
};