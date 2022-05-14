// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Jump Events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Fire Events
	// Primary Fire
	InputComponent->BindAction("Primary Action", IE_Pressed, this, &AFP_Character::OnPrimaryActionPressed);
	InputComponent->BindAction("Primary Action", IE_Released, this, &AFP_Character::OnPrimaryActionReleased);
	// Secondary Fire
	InputComponent->BindAction("Secondary Action", IE_Pressed, this, &AFP_Character::OnSecondaryActionPressed);
	InputComponent->BindAction("Secondary Action", IE_Released, this, &AFP_Character::OnSecondaryActionReleased);

	// Bind Movement Events
	InputComponent->BindAxis("Move Forward / Backward", this, &AFP_Character::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &AFP_Character::MoveRight);

	// Bind Mouse Input
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void AFP_Character::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFP_Character::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFP_Character::OnPrimaryActionPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Shoot"));
}

void AFP_Character::OnPrimaryActionReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("End Shoot"));
}

void AFP_Character::OnSecondaryActionPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Scope"));
}

void AFP_Character::OnSecondaryActionReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("End Scope"));
}

