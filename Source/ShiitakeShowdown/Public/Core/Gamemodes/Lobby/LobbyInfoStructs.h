// 2022 ChronoOwl Studios

#pragma once
#include "Engine/DataTable.h"

#include "LobbyInfoStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGameModeInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AVerticalSliceGameMode* GameMode = nullptr;
};

USTRUCT(BlueprintType)
struct FMapInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* PreviewImage = nullptr;
};
