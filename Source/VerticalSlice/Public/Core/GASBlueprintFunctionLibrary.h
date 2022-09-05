// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/PlayerGameplayAbility.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerAttributeSet.h"
#include "Core/Abilities/GameplayEffects/GameplayEffectContext.h"
#include "GameplayTagContainer.h"
#include "GASBlueprintFunctionLibrary.generated.h"

/**
 *	Useful Functions used for GAS classes
 */
UCLASS()
class VERTICALSLICE_API UGASBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Locate Primary AbilityInstances
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UPlayerGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UPlayerGameplayAbility* GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass);
		
	// Identifies if SpecHandle is valid
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);
	
	// Gameplay Effect Context
	// Returns TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "GetTargetData"))
	static FGameplayAbilityTargetDataHandle EffectContextGetTargetData(FGameplayEffectContextHandle EffectContextHandle);

	// Adds TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "AddTargetData"))
	static void EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData);
	
	UFUNCTION(BlueprintCallable, Category = "Ability|TargetData")
	static void ClearTargetData(UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tags")
	static void CancelAbilitiesWithTags(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags, UGameplayAbility* Ignore);
	
};
