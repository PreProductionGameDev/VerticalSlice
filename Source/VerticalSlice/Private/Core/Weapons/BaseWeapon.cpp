// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapons/BaseWeapon.h"

#include "AbilitySystemComponent.h"
#include "FP_Character.h"
#include "Core/PlayerAbilitySystemComponent.h"

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
	if (OwningPlayer)
	{
		UPlayerAbilitySystemComponent* ASC = OwningPlayer->GetAbilitySystemComponent();	
		if (OwningPlayer->GetLocalRole() == ROLE_Authority)
		{
			//Grant abilities, but only on the server
			for(TSubclassOf<UPlayerGameplayAbility>& StartupAbility : GameplayAbilities)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ability Added"));
				ASC->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Gun has no Owning Player"));
	}
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

