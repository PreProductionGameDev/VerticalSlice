// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/GameplayEffects/GEDamageExecutionCalc.h"
#include "Core/PlayerAttributeSet.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/Abilities/Utility/GEUIData_KillFeed.h"

struct GSDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	GSDamageStatics()
	{
		// Snapshot happens at the time of GESpec creation
		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.
		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Damage, Source, true);
	}
};

static const GSDamageStatics& DamageStatics()
{
	static GSDamageStatics DStatics;
	return DStatics;
}

UGEDamageExecutionCalc::UGEDamageExecutionCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UGEDamageExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	// Ignore Team Damage
	if (TargetAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))) && SourceAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Blue"))))
	{
		return;
	}
	if (TargetAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))) && SourceAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Team.Red"))))
	{
		return;
	}

	// Set the KillFeed Icon if Valid
	if (const UGEUIData_KillFeed* KillFeedData = Cast<UGEUIData_KillFeed>(Spec.Def->UIData))
	{
		Cast<UPlayerAbilitySystemComponent>(TargetAbilitySystemComponent)->LastDamagedBy = KillFeedData->KillFeedIcon;
	}
	
	float Damage = 0.0f;
	// Capture Optional damage value set on the Damage GE as a Calculation modifier under the Execution Calculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, Damage);
	// Add SetByCallerDamage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	if (Damage > 0.0f)
	{
		// Set the Target's Damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, Damage));
	}
}
