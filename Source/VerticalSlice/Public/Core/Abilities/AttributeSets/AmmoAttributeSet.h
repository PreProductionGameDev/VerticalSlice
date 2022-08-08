// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AmmoAttributeSet.generated.h"

//uses macros from Attributes.h
#define ATTRIBUTER_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 *	 An Attribute Set used for storing Ammo of the players
 *	 Each Weapon type has Reserve and Max Reserve Ammo stored in this set.
 *	 To Create a New Ammo Type:
 *	  - Create an FGameplayAttributeData for both Reserve and MaxReserve
 *	  - Create a OnRep function for the Max and Reserve
 *	  - Create a Cached Gameplay Tag for the Ammo Type
 *	  - Set the Cached Gameplay Tag in the Constructor
 *	  - Add functionality for the Ammo Type to the PostGameplayEffectExecute()
 *	  - Update both GetReserveAmmoAttributeFromTag() and GetMaxReserveAmmoAttributeFromTag()
 */
UCLASS()
class VERTICALSLICE_API UAmmoAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAmmoAttributeSet();

	// SMG Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_SMGReserveAmmo)
	FGameplayAttributeData SMGReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, SMGReserveAmmo)

	// SMG Max Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxSMGReserveAmmo)
	FGameplayAttributeData MaxSMGReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, MaxSMGReserveAmmo)

	// Shotgun Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ShotgunReserveAmmo)
	FGameplayAttributeData ShotgunReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, ShotgunReserveAmmo)

	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxShotgunReserveAmmo)
	FGameplayAttributeData MaxShotgunReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, MaxShotgunReserveAmmo)
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	static FGameplayAttribute GetReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag);
	static FGameplayAttribute GetMaxReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag);
protected:
	// Cached Gameplay Tags
	FGameplayTag SMGAmmoTag;
	FGameplayTag ShotgunAmmoTag;

	// OnRep Functions are used to make sure the AbilitySystem Internal Representations are Synchronized properly during Replication
	// SMG OnRep()
	UFUNCTION()
	virtual void OnRep_SMGReserveAmmo(const FGameplayAttributeData& OldSMGReserveAmmo);
	UFUNCTION()
	virtual void OnRep_MaxSMGReserveAmmo(const FGameplayAttributeData& OldMaxSMGReserveAmmo);
	// Shotgun OnRep()
	UFUNCTION()
	virtual void OnRep_ShotgunReserveAmmo(const FGameplayAttributeData& OldShotgunReserveAmmo);
	UFUNCTION()
	virtual void OnRep_MaxShotgunReserveAmmo(const FGameplayAttributeData& OldMaxShotgunReserveAmmo);
};
