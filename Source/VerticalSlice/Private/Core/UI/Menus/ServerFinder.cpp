// 2022 ChronoOwl Studios


#include "Core/UI/Menus/ServerFinder.h"

#include "OnlineSessionSettings.h"
#include "Components/ScrollBox.h"
#include "Core/UI/Menus/ServerResult.h"

UServerFinder::UServerFinder(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerResultBPClass(TEXT("/Game/ShiitakeSorcerers/UI/Networking/WB_ServerResult"));
	if (!ensure(ServerResultBPClass.Class != nullptr))
	{
		return;
	}

	ServerRowClass = ServerResultBPClass.Class;
}

void UServerFinder::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (FServerData& ServerData : ServerNames)
	{
		// Create the Server Row Widget
		UServerResult* Row = CreateWidget<UServerResult>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->PopulateServerData(ServerData);
		
		ServerList->AddChild(Row);
	}
}
