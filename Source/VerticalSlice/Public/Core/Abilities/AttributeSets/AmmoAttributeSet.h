// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AmmoAttributeSet.generated.h"

// Uses Macros from Attributes.h
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
// Ammo Property Values
// SMG Ammo
#pragma region SMG_Values
	// SMG Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_SMGReserveAmmo)
	FGameplayAttributeData SMGReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, SMGReserveAmmo)
	// SMG Max Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxSMGReserveAmmo)
	FGameplayAttributeData MaxSMGReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, MaxSMGReserveAmmo)
#pragma endregion SMG_Values
// Shotgun Ammo
#pragma region Shotgun_Values
	// Shotgun Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_ShotgunReserveAmmo)
	FGameplayAttributeData ShotgunReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, ShotgunReserveAmmo)
	// Shotgun Max Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxShotgunReserveAmmo)
	FGameplayAttributeData MaxShotgunReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, MaxShotgunReserveAmmo)
#pragma endregion Shotgun_Values
// Sniper Ammo
#pragma region Sniper_Values
	// Sniper Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_SniperReserveAmmo)
	FGameplayAttributeData SniperReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, SniperReserveAmmo)
	// Sniper Max Reserve Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Ammo", ReplicatedUsing = OnRep_MaxSniperReserveAmmo)
	FGameplayAttributeData MaxSniperReserveAmmo;
	ATTRIBUTER_ACCESSORS(UAmmoAttributeSet, MaxSniperReserveAmmo)
#pragma endregion Sniper_Values

	// Occurs when an attribute is about to be changed. Just calls Super
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// Post Gameplay Effect. Used for adding ammo
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// Variable Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Returns the Reserve Ammo Attribute from a specified Tag 
	static FGameplayAttribute GetReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag);
	// Returns the Max Reserve Ammo Attribute from a specified Tag
	static FGameplayAttribute GetMaxReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag);
protected:
	// Cached Gameplay Tags
	FGameplayTag SMGAmmoTag;
	FGameplayTag ShotgunAmmoTag;
	FGameplayTag SniperAmmoTag;

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
	// Sniper OnRep()
	UFUNCTION()
	virtual void OnRep_SniperReserveAmmo(const FGameplayAttributeData& OldSniperReserveAmmo);
	UFUNCTION()
	virtual void OnRep_MaxSniperReserveAmmo(const FGameplayAttributeData& OldMaxSniperReserveAmmo);
};
