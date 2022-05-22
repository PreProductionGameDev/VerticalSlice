// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapons/BaseWeapon.h"

#include "AbilitySystemComponent.h"
#include "FP_Character.h"
#include "Core/TestAbilitySystemComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::OnEquip()
{
	/*AFP_Character* Owner = Cast<AFP_Character>(this->GetOwner());
	
	UTestAbilitySystemComponent* ASC = Owner->GetAbilitySystemComponent();

	//Grant abilities, but only on the server
	for(TSubclassOf<UTestGameplayAbility>& newAbility : GameplayAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(
			newAbility, 1, static_cast<int32>(newAbility.GetDefaultObject()->AbilityInputID),
			this));
	}
*/
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

