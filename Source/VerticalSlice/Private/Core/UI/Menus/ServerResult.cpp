// 2022 ChronoOwl Studios


#include "Core/UI/Menus/ServerResult.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Core/UI/Menus/ServerFinder.h"

void UServerResult::Setup(UServerFinder* InParent, uint32 InIndex)
{
	RowButton->OnClicked.AddDynamic(this, &UServerResult::OnClicked);

	Parent = InParent;
	Index = InIndex;
	
}

void UServerResult::PopulateServerData(FServerData& ServerData)
{
	this->ServerName->SetText(FText::FromString(ServerData.HostUsername));

	const FString PlayerCountText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
	this->PlayerCount->SetText(FText::FromString(PlayerCountText));
}

void UServerResult::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("YES %d"), Index);
	Parent->SelectIndex(Index);
}
