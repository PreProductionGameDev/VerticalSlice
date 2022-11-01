#pragma once

#include "Engine/DataTable.h"

#include "CosmeticItem.generated.h"

USTRUCT(BlueprintType)
struct FCosmeticItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTexture2D* Image;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USkeletalMesh* SkeletalMeshModel;
};
