// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Networking/NetworkInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNetworkInterface(INetworkInterface* InNetworkInterface);
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* B_Host;
	UPROPERTY(meta = (BindWidget))
	class UButton* B_Join;

	UFUNCTION()
	void HostServer();

	INetworkInterface* NetworkingInterface;
};
