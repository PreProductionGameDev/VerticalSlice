// 2022 ChronoOwl Studios


#include "Core/Elements/BElement.h"
#include "FP_Character.h"

// Sets default values
ABElement::ABElement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

}

void ABElement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ABElement, OwningPlayer, COND_OwnerOnly);
}

// Called when the game starts or when spawned
void ABElement::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayer = Cast<AFP_Character>(GetOwner());
	AddAbilities();
}

void ABElement::AddAbilities()
{
	if (!IsValid(OwningPlayer) || !OwningPlayer->GetPlayerAbilitySystemComponent())
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningPlayer->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s Role: %s ASC is null"), *FString(__FUNCTION__), *GetName(), *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(OwningPlayer->GetLocalRole())));
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

void ABElement::RemoveAbilities()
{
	if (!IsValid(OwningPlayer) || !OwningPlayer->GetPlayerAbilitySystemComponent())
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningPlayer->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s Role: %s ASC is null"), *FString(__FUNCTION__), *GetName(), *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(OwningPlayer->GetLocalRole())));
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


