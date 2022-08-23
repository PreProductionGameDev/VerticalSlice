// 2022 ChronoOwl Studios


#include "Core/UI/Menus/MainMenu.h"
#include "Components/Button.h"
#include "Core/UI/Menus/ServerFinder.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UMainMenu::SetNetworkInterface(INetworkInterface* InNetworkInterface)
{
	this->NetworkingInterface = InNetworkInterface;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	if (!ensure(WB_ServerList != nullptr)) return;

	WB_ServerList->SetServerList(ServerNames);
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	// TODO: SETUP
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
