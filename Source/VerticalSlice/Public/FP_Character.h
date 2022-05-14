// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"


#include "FP_Character.generated.h"

UCLASS()
class VERTICALSLICE_API AFP_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true", DisplayName = "FPCamera"));
	UCameraComponent* FirstPersonCameraComponent;
public:
	// Sets default values for this character's properties
	AFP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	// Movement Values
	void MoveForward(float value);
	void MoveRight(float value);

	void OnPrimaryActionPressed();
	void OnPrimaryActionReleased();

	void OnSecondaryActionPressed();
	void OnSecondaryActionReleased();
};
