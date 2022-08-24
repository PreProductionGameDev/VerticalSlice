// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/FServerData.h"
#include "ServerResult.generated.h"

/**
 *	The Widget for a Singular Server Result
 *	Added to the Server Finder Widget
 *	Stores its index in the list
 *	Need to tweak to display information properly	
 */
UCLASS()
class VERTICALSLICE_API UServerResult : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Setup the Functionality for Widget
	void Setup(class UServerFinder* InParent, uint32 InIndex);

	// Populate the Server Data
	void PopulateServerData(FServerData& ServerData);
	
private:
	// The ServerName displayed
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	// The Button of the Server Widget
	UPROPERTY(meta = (BindWidget))
	class UButton* RowButton;

	// The Buttons OnClicked Event. Handles setting the index
	UFUNCTION()
	void OnClicked();

	// The Parent Server Finder Class
	UPROPERTY()
	class UServerFinder* Parent;
	// The Index that this Result is in the Server Finder
	uint32 Index;
};
