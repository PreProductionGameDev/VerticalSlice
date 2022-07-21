// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"

#include "Core/Data.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerAttributeSet.h"
#include "Core/PlayerGameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAbilitiesInitialized =false;

	NoWeaponTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.None"));
	WeaponAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Weapon"));
	CurrentWeaponTag = NoWeaponTag;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes= CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Attributes"));
	
	SetReplicates(true);
}

void AFP_Character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFP_Character, EquipedGun);

	// Only replicate CurrentWeapon to simulated clients and manually sync CurrentWeapon with Owner when we're ready.
	// This allows us to predict weapon changing.
	DOREPLIFETIME_CONDITION(AFP_Character, CurrentWeapon, COND_SimulatedOnly);
}

float AFP_Character::GetHealth()
{
	if(Attributes)
	{
		return Attributes->GetHealth();
	}
	return 1.f;
}

float AFP_Character::GetMaxHealth()
{
	if(Attributes)
	{
		return Attributes->GetMaxHealth();
	}
	return 1.f;
}

ABaseWeapon* AFP_Character::GetEquippedGun()
{
	if (!EquipedGun)
	{
		return nullptr;
	}
	return EquipedGun;
}

void AFP_Character::ClientCameraRotation_Implementation()
{
	ServerCameraRotation(FirstPersonCameraComponent->GetComponentTransform());
}

void AFP_Character::ServerCameraRotation_Implementation(FTransform Transform)
{
	FirstPersonCameraComponent->SetWorldTransform(Transform);
}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();

	// Current weapon is replicated only to Simulated Clients so Sync the Weapon Manually
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		ServerSyncCurrentWeapon();
	}
}

void AFP_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GetWorldTimerManager().SetTimerForNextTick(this, &AFP_Character::SpawnDefaultInventory);
}

bool AFP_Character::DoesWeaponExistInInventory(const ABWeapon* InWeapon) const
{
	// Finds the specific tag in the map and returns true if it is there.
	return WeaponInventory.Contains(InWeapon->WeaponTag);
}

void AFP_Character::SetCurrentWeapon(ABWeapon* NewWeapon, ABWeapon* LastWeapon)
{
	UE_LOG(LogTemp, Error, TEXT("Set Current Weapon Function"));
	if (NewWeapon == LastWeapon)
	{
		if (NewWeapon == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("FUCK ITS NULL"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s, %s"), *NewWeapon->GetClass()->GetName(), *LastWeapon->GetClass()->GetName());
			
		}
		UE_LOG(LogTemp, Error, TEXT("UhOh They're the same"));
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("%s"), *CurrentWeaponTag.ToString());
	UE_LOG(LogTemp, Error, TEXT("Setting Current Weapon"))
	// Cancel Active Weapon Abilities
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTagsToCancel = FGameplayTagContainer(WeaponAbilityTag);
		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel);
	}

	UnEquipWeapon(LastWeapon);

	if (NewWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Please Get Here"));
		if (AbilitySystemComponent)
		{
			// Clear out Potential NoWeaponTag
			AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		}

		// Weapons coming from OnRep_CurrentWeapon won't have the owner set
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetOwningCharacter(this);
		CurrentWeapon->Equip();
		CurrentWeaponTag = CurrentWeapon->WeaponTag;

		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		}

		/*
		 *	ADD UI AND AMMO FUNCTIONALITY HERE
		 *
		 */

		NewWeapon->OnPrimaryClipAmmoChanged.AddDynamic(this, &AFP_Character::CurrentWeaponPrimaryClipAmmoChanged);

		if (AbilitySystemComponent)
		{
			// ammo tag delegate thingy
		}

		UAnimMontage* Equip1PMontage = CurrentWeapon->GetEquip1PMontage();
		if (Equip1PMontage && GetFirstPersonMesh())
		{
			GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Equip1PMontage);
		}

		UAnimMontage* Equip3PMontage = CurrentWeapon->GetEquip3PMontage();
		if (Equip3PMontage && GetThirdPersonMesh())
		{
			GetThirdPersonMesh()->GetAnimInstance()->Montage_Play(Equip3PMontage);
		}
	}
	else
	{
		// This Will Clear Tags
		UnEquipCurrentWeapon();
	}
}

void AFP_Character::UnEquipWeapon(ABWeapon* WeaponToUnEquip)
{
	// Might cause issues when called explicitly when the CurrentWeapon == WeaponToUnEquip
	if (WeaponToUnEquip)
	{
		WeaponToUnEquip->OnPrimaryClipAmmoChanged.RemoveDynamic(this, &AFP_Character::CurrentWeaponPrimaryClipAmmoChanged);

		if (AbilitySystemComponent)
		{
			// Setup After Ammo Attributes are made
			// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate()
		}

		WeaponToUnEquip->UnEquip();
	}
}

void AFP_Character::UnEquipCurrentWeapon()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);
		CurrentWeaponTag = NoWeaponTag;
		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
	}

	UnEquipWeapon(CurrentWeapon);
	CurrentWeapon = nullptr;

	/*
	 *		HANDLE AMMO SETUP AND ADDITION SETUP
	 *
	 */
}

void AFP_Character::CurrentWeaponPrimaryClipAmmoChanged(int32 OldPrimaryClipAmmo, int32 NewPrimaryClipAmmo)
{
}

void AFP_Character::OnRep_CurrentWeapon(ABWeapon* LastWeapon)
{
	bChangedWeaponLocally = false;

	UE_LOG(LogTemp, Error, TEXT("hmmm"));
	UE_LOG(LogTemp, Error, TEXT("%s, %s"), *CurrentWeapon->GetClass()->GetName(), *LastWeapon->GetClass()->GetName());
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void AFP_Character::SpawnDefaultInventory()
{
	UE_LOG(LogTemp, Error, TEXT("Spawning Inv"));
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	ABWeapon* NewWeapon = GetWorld()->SpawnActorDeferred<ABWeapon>(DefaultInventoryWeapons[0], FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	NewWeapon->bSpawnWithCollision = false;
	NewWeapon->FinishSpawning(FTransform::Identity);
		
	UE_LOG(LogTemp, Error, TEXT("Weapon Made. Time To Add to Inv"));
	AddWeaponToInventory(NewWeapon, true);
}

void AFP_Character::ServerSyncCurrentWeapon_Implementation()
{
	ClientSyncCurrentWeapon(CurrentWeapon);
}

bool AFP_Character::ServerSyncCurrentWeapon_Validate()
{
	return true;
}

void AFP_Character::ClientSyncCurrentWeapon_Implementation(ABWeapon* InWeapon)
{
	UE_LOG(LogTemp, Error, TEXT(" Client Sync %s, %s"), *CurrentWeapon->GetClass()->GetName(), *InWeapon->GetClass()->GetName());
	ABWeapon* LastWeapon = CurrentWeapon;
	CurrentWeapon = InWeapon;
	OnRep_CurrentWeapon(LastWeapon);
}

bool AFP_Character::ClientSyncCurrentWeapon_Validate(ABWeapon* InWeapon)
{
	return true;
}

USkeletalMeshComponent* AFP_Character::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

USkeletalMeshComponent* AFP_Character::GetThirdPersonMesh()
{
	return GetMesh();
}

// Called every frame
void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Jump Events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	// Bind Movement Events
	InputComponent->BindAxis("Move Forward / Backward", this, &AFP_Character::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &AFP_Character::MoveRight);

	// Bind Mouse Input
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EPlayerAbilityInputID",
			static_cast<int32>(EPlayerAbilityInputID::Confirm),
			static_cast<int32>(EPlayerAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

UPlayerAbilitySystemComponent* AFP_Character::GetAbilitySystemComponent()
{
	return AbilitySystemComponent;
}

void AFP_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}
void AFP_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if(AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EPlayerAbilityInputID",
			static_cast<int32>(EPlayerAbilityInputID::Confirm),
			static_cast<int32>(EPlayerAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
	
}

UAbilitySystemComponent* AFP_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFP_Character::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		//Grant abilities, but only on the server
		for(TSubclassOf<UPlayerGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this));
		}

		//now apply passives
		for(const TSubclassOf<UGameplayEffect>& GameplayEffect:PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
				GameplayEffect, 1, EffectContext);

			if(NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle =
					AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
		bAbilitiesInitialized = true;
	}
}

void AFP_Character::MoveForward(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AFP_Character::MoveRight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFP_Character::EquipGun(ABaseWeapon* const NewGun)
{
	EquipedGun = NewGun;
	EquipedGun->OnEquip();
}

void AFP_Character::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
                                 const FGameplayTagContainer& DamageTags, AFP_Character* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void AFP_Character::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
		DeathCheck();
	}
}

void AFP_Character::DeathCheck()
{
	if(Attributes)
	{
		if(Attributes->GetHealth()<=0.001f)
		{
			OnDeath();
		}
	}
}

bool AFP_Character::IsAlive()
{
	return Attributes->GetHealth() > 0.001f; 
}

FName AFP_Character::GetWeaponAttachPoint()
{
	return WeaponAttachPoint;
}

ABWeapon* AFP_Character::GetCurrentWeapon() const
{
	return CurrentWeapon;
}

bool AFP_Character::AddWeaponToInventory(ABWeapon* NewWeapon, bool bEquipWeapon)
{
	// If the weapon is in the inventory, pickup just ammo.
	if (DoesWeaponExistInInventory(NewWeapon))
	{
		// Play the pickup audio if playing locally
		USoundCue* PickupSound = NewWeapon->GetPickupSound();
		if (PickupSound && IsLocallyControlled())
		{
			UGameplayStatics::SpawnSoundAttached(PickupSound, GetRootComponent());
		}

		if (GetLocalRole() < ROLE_Authority)
		{
			return false;
		}

		/*
		 *	SET UP AMMO PICKUP
		 *
		 */

		NewWeapon->Destroy();
		return false;
	}

	if (GetLocalRole() < ROLE_Authority)
	{
		return false;
	}

	UE_LOG(LogTemp, Error, TEXT("Adding To map"));
	// Add the New Weapon the the Map.
	WeaponInventory.Add(NewWeapon->WeaponTag, NewWeapon);
	NewWeapon->SetOwningCharacter(this);
	NewWeapon->AddAbilities();

	if (bEquipWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Pog Equip Time"));
		EquipWeapon(NewWeapon);
		ClientSyncCurrentWeapon(CurrentWeapon);
	}
	
	return true;
}

bool AFP_Character::RemoveWeaponFromInventory(ABWeapon* WeaponToRemove)
{
	if (DoesWeaponExistInInventory(WeaponToRemove))
	{
		// If Weapon is Equipped then Remove safely
		if (WeaponToRemove == CurrentWeapon)
		{
			UnEquipCurrentWeapon();
		}

		// Remove the Weapon and Reset Weapon information
		WeaponInventory.Remove(WeaponToRemove->WeaponTag);
		WeaponToRemove->RemoveAbilities();
		WeaponToRemove->SetOwningCharacter(nullptr);
		WeaponToRemove->ResetWeapon();

		// Currently Does not do Drop. Setup Function for Drop

		return true;
	}
	
	return false;
}

void AFP_Character::RemoveAllWeaponsFromInventory()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	UnEquipCurrentWeapon();

	for (auto It = WeaponInventory.end(); It == WeaponInventory.begin();)
	{
		RemoveWeaponFromInventory(It.Value());
	}
}

void AFP_Character::EquipWeapon(ABWeapon* NewWeapon)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("Setting Weapons"));
		ServerEquipWeapon(NewWeapon);
		SetCurrentWeapon(NewWeapon, CurrentWeapon);
		bChangedWeaponLocally = true;
	}
	else
	{
		if (NewWeapon == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("what the fuck"));
		}
		SetCurrentWeapon(NewWeapon, CurrentWeapon);
	}
}

void AFP_Character::ServerEquipWeapon_Implementation(ABWeapon* NewWeapon)
{
	EquipWeapon(NewWeapon);
}

bool AFP_Character::ServerEquipWeapon_Validate(ABWeapon* NewWeapon)
{
	return true;
}

int32 AFP_Character::GetPrimaryClipAmmo() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetPrimaryClipAmmo();
	}
	return 0;
}

int32 AFP_Character::GetMaxPrimaryClipAmmo() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetMaxPrimaryClipAmmo();
	}
	return 0;
}

