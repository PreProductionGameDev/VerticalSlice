// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Data.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

/**
 *	The Player Ability Class
 *	Extends the GAS Plugin Gameplay Ability
 *	Additional Functionality is Added here for our custom use
 */
UCLASS()
class SHIITAKESHOWDOWN_API UPlayerGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerGameplayAbility();

	//Abilities will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EPlayerAbilityInputID AbilityInputID = EPlayerAbilityInputID::None;
	// Value to associate an Ability with a slot without tying it to an automatically activated input.
	// EG Passive Abilities as they are not tied to an input.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EPlayerAbilityInputID AbilityID = EPlayerAbilityInputID::None;
	// Only Activate this Ability if is the current Weapon Equipped
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	bool bSourceObjectMustBeCurrentWeaponToActivate;
	// Only Activate this Ability if is the current element Equipped
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Abilities")
	bool bSourceObjectMustBeCurrentElementToActivate;
		
	// Ability Utility Functions
	// Expose GetSourceObject to Blueprint. Retrieves the SourceObject associated with this ability. Callable on non instanced abilities.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability", meta = (DisplayName = "Get Source Object"))
	UObject* K2_GetSourceObject(FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;
	// Attempts to activate the given ability handle and batch all RPCs into one. This will only batch all RPCs that happen
	// in one frame. Best case scenario we batch ActivateAbility, SendTargetData, and EndAbility into one RPC instead of three.
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);
	// Overrides the CanActivateAbility for custom functionality. Used for weapon system
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	// Same as calling K2_EndAbility. Meant for use with batching system to end the ability externally.
	virtual void ExternalEndAbility();
	
	// Check the Cost to use an ability
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	// Allows C++ and Blueprint abilities to override how cost is checked in case they don't use a GE like weapon ammo
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	bool GSCheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;
	virtual bool GSCheckCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo) const;
	// Apply the cost of the ability
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	// Allows C++ and Blueprint abilities to override how cost is applied in case they don't use a GE like weapon ammo
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Ability")
	void GSApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;
	virtual void GSApplyCost_Implementation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo& ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const {};

};
