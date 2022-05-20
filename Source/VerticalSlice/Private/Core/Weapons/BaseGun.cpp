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

	// Validation for Data. Deletes Weapon
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
	// If the player can currently shoot, Then shoot
	// Otherwise Calculate time till they can next shoot and shoot then	
	if (bCanShoot)
	{
		FirstShot();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_DelayedShot);
		GetWorldTimerManager().SetTimer(TimerHandle_DelayedShot, this, &ABaseGun::FirstShot, GetWorldTimerManager().GetTimerRemaining(TimerHandle_CanShoot), false);
	}
}

void ABaseGun::Primary()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Shot"));
}

void ABaseGun::EndPrimary()
{
	// This stops repeated clicking of left mouse causing issues
	if (!GetWorldTimerManager().TimerExists(TimerHandle_CanShoot))
	{
		SetCanShoot(false);
		GetWorldTimerManager().SetTimer(TimerHandle_CanShoot, this, &ABaseGun::ResetShot, GetWorldTimerManager().GetTimerRemaining(TimerHandle_HandleRefire), false);
	}
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
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

void ABaseGun::SetCanShoot(bool bShoot)
{
	bCanShoot = bShoot;
}

void ABaseGun::FirstShot()
{
	// Do Primary action then setup timers.
	Primary();
	GetWorldTimerManager().SetTimer(TimerHandle_HandleRefire, this, &ABaseGun::Primary, 60 / Data->FireRate, true);
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedShot);
}

void ABaseGun::ResetShot()
{
	SetCanShoot(true);
	GetWorldTimerManager().ClearTimer(TimerHandle_CanShoot);
}
