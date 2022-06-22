// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapons/BaseWeapon.h"

#include "AbilitySystemComponent.h"
#include "FP_Character.h"
#include "Components/BillboardComponent.h"
#include "Core/PlayerAbilitySystemComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FP_Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FP_Mesh->bOnlyOwnerSee = true;
	FP_Mesh->SetCastShadow(false);

	TP_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	TP_Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TP_Mesh->bOwnerNoSee = true;

	TPHelper = CreateDefaultSubobject<UBillboardComponent>(TEXT("ThirdPersonPositioning"));
	TPHelper->AttachToComponent(TP_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (Data == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WEAPON HAS NO DATA SELECTED"));
		Destroy();
	}
	else
	{
		CurrentAmmoPool = Data->StartingAmmo;
		FP_Mesh->GetAnimInstance()->Montage_Play(Data->WeaponAnim, 1);
		FP_Mesh->GetAnimInstance()->Montage_JumpToSection(FName("Idle"));
	}
}

void ABaseWeapon::OnEquip()
{
	if (Owner)
	{
		AFP_Character* OwningPlayer = Cast<AFP_Character>(Owner);
		UPlayerAbilitySystemComponent* ASC = OwningPlayer->GetAbilitySystemComponent();	
		if (Owner->GetLocalRole() == ROLE_Authority)
		{
			//Grant abilities, but only on the server
			for(TSubclassOf<UPlayerGameplayAbility>& StartupAbility : GameplayAbilities)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Ability Added"));
				GameplayAbilityHandles.Add(ASC->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this)));
			} 
		}
	}
}

void ABaseWeapon::OnUnequip_Implementation()
{
	if (Owner)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("TRY TO DELETE"));
		AFP_Character* OwningPlayer = Cast<AFP_Character>(Owner);
		UPlayerAbilitySystemComponent* ASC = OwningPlayer->GetAbilitySystemComponent();	
		if (Owner->GetLocalRole() == ROLE_Authority)
		{
			//Grant abilities, but only on the server
			for(FGameplayAbilitySpecHandle& AbilityToRemove : GameplayAbilityHandles)
			{
				ASC->ClearAbility(AbilityToRemove);
			}
			GameplayAbilityHandles.Empty();
		}
		// DIRTY CODE PLEASE REMOVE STEF
		Destroy();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOT OWNER"));
	}
}

void ABaseWeapon::Server_SpawnVFX_Implementation()
{
	Multi_SpawnVFX();
}

void ABaseWeapon::Multi_SpawnVFX_Implementation()
{
	if (!Owner)
	{
		return;
	}
	if (Cast<AFP_Character>(Owner)->IsLocallyControlled())
	{
		return;
	}
	UNiagaraFunctionLibrary::SpawnSystemAttached(VFXSystem, TP_Mesh, FName("MuzzlePos"), FVector(0.0f,0.0f,0.0f), TP_Mesh->GetSocketRotation(FName("MuzzlePos")), EAttachLocation::Type::SnapToTarget, true, true);
}

bool ABaseWeapon::CanPickupAmmo()
{
	return !(CurrentAmmoPool == Data->MaxAmmo);
}

void ABaseWeapon::AddAmmo(int ammoPickup)
{
	CurrentAmmoPool = FMath::Clamp(CurrentAmmoPool+= ammoPickup, 0, Data->MaxAmmo);
}

void ABaseWeapon::RemoveAmmo(int AmmoToRemove)
{
	CurrentAmmoPool = FMath::Clamp(CurrentAmmoPool -= AmmoToRemove, 0, Data->MaxAmmo);
}

int ABaseWeapon::GetAmmo()
{
	return CurrentAmmoPool;
}

bool ABaseWeapon::CanShoot()
{
	return CurrentAmmoPool > 0;
}

