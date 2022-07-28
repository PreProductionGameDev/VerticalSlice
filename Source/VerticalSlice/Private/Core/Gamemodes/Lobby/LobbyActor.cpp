// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Gamemodes/Lobby/LobbyActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALobbyActor::ALobbyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	USpringArmComponent* ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,50.0f), FRotator(-30.0f,-90.0f,0.0f));
	ArmComponent->TargetArmLength = 400.0f;

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(ArmComponent, USpringArmComponent::SocketName);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

}

void ALobbyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue1, COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue2 ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue3 ,COND_None );

}

// Called when the game starts or when spawned
void ALobbyActor::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this,0);

	if(IsOwnedBy(Controller))
	{
		Controller->bShowMouseCursor=true;
		ResetCamera();
	}
}

void ALobbyActor::ResetCamera()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	Controller->SetViewTarget(this);
}


// Called every frame
void ALobbyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

