// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gamemodes/Lobby/LobbyActor.h"

// Sets default values
ALobbyActor::ALobbyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALobbyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

