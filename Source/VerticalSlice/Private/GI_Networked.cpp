// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Networked.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UGI_Networked::UGI_Networked(const FObjectInitializer& ObjectInitializer)
{
    /*ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MenuBPClass.Class != nullptr)) return;

    MenuClass = MenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

    InGameMenuClass = InGameMenuBPClass.Class;*/
}

void UGI_Networked::Init()
{
  /*  UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());*/
}

void UGI_Networked::LoadMenu()
{
    /*if (!ensure(MenuClass != nullptr)) return;

    Menu = CreateWidget<UMainMenu>(this, MenuClass);
    if (!ensure(Menu != nullptr)) return;

    Menu->Setup();

    Menu->SetMenuInterface(this);*/
}

void UGI_Networked::InGameLoadMenu()
{
   /* if (!ensure(InGameMenuClass != nullptr)) return;

    UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if (!ensure(Menu != nullptr)) return;

    Menu->Setup();

    Menu->SetMenuInterface(this);*/
}

void UGI_Networked::Host()
{
   /* if (Menu != nullptr)
    {
        Menu->Teardown();
    }*/

    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UGI_Networked::Join(const FString& Address)
{
    /*if (Menu != nullptr)
    {
        Menu->Teardown();
    }*/

    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UGI_Networked::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}