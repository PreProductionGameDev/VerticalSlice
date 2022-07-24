// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/AttributeSets/AmmoAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAmmoAttributeSet::UAmmoAttributeSet()
{
	SMGAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG"));
}

void UAmmoAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UAmmoAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Add additional Else If for each new Ammo type here
	if (Data.EvaluatedData.Attribute == GetSMGReserveAmmoAttribute())
	{
		float Ammo = GetSMGReserveAmmo();
		UE_LOG(LogTemp, Error, TEXT("AMMO CHANGED TO %s"), *FString::FromInt(Ammo)); 
		SetSMGReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxSMGReserveAmmo()));
	}	
}

void UAmmoAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Ensure that each new AttributeData is RepNotified
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, SMGReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxSMGReserveAmmo, COND_None, REPNOTIFY_Always);
}

FGameplayAttribute UAmmoAttributeSet::GetReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag)
{
	// Add additional Else If for each new ammo type here
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG")))
	{
		return GetSMGReserveAmmoAttribute();
	}

	return FGameplayAttribute();
}

FGameplayAttribute UAmmoAttributeSet::GetMaxReserveAmmoAttributeFromTag(FGameplayTag& PrimaryAmmoTag)
{
	// Add additional Else If for each new ammo type here
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG")))
	{
		return GetMaxSMGReserveAmmoAttribute();
	}

	return FGameplayAttribute();
}

void UAmmoAttributeSet::OnRep_SMGReserveAmmo(const FGameplayAttributeData& OldSMGReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, SMGReserveAmmo, OldSMGReserveAmmo);
}

void UAmmoAttributeSet::OnRep_MaxSMGReserveAmmo(const FGameplayAttributeData& OldMaxSMGReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, MaxSMGReserveAmmo, OldMaxSMGReserveAmmo);
}
