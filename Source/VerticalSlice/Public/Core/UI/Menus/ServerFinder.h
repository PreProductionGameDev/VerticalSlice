// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/FServerData.h"
#include "Core/Networking/NetworkInterface.h"
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
	void SetNetworkInterface(INetworkInterface* InNetworkInterface);

	
	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);
private:
	INetworkInterface* NetworkInterface;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedIndex;
	
	UFUNCTION()
	void JoinServer();
};
