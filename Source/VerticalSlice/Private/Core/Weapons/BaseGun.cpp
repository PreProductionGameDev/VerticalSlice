// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapons/BaseGun.h"

// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();
	
	if (Data == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GUN HAS NO DATA SELECTED"));
		Destroy();
	}
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGun::BeginPrimary()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Shoot"));
}

void ABaseGun::Primary()
{
	
}

void ABaseGun::EndPrimary()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("STOP Shoot"));
}

void ABaseGun::BeginSecondary()
{
	
}

void ABaseGun::Secondary()
{
	
}

void ABaseGun::EndSecondary()
{
	
}

