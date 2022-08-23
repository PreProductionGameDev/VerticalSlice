// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/FServerData.h"
#include "ServerFinder.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UServerFinder : public UUserWidget
{
	GENERATED_BODY()

public:
	UServerFinder(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);
private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	TSubclassOf<UUserWidget> ServerRowClass;
};
