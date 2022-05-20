// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class VERTICALSLICE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Primary Actions for firing
	virtual void BeginPrimary() {};
	virtual void Primary() {};
	virtual void EndPrimary() {};

	// Secondary Actions
	virtual void BeginSecondary() {};
	virtual void Secondary() {};
	virtual void EndSecondary() {};
};
