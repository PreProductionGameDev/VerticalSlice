// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/NetworkInterface.h"
#include "Core/Networking/FServerData.h"
#include "MainMenu.generated.h"



/**
 *	The Main Menu UI, Used to Create and Join Sessions
 */
UCLASS()
class SHIITAKESHOWDOWN_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	// Set the Network Interface Pointer. Will be set to the GameInstance
	void SetNetworkInterface(INetworkInterface* InNetworkInterface);

	void SetServerList(TArray<FServerData> ServerNames);

	void RemoveUI();
	
protected:
	// Initialize All components
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UServerFinder* WB_ServerList;

	UFUNCTION(BlueprintCallable)
	void HostServer();

	UFUNCTION(BlueprintCallable)
	void JoinServer();

	INetworkInterface* NetworkingInterface;
};
