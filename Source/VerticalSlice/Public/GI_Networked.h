// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Networked.generated.h"

/**
 * 
 */
UCLASS()
class UGI_Networked : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UGI_Networked();


    UFUNCTION(BlueprintCallable)
        void Host(const FString& Location);

    UFUNCTION(BlueprintCallable)
        void Join(const FString& Address);

    UFUNCTION(BlueprintCallable)
    virtual void LoadMainMenu();
};
