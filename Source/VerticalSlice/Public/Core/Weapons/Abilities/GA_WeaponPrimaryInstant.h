// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/PlayerGameplayAbility.h"
#include "GA_WeaponPrimaryInstant.generated.h"

/**
 *  The Base Primary Instant Ability
 *  Overriden in Children Abilities to handle actual functionality
 */
UCLASS()
class VERTICALSLICE_API UGA_WeaponPrimaryInstant : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_WeaponPrimaryInstant();


	// Checks if Ability Can Activate
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	// Creates and fires a bullet
	UFUNCTION()
	virtual void FireBullet();
	// Handle the Data for the Server
	UFUNCTION()
	virtual void HandleTargetData(const FGameplayAbilityTargetDataHandle& TargetData);

	// The Time of the Last shot
	float TimeOfLastShot = -99999.9f;
	
protected:
	// Weapon Recoil
	virtual void HandleRecoil();
	// Store all Cacheable Variables
	virtual void SetupCacheables();
	// Play the Animations
	virtual void PlayAnimations();
};
