// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/Weapons/BaseWeapon.h"
#include "Camera/CameraComponent.h"


#include "FP_Character.generated.h"

class UTestGameplayAbility;
class UTestAbilitySystemComponent;
class UTestAttributeSet;

UCLASS()
class VERTICALSLICE_API AFP_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true", DisplayName = "FPCamera"));
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ABaseWeapon> EquipedGun;
	
public:
	// Sets default values for this character's properties
	AFP_Character();

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Abillity System functions
	//Class Overides
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void AddStartupGameplayAbilities();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UTestGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitialized:1;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser );

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags,
		AFP_Character* InstigatorCharacter, AActor* DamageCauser);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	friend UTestAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTestAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UTestAttributeSet> Attributes;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

private:
	// Movement Values
	void MoveForward(float value);
	void MoveRight(float value);

	// Primary Action Functions
	// Used For Shooting
	UFUNCTION(BlueprintCallable)
	void OnPrimaryActionPressed();
	UFUNCTION(BlueprintCallable)
	void OnPrimaryActionReleased();

	// Secondary Action Functions
	// Used for Scoping
	UFUNCTION(BlueprintCallable)
	void OnSecondaryActionPressed();
	UFUNCTION(BlueprintCallable)
	void OnSecondaryActionReleased();

public:
	UFUNCTION(BlueprintCallable)
	void EquipGun(ABaseWeapon* const NewGun);
};
