// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Networked.h"



UGI_Networked::UGI_Networked()
{
    
}

void UGI_Networked::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        UEngine* Engine = GetEngine();
        Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString()));
    }
}

void UGI_Networked::Host(const FString& Location)
{
    UE_LOG(LogTemp, Warning, TEXT("Started Host"));
    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    World->ServerTravel(Location);
}

void UGI_Networked::Search()
{
  
}

void UGI_Networked::Join(const FString& Address)
{
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