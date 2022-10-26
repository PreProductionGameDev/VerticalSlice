// 2022 ChronoOwl Studios


#include "Core/Abilities/EffectExecutionCalculations/GEDamageFalloffExecutionCalc.h"
#include "Core/PlayerAttributeSet.h"

#include "Core/Abilities/Utility/GEUIData_KillFeed.h"

struct GSDamageStaticsFalloff
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	GSDamageStaticsFalloff()
	{
		// Snapshot happens at the time of GESpec creation
		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.
		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerAttributeSet, Damage, Source, true);
	}
};

static const GSDamageStaticsFalloff& DamageStatics()
{
	static GSDamageStaticsFalloff DStatics;
	return DStatics;
}

UGEDamageFalloffExecutionCalc::UGEDamageFalloffExecutionCalc()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UGEDamageFalloffExecutionCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Cache the AbilitySystem component
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
    // Get the owning Actor from the Ability System Component
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;

	
	
	UE_LOG(LogTemp, Warning, TEXT("SourceActor Name: %s, TargetActor Name: %s"), *SourceAbilitySystemComponent->GetName(), *TargetAbilitySystemComponent->GetName())
	
	// Obtain the Gameplay Spec and Get all Asset Tags
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// Obtain the Tags from the Source and Target
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

}
