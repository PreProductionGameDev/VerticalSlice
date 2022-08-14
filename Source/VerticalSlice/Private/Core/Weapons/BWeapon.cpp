// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Weapons/BWeapon.h"

#include "FP_Character.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/Abilities/AbilityTargetActor/ATA_LineTrace.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABWeapon::ABWeapon()
{
 	// Disable Actor Tick
	PrimaryActorTick.bCanEverTick = false;
	// Network Setup
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
	NetUpdateFrequency = 100.0f;
	// Default Values
	bSpawnWithCollision = true;
	PrimaryClipAmmo = 0;
	MaxPrimaryClipAmmo = 0;
	bInfiniteAmmo = false;
	PrimaryAmmoType = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.None"));

	// Pickup Collision Component
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CollisionComponent"));
	CollisionComp->InitCapsuleSize(40.0f, 50.0f);
	CollisionComp->SetCollisionObjectType(COLLISION_PICKUP);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Enable Collision when in Pickup Mode
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComp;

	// First Person Mesh Setup
	WeaponMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh1P"));
	WeaponMesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh1P->bOnlyOwnerSee = true;
	WeaponMesh1P->CastShadow = false;
	WeaponMesh1P->SetVisibility(false, true);
	WeaponMesh1P->SetupAttachment(CollisionComp);
	WeaponMesh1P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

	// Pickup Location
	WeaponMesh3PickupRelativeLocation = FVector(0.0f, -25.0f, 0.0f); // Tweak as see fits

	// Setup Third Person Mesh (For Players and Pickups)
	WeaponMesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh3P"));
	WeaponMesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh3P->SetupAttachment(CollisionComp);
	WeaponMesh3P->SetRelativeLocation(WeaponMesh3PickupRelativeLocation);
	WeaponMesh3P->CastShadow = true;
	WeaponMesh3P->SetVisibility(true, true);
	WeaponMesh3P->bOwnerNoSee = true;
	WeaponMesh3P->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

	// Setup Weapon Gameplay Tags
	WeaponPrimaryInstantAbilityTag = FGameplayTag::RequestGameplayTag("Ability.Weapon.Primary.Instant");
	WeaponSecondaryInstantAbilityTag = FGameplayTag::RequestGameplayTag("Ability.Weapon.Secondary.Instant");
	WeaponIsFiringTag = FGameplayTag::RequestGameplayTag("Weapon.IsFiring");
	FireMode = FGameplayTag::RequestGameplayTag("Weapon.FireMode.None");
	RestrictedPickupTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));
}

UAbilitySystemComponent* ABWeapon::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

USkeletalMeshComponent* ABWeapon::GetWeaponMesh1P() const
{
	return WeaponMesh1P;
}

USkeletalMeshComponent* ABWeapon::GetWeaponMesh3P() const
{
	return WeaponMesh3P;
}

void ABWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add the Replication for the required Objects
	DOREPLIFETIME_CONDITION(ABWeapon, OwningCharacter, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABWeapon, PrimaryClipAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ABWeapon, MaxPrimaryClipAmmo, COND_OwnerOnly);
}

void ABWeapon::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	DOREPLIFETIME_ACTIVE_OVERRIDE(ABWeapon, PrimaryClipAmmo, (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponIsFiringTag)));
}

void ABWeapon::SetOwningCharacter(AFP_Character* InOwningCharacter)
{
	OwningCharacter = InOwningCharacter;
	// If the Owning Character is a valid character, handle ownership
	// Otherwise disconnect from owner
	if (OwningCharacter)
	{
		// Called when Added to Inventory
		// Sets the owner and attaches to owner
		AbilitySystemComponent = Cast<UPlayerAbilitySystemComponent>(OwningCharacter->GetPlayerAbilitySystemComponent());
		SetOwner(InOwningCharacter);
		AttachToComponent(OwningCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		// Disable Pickup Collision sphere
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		// Disconnect from all ownership
		AbilitySystemComponent = nullptr;
		SetOwner(nullptr);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void ABWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Ensures pickup of weapon if valid and not currently owned
	if (IsValid(this) && !OwningCharacter)
	{
		PickUpOnTouch(Cast<AFP_Character>(OtherActor));
	}
}

void ABWeapon::Equip()
{
	// Validate the Owning Character
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s OwningCharacter is nullptr"), *FString(__FUNCTION__), *GetName());
		return;
	}

	// When updating Character. This connects to the models to the person correctly.
	FName AttachPoint = OwningCharacter->GetWeaponAttachPoint();

	// Setup FirstPerson Mesh if Valid
	if (WeaponMesh1P)
	{
		WeaponMesh1P->SetVisibility(true, true);
		if (OwningCharacter->GetFirstPersonMesh())
		{
			// TODO: SOLVE WARNINGS THIS THROWS CAUSE NO SKELETAL MESH 
			// Attaches and sets correct display. Might need to tweak upon applying the models
			if (OwningCharacter->GetFirstPersonMesh()->DoesSocketExist(AttachPoint))
			{
				WeaponMesh1P->AttachToComponent(OwningCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
			}
			else
			{
				WeaponMesh1P->AttachToComponent(OwningCharacter->GetFirstPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
			WeaponMesh1P->SetRelativeLocation(WeaponMesh1PEquippedRelativeLocation);
			WeaponMesh1P->SetRelativeRotation(WeaponMesh1PEquippedRelativeRotation);
		}
	}

	// Setup ThirdPerson Mesh if Valid
	if (WeaponMesh3P)
	{
		WeaponMesh3P->SetVisibility(true, true);
		// Attaches and sets correct display. Might need to tweak upon applying the models
		if (OwningCharacter->GetThirdPersonMesh()->DoesSocketExist(AttachPoint))
		{
			WeaponMesh3P->AttachToComponent(OwningCharacter->GetThirdPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachPoint);
		}
		else
		{
			WeaponMesh3P->AttachToComponent(OwningCharacter->GetThirdPersonMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
			
		WeaponMesh3P->SetRelativeLocation(WeaponMesh3PEquippedRelativeLocation);
		WeaponMesh3P->SetRelativeRotation(WeaponMesh3PEquippedRelativeRotation);
	}
}

void ABWeapon::UnEquip()
{
	if (OwningCharacter == nullptr)
	{
		return;
	}

	// Necessary to detach for visibility reasons
	WeaponMesh1P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh1P->SetVisibility(false, true);

	// Detach Third person view model as well
	WeaponMesh3P->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh3P->CastShadow = false;
	WeaponMesh3P->bCastHiddenShadow = false;
	// This has to be done like this otherwise a lingering shadow can be caused
	WeaponMesh3P->SetVisibility(true, true);
	WeaponMesh3P->SetVisibility(false, true);
}

void ABWeapon::AddAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetPlayerAbilitySystemComponent())
	{
		return;
	}
	
	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningCharacter->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s %s Role: %s ASC is null"), *FString(__FUNCTION__), *GetName(), *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(OwningCharacter->GetLocalRole())));
		return;
	}

	// Grant Abilities but only on the server
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Add all the abilities to the player
	for (TSubclassOf<UPlayerGameplayAbility>& Ability : Abilities)
	{
		AbilitySpecHandles.Add(ASC->GiveAbility(FGameplayAbilitySpec(Ability, GetAbilityLevel(Ability.GetDefaultObject()->AbilityInputID), static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this)));
	}
}

void ABWeapon::RemoveAbilities()
{
	if (!IsValid(OwningCharacter) || !OwningCharacter->GetPlayerAbilitySystemComponent())
	{
		return;
	}

	UPlayerAbilitySystemComponent* ASC = Cast<UPlayerAbilitySystemComponent>(OwningCharacter->GetPlayerAbilitySystemComponent());

	// Validity check if AbilitySystemComponent Exists
	if (!ASC)
	{
		return;
	}

	// Remove Abilities but only on server
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Removes all abilities that the weapon owns from the player.
	for (FGameplayAbilitySpecHandle& SpecHandle : AbilitySpecHandles)
	{
		ASC->ClearAbility(SpecHandle);
	}
}

int32 ABWeapon::GetAbilityLevel(EPlayerAbilityInputID AbilityID)
{
	// All Abilities for now are level 1
	// Could Discard possibly
	return 1;
}

void ABWeapon::ResetWeapon()
{
	FireMode = DefaultFireMode;
}

void ABWeapon::OnDropped_Implementation(FVector NewLocation)
{
	// Removes the weapon from the Player and resets weapon
	SetOwningCharacter(nullptr);
	ResetWeapon();

	// Sets the dropped location and enables sphere pickup collision
	SetActorLocation(NewLocation);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Disables First person mesh
	if (WeaponMesh1P)
	{
		WeaponMesh1P->AttachToComponent(CollisionComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	// Enables ThirdPersonMesh for all clients
	if (WeaponMesh3P)
	{
		WeaponMesh3P->AttachToComponent(CollisionComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
		WeaponMesh3P->SetRelativeLocation(WeaponMesh3PickupRelativeLocation);
		WeaponMesh3P->CastShadow = true;
		WeaponMesh3P->SetVisibility(true, true);
	}
}

bool ABWeapon::OnDropped_Validate(FVector NewLocation)
{
	return true;
}

void ABWeapon::DeleteWeapon()
{
	RemoveAbilities();
	SetOwningCharacter(nullptr);
	ResetWeapon();
	Destroy();
}

int32 ABWeapon::GetPrimaryClipAmmo() const
{
	return PrimaryClipAmmo;
}

int32 ABWeapon::GetMaxPrimaryClipAmmo() const
{
	return MaxPrimaryClipAmmo;
}

void ABWeapon::SetPrimaryClipAmmo(int32 NewPrimaryClipAmmo)
{
	int32 const OldPrimaryClipAmmo = PrimaryClipAmmo;
	PrimaryClipAmmo = NewPrimaryClipAmmo;
	OnPrimaryClipAmmoChanged.Broadcast(OldPrimaryClipAmmo, PrimaryClipAmmo);
}

void ABWeapon::SetMaxPrimaryClipAmmo(int32 NewMaxPrimaryClipAmmo)
{
	int32 const OldMaxPrimaryClipAmmo = MaxPrimaryClipAmmo;
	MaxPrimaryClipAmmo = NewMaxPrimaryClipAmmo;
	OnMaxPrimaryClipAmmoChanged.Broadcast(OldMaxPrimaryClipAmmo, MaxPrimaryClipAmmo);
}

bool ABWeapon::HasInfiniteAmmo() const
{
	return bInfiniteAmmo;
}

UAnimMontage* ABWeapon::GetEquip1PMontage() const
{
	return Equip1PMontage;
}

UAnimMontage* ABWeapon::GetEquip3PMontage() const
{
	return Equip3PMontage;
}

USoundCue* ABWeapon::GetPickupSound() const
{
	return PickupSound;
}

AATA_LineTrace* ABWeapon::GetLineTraceTargetActor()
{
	// If the LineTraceTargetActor exists return otherwise create new
	if (LineTraceTargetActor)
	{
		return LineTraceTargetActor;
	}

	// Create New TargetActor to return
	LineTraceTargetActor = GetWorld()->SpawnActor<AATA_LineTrace>();
	LineTraceTargetActor->SetOwner(this);
	return LineTraceTargetActor;
}

UCurveVector* ABWeapon::GetRecoilPattern() const
{
	return RecoilPattern;
}

int32 ABWeapon::GetAmmoCost() const
{
	return AmmoCost;
}

float ABWeapon::GetTimeBetweenShots() const
{
	return TimeBetweenShots;
}

float ABWeapon::GetRecoilCooldown() const
{
	return RecoilCooldownRate;
}

UTexture2D* ABWeapon::GetUITexture() const
{
	return WeaponIcon;
}

void ABWeapon::BeginPlay()
{
	// Reset the weapon values
	ResetWeapon();
	
	if (!OwningCharacter && bSpawnWithCollision)
	{
		// Spawned into the world without an owner, enable collision as we are in pickup mode
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	
	Super::BeginPlay();
}

void ABWeapon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABWeapon::PickUpOnTouch(AFP_Character* InCharacter)
{
	if (!InCharacter || !InCharacter->IsAlive() || !InCharacter->GetPlayerAbilitySystemComponent() || InCharacter->GetPlayerAbilitySystemComponent()->HasAnyMatchingGameplayTags(RestrictedPickupTags))
	{
		return;
	}

	// TODO: Add the weapon to the inventory and disable Meshes
	if (InCharacter->AddWeaponToInventory(this, false))
	{
		
	}
}

void ABWeapon::OnRep_PrimaryClipAmmo(int32 OldPrimaryClipAmmo)
{
	OnPrimaryClipAmmoChanged.Broadcast(OldPrimaryClipAmmo, PrimaryClipAmmo);
}

void ABWeapon::OnRep_MaxPrimaryClipAmmo(int32 OldMaxPrimaryClipAmmo)
{
	OnMaxPrimaryClipAmmoChanged.Broadcast(OldMaxPrimaryClipAmmo, PrimaryClipAmmo);
}

