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
	this->ServerName->SetText(FText::FromString(ServerData.Name));

	const FString PlayerCountText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
	this->PlayerCount->SetText(FText::FromString(PlayerCountText));
}

void UServerResult::ResetText()
{
	this->ServerName->SetColorAndOpacity(FLinearColor(1,1,1,1));
}


void UServerResult::OnClicked()
{
	Parent->ResetSelected();
	Parent->SelectIndex(Index);
	this->ServerName->SetColorAndOpacity(FLinearColor(0,1,0,1));
	Parent->UpdateServerSettings();
}
