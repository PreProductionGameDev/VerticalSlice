// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Networked.generated.h"

/**
 * 
 */
UCLASS()
class VERTICALSLICE_API UGI_Networked : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UGI_Network(const FObjectInitializer& ObjectInitializer);

    virtual void Init();

    UFUNCTION(BlueprintCallable)
        void LoadMenu();

    UFUNCTION(BlueprintCallable)
        void InGameLoadMenu();

    UFUNCTION(Exec)
        void Host() override;

    UFUNCTION(Exec)
        void Join(const FString& Address) override;

    virtual void LoadMainMenu() override;

private:
    TSubclassOf<class UUserWidget> MenuClass;
    TSubclassOf<class UUserWidget> InGameMenuClass;

    class UMainMenu* Menu;
};
