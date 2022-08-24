// 2022 ChronoOwl Studios


#include "Core/UI/Menus/MainMenu.h"
#include "Components/Button.h"
#include "Core/UI/Menus/ServerFinder.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UMainMenu::SetNetworkInterface(INetworkInterface* InNetworkInterface)
{
	// Set the Networking Interface for the Main Menu and the Server List
	this->NetworkingInterface = InNetworkInterface;
	if (WB_ServerList != nullptr)
	{
		WB_ServerList->SetNetworkInterface(NetworkingInterface);
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	// Send the Server Data to the Server List
	if (!ensure(WB_ServerList != nullptr)) return;
	WB_ServerList->SetServerList(ServerNames);
}

void UMainMenu::RemoveUI()
{
	// Clear the UI
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// Re-enable User Input
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	// TODO: SETUP THE REST OF THE MAIN MENU
	if (!ensure(B_Host != nullptr)) return false;
	B_Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(B_Join != nullptr)) return false;
	B_Join->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}

void UMainMenu::HostServer()
{
	if (NetworkingInterface != nullptr)
	{
		NetworkingInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (NetworkingInterface != nullptr)
	{
		NetworkingInterface->RefreshServerList();
	}
}
