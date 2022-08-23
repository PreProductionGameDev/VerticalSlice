// 2022 ChronoOwl Studios


#include "Core/UI/Menus/MainMenu.h"
#include "Components/Button.h"

void UMainMenu::SetNetworkInterface(INetworkInterface* InNetworkInterface)
{
	this->NetworkingInterface = InNetworkInterface;
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
	
	return true;
}

void UMainMenu::HostServer()
{
	if (NetworkingInterface != nullptr)
	{
		NetworkingInterface->Host();
	}
}
