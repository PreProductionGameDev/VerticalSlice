// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/TestGameplayAbility.h"

#include "BaseWeapon.generated.h"

class AFP_Character;

UCLASS()
class VERTICALSLICE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UTestGameplayAbility>> GameplayAbilities;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnEquip();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
