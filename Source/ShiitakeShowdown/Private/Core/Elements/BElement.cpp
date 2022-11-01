// 2022 ChronoOwl Studios
// Jacob

#include "Core/Elements/BElement.h"
#include "FP_Character.h"

/**
 * @name Jacob
 * @brief Turns of tick and enable replication
 */
ABElement::ABElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

/**
 * @name Jacob
 * @brief Replicates the owning player pointer
 * @param OutLifetimeProps 
 */
void ABElement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//ensures the element is being replicated from the server to the player
	DOREPLIFETIME_CONDITION(ABElement, OwningPlayer, COND_OwnerOnly);
}

/**
 * @name Jacob
 * @brief Sets the owning player and adds the abilities
 */
void ABElement::BeginPlay()
{
	Super::BeginPlay();
	//sets the owning player and gives them access to the abilities
	OwningPlayer = Cast<AFP_Character>(GetOwner());
	AddAbilities();
}

/**
 * @name Jacob
 * @brief Adds the abilities to the players ability system component 
 */
void ABElement::AddAbilities()
{
	//check to ensure has been spawned correctly
	if (!IsValid(OwningPlayer) || !OwningPlayer->GetPlayerAbilitySystemComponent())
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningPlayer->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		return;
	}

	// Grant Abilities but only on the server
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	//adds the abilities and stores the handles
	UtilityAbilityHandle =  ASC->GiveAbility(FGameplayAbilitySpec(UtilityAbility, 1,static_cast<int32>(UtilityAbility.GetDefaultObject()->AbilityInputID), this));
	MovementAbilityHandle =  ASC->GiveAbility(FGameplayAbilitySpec(MovementAbility, 1,static_cast<int32>(MovementAbility.GetDefaultObject()->AbilityInputID), this));
}

/**
 * @name Jacob
 * @brief Removes all of the abilities that I had given the player
 */
void ABElement::RemoveAbilities()
{
	//check to ensure has been spawned correctly
	if (!IsValid(OwningPlayer) || !OwningPlayer->GetPlayerAbilitySystemComponent())
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningPlayer->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		return;
	}

	// Grant Abilities but only on the server
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	//removes the abilities using the stored handles
	ASC->ClearAbility(UtilityAbilityHandle);
	ASC->ClearAbility(MovementAbilityHandle);
}


