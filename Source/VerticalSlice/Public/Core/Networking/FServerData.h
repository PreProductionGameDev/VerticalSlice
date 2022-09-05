// 2022 ChronoOwl Studios

#pragma once

#include "Core/Gamemodes/Lobby/LobbyInfoStructs.h"
#include "FServerData.generated.h"

/**
 * 
 */
USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;

	FGameModeInfo CurrentGameModeInfo;
	FMapInfo CurrentMapInfo;
	
};