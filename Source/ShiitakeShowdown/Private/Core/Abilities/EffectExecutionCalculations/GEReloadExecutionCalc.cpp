// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/EffectExecutionCalculations/GEReloadExecutionCalc.h"

#include "FP_Character.h"
#include "Core/Abilities/AttributeSets/AmmoAttributeSet.h"
#include "Core/PlayerAbilitySystemComponent.h"

UGEReloadExecutionCalc::UGEReloadExecutionCalc()
{
}

void UGEReloadExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);
	
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.TargetTags = TargetTags;
	EvaluateParameters.SourceTags = SourceTags;

	AFP_Character* OwningPlayer = Cast<AFP_Character>(SourceActor);

	FGameplayTag& AmmoTag = OwningPlayer->GetCurrentWeapon()->PrimaryAmmoType;

	if (AmmoTag == FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.SMG")))
	{
		Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.ReloadAmount.Reserve")), false, -1.0f);
	}
}
