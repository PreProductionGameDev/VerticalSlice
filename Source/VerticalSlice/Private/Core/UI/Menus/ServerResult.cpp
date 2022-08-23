// 2022 ChronoOwl Studios


#include "Core/UI/Menus/ServerResult.h"

#include "Components/TextBlock.h"

void UServerResult::PopulateServerData(FServerData& ServerData)
{
	this->ServerName->SetText(FText::FromString(ServerData.HostUsername));
}
