// Fill out your copyright notice in the Description page of Project Settings.
// Jacob


#include "Core/Gamemodes/Lobby/LobbyActor.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

/**
 * @name Jacob
 * @brief creates the camera and skeletal mesh 
 */
ALobbyActor::ALobbyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	USpringArmComponent* ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	ArmComponent->SetupAttachment(RootComponent);
	ArmComponent->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,140.0f), FRotator(-20.0f,-90.0f,0.0f));
	ArmComponent->TargetArmLength = 400.0f;

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(ArmComponent, USpringArmComponent::SocketName);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

}

/**
 * @name Jacob
 * @brief Replicates all of the needed parts with notifies
 * @param OutLifetimeProps 
 */
void ALobbyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue1, COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue2 ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_Hue3 ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_CurrentMesh ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_HeadMesh ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_BodyMesh ,COND_None );
	DOREPLIFETIME_CONDITION(ALobbyActor,DoRep_OtherMesh ,COND_None );

}

/**
 * @name Jacob
 * @brief Shows the mouse cursor on the owning player and resets the camera
 */
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

/**
 * @name Jacob
 * @brief Sets the owning players camera to the camera
 */
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

