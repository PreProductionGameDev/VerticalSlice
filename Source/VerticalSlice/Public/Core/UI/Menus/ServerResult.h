// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/FServerData.h"
#include "ServerResult.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UServerResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PopulateServerData(FServerData& ServerData);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
};
