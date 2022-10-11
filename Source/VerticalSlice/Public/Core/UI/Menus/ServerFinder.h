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

	virtual void NativeConstruct() override;
	
	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);
protected:
	INetworkInterface* NetworkInterface;

public:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ServerBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* RoomName;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* GameMode;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Map;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> ServerRowClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UUserWidget> ServerFailureMessage;
	
	TOptional<uint32> SelectedIndex;

public:

	UFUNCTION()
	void ResetSelected();

	UFUNCTION()
	void UpdateServerSettings();

	UFUNCTION( BlueprintImplementableEvent)
	void UpdateIcons();
	
	UFUNCTION(BlueprintCallable)
	void JoinServer();

	UFUNCTION(BlueprintCallable)
	void RefreshServerList();
};
