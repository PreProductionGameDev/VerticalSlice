// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/AttributeSets/AmmoAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAmmoAttributeSet::UAmmoAttributeSet()
{
	SMGAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG"));
	ShotgunAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Shotgun"));
	SniperAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Sniper"));
	RocketAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Rocket"));
	BurstRifleAmmoTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.BurstRifle"));
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
		const float Ammo = GetSMGReserveAmmo();	
		SetSMGReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxSMGReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetShotgunReserveAmmoAttribute())
	{
		const float Ammo = GetShotgunReserveAmmo();
		SetShotgunReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxSMGReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetSniperReserveAmmoAttribute())
	{
		const float Ammo = GetSniperReserveAmmo();
		SetShotgunReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxSniperReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetBurstRifleReserveAmmoAttribute())
	{
		const float Ammo = GetBurstRifleReserveAmmo();
		SetBurstRifleReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxBurstRifleReserveAmmo()));
	}
	else if (Data.EvaluatedData.Attribute == GetRocketReserveAmmoAttribute())
	{
		const float Ammo = GetRocketReserveAmmo();
		SetRocketReserveAmmo(FMath::Clamp<float>(Ammo, 0, GetMaxRocketReserveAmmo()));
	}
}

void UAmmoAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Ensure that each new AttributeData is RepNotified
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, SMGReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxSMGReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, ShotgunReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxShotgunReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, SniperReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxSniperReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, BurstRifleReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxBurstRifleReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, RocketReserveAmmo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAmmoAttributeSet, MaxRocketReserveAmmo, COND_None, REPNOTIFY_Always);
}

FGameplayAttribute UAmmoAttributeSet::GetReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag)
{
	// Add additional Else If for each new ammo type here
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG")))
	{
		return GetSMGReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Shotgun")))
	{
		return GetShotgunReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Sniper")))
	{
		return GetSniperReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.BurstRifle")))
	{
		return GetBurstRifleReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Rocket")))
	{
		return  GetRocketReserveAmmoAttribute();
	}
	
	return FGameplayAttribute();
}

FGameplayAttribute UAmmoAttributeSet::GetMaxReserveAmmoAttributeFromTag(const FGameplayTag& PrimaryAmmoTag)
{
	// Add additional Else If for each new ammo type here
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG")))
	{
		return GetMaxSMGReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Shotgun")))
	{
		return GetMaxShotgunReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Sniper")))
	{
		return GetMaxSniperReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.BurstRifle")))
	{
		return GetMaxBurstRifleReserveAmmoAttribute();
	}
	if (PrimaryAmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.Rocket")))
	{
		return GetMaxRocketReserveAmmoAttribute();
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

void UAmmoAttributeSet::OnRep_ShotgunReserveAmmo(const FGameplayAttributeData& OldShotgunReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, ShotgunReserveAmmo, OldShotgunReserveAmmo);
}

void UAmmoAttributeSet::OnRep_MaxShotgunReserveAmmo(const FGameplayAttributeData& OldMaxShotgunReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, MaxShotgunReserveAmmo, OldMaxShotgunReserveAmmo);
}

void UAmmoAttributeSet::OnRep_SniperReserveAmmo(const FGameplayAttributeData& OldSniperReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, SniperReserveAmmo, OldSniperReserveAmmo);
}

void UAmmoAttributeSet::OnRep_MaxSniperReserveAmmo(const FGameplayAttributeData& OldMaxSniperReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, MaxSniperReserveAmmo, OldMaxSniperReserveAmmo);
}

void UAmmoAttributeSet::OnRep_BurstRifleReserveAmmo(const FGameplayAttributeData& OldBurstRifleReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, BurstRifleReserveAmmo, OldBurstRifleReserveAmmo);
}

void UAmmoAttributeSet::OnRep_MaxBurstRifleReserveAmmo(const FGameplayAttributeData& OldMaxBurstRifleReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, MaxBurstRifleReserveAmmo, OldMaxBurstRifleReserveAmmo);
}

void UAmmoAttributeSet::OnRep_RocketReserveAmmo(const FGameplayAttributeData& OldRocketReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, RocketReserveAmmo, OldRocketReserveAmmo);
}

void UAmmoAttributeSet::OnRep_MaxRocketReserveAmmo(const FGameplayAttributeData& OldMaxRocketReserveAmmo)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAmmoAttributeSet, MaxRocketReserveAmmo, OldMaxRocketReserveAmmo);
}
