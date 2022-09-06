// 2022 ChronoOwl Studios


#include "Core/UI/Menus/ServerFinder.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Core/UI/Menus/ServerResult.h"

void UServerFinder::NativeConstruct()
{
	Super::NativeConstruct();
	SetNetworkInterface(Cast<INetworkInterface>(GetGameInstance()));
}

UServerFinder::UServerFinder(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	
}

void UServerFinder::SetNetworkInterface(INetworkInterface* InNetworkInterface)
{
	if(!InNetworkInterface)
	{
		return;
	}
	NetworkInterface = InNetworkInterface;
	if(!JoinButton)
	{
		return;
	}
	JoinButton->OnClicked.AddDynamic(this, &UServerFinder::JoinServer);
	if(!RefreshButton)
	{

		return;
	}
	RefreshButton->OnClicked.AddDynamic(this, &UServerFinder::RefreshServerList);
}

void UServerFinder::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// Clear the Existing server list
	ServerList->ClearChildren();

	if (ServerNames.Num() == 0)
	{
		UUserWidget* ErrorMessage = CreateWidget<UUserWidget>(World, ServerFailureMessage);
		if (!ensure(ErrorMessage != nullptr)) return;

		ServerList->AddChild(ErrorMessage);
	}
	
	// Cycle through all the server data received, Create widget and populate data.
	uint32 i = 0;
	for (FServerData& ServerData : ServerNames)
	{
		// Create the Server Row Widget
		UServerResult* Row = CreateWidget<UServerResult>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->Setup(this, i);
		++i;
		Row->PopulateServerData(ServerData);
		
		ServerList->AddChild(Row);
	}
}

void UServerFinder::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UServerFinder::JoinServer()
{
	// Call the network interface to join a valid server
	if (NetworkInterface != nullptr && SelectedIndex.IsSet())
	{
		NetworkInterface->Join(SelectedIndex.GetValue());
	}
}

void UServerFinder::RefreshServerList()
{
	if (NetworkInterface != nullptr)
	{
		ServerList->ClearChildren();
		NetworkInterface->RefreshServerList(this);
	}
}
