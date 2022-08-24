// 2022 ChronoOwl Studios


#include "Core/UI/Menus/ServerFinder.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Core/UI/Menus/ServerResult.h"

UServerFinder::UServerFinder(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	// Find and Store the Server Result Class
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerResultBPClass(TEXT("/Game/ShiitakeSorcerers/UI/Networking/WB_ServerResult"));
	if (!ensure(ServerResultBPClass.Class != nullptr))
	{
		return;
	}
	ServerRowClass = ServerResultBPClass.Class;
}

void UServerFinder::SetNetworkInterface(INetworkInterface* InNetworkInterface)
{
	NetworkInterface = InNetworkInterface;
	JoinButton->OnClicked.AddDynamic(this, &UServerFinder::JoinServer);
}

void UServerFinder::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// Clear the Existing server list
	ServerList->ClearChildren();

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
