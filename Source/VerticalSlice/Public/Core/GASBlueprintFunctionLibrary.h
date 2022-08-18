// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/PlayerGameplayAbility.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerAttributeSet.h"
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
};
