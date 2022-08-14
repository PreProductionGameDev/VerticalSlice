// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"


#include "Blueprint/UserWidget.h"
#include "Core/Data.h"
#include "Core/PlayerAbilitySystemComponent.h"
#include "Core/PlayerAttributeSet.h"
#include "Core/PlayerGameplayAbility.h"
#include "Core/Abilities/AttributeSets/AmmoAttributeSet.h"
#include "Core/Player/FPPlayerController.h"
#include "Core/UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAbilitiesInitialized =false;

	// Set the Default Cached Tags
	NoWeaponTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.None"));
	WeaponAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Weapon"));
	CurrentWeaponTag = NoWeaponTag;
	WeaponAmmoNoneTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Ammo.None"));

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);

	// Add the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// Create the Player Health AttributeSet
	Attributes = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Attributes"));

	// Create the Player Ammo AttributeSet
	AmmoAttributes = CreateDefaultSubobject<UAmmoAttributeSet>(TEXT("AmmoAttributeSet"));
	
	// Enable Replication
	SetReplicates(true);
}

void AFP_Character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Only replicate CurrentWeapon to simulated clients and manually sync CurrentWeapon with Owner when we're ready.
	// This allows us to predict weapon changing.
	DOREPLIFETIME_CONDITION(AFP_Character, CurrentWeapon, COND_SimulatedOnly);
}

void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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

	// Bind Ability System Inputs
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

float AFP_Character::GetHealth()
{
	// Returns health if Health Attributes Exist
	if(Attributes)
	{
		return Attributes->GetHealth();
	}
	return 1.f;
}

float AFP_Character::GetMaxHealth()
{
	// Returns Max Health if Health Attributes Exist
	if(Attributes)
	{
		return Attributes->GetMaxHealth();
	}
	return 1.f;
}

void AFP_Character::ClientCameraRotation_Implementation()
{
	// Sets the Camera Rotation on the server to that of the local client
	ServerCameraRotation(FirstPersonCameraComponent->GetComponentTransform());
}

void AFP_Character::ServerCameraRotation_Implementation(FTransform Transform)
{
	// Sets the Camera Position
	FirstPersonCameraComponent->SetWorldTransform(Transform);
}


USkeletalMeshComponent* AFP_Character::GetFirstPersonMesh()
{
	return FirstPersonMesh;
}

USkeletalMeshComponent* AFP_Character::GetThirdPersonMesh()
{
	return GetMesh();
}

UPlayerAbilitySystemComponent* AFP_Character::GetPlayerAbilitySystemComponent()
{
	return AbilitySystemComponent;
}


bool AFP_Character::IsAlive()
{
	// Tiny edge case taken into account
	return Attributes->GetHealth() > 0.001f; 
}

bool AFP_Character::ClientClearWidgets()
{
	const APlayerController* LocalController = UGameplayStatics::GetPlayerController(this,0);

	return IsOwnedBy(LocalController);
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

		// Check if Reserve Ammo is Max
		const FGameplayAttribute ReserveAttribute = AmmoAttributes->GetReserveAmmoAttributeFromTag(NewWeapon->PrimaryAmmoType);
		const FGameplayAttribute MaxReserveAttribute = AmmoAttributes->GetMaxReserveAmmoAttributeFromTag(NewWeapon->PrimaryAmmoType);
		if (ReserveAttribute.IsValid() && MaxReserveAttribute.IsValid())
		{
			if (AbilitySystemComponent->GetNumericAttribute(ReserveAttribute) == AbilitySystemComponent->GetNumericAttribute(MaxReserveAttribute))
			{
				return false;
			}
		}
		
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
		
		// Using Dynamic Instant Gameplay Effect to give the weapon ammo
		UGameplayEffect* GEAmmoPickup = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("AmmoPickup")));
		// Create the Dynamic Instant Gameplay Effect
		if (NewWeapon->PrimaryAmmoType != WeaponAmmoNoneTag)
		{
			int32 ID = GEAmmoPickup->Modifiers.Num();
			GEAmmoPickup->Modifiers.SetNum(ID + 1);
			FGameplayModifierInfo& InfoAmmo = GEAmmoPickup->Modifiers[ID];
			// Add A Clip amount to the ammo pool
			InfoAmmo.ModifierMagnitude = FScalableFloat(NewWeapon->GetPrimaryClipAmmo());
			InfoAmmo.ModifierOp = EGameplayModOp::Additive;
			InfoAmmo.Attribute = UAmmoAttributeSet::GetReserveAmmoAttributeFromTag(NewWeapon->PrimaryAmmoType);
		}

		if (GEAmmoPickup->Modifiers.Num() > 0)
		{
			AbilitySystemComponent->ApplyGameplayEffectToSelf(GEAmmoPickup, 1.0f, AbilitySystemComponent->MakeEffectContext());
		}
		
		// Destroy Entity since only ammo is collected
		NewWeapon->Destroy();
		return false;
	}

	if (GetLocalRole() < ROLE_Authority)
	{
		return false;
	}

	// Add the New Weapon the the Map.
	WeaponInventory.Add(NewWeapon->WeaponTag, NewWeapon);
	NewWeapon->SetOwningCharacter(this);
	NewWeapon->AddAbilities();

	// If User needs to equip the weapon
	// Equip and Sync Weapon
	if (bEquipWeapon)
	{
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
		WeaponToRemove->DeleteWeapon();

		// Currently Does not handle drop
		// TODO: Discuss and possibly setup Functionality for Drop

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

	// UnEquips Current Weapon
	UnEquipCurrentWeapon();

	TArray<FGameplayTag> WeaponKeys;
	WeaponInventory.GetKeys(WeaponKeys);

	for (FGameplayTag WeaponKey : WeaponKeys)
	{
		RemoveWeaponFromInventory(WeaponInventory.FindRef(WeaponKey));
	}
	WeaponInventory.Empty();
}

void AFP_Character::EquipWeapon(ABWeapon* NewWeapon)
{
	// Client tells server to swap weapon, and weapon is changed locally
	// Otherwise client is server, just swap weapon
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerEquipWeapon(NewWeapon);
		SetCurrentWeapon(NewWeapon, CurrentWeapon);
		bChangedWeaponLocally = true;
	}
	else
	{
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

void AFP_Character::CreateHitMarker_Implementation(bool isHit)
{
	GetPlayerHUD()->MakeHitMarker(isHit);
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

int32 AFP_Character::GetPrimaryReserveAmmo() const
{
	// Access the AmmoAttributeSet and return the Ammo Value related to the weapon.
	if (CurrentWeapon && AmmoAttributes)
	{
		FGameplayAttribute Attribute = AmmoAttributes->GetReserveAmmoAttributeFromTag(CurrentWeapon->PrimaryAmmoType);
		if (Attribute.IsValid())
		{
			return AbilitySystemComponent->GetNumericAttribute(Attribute);
		}
	}
	return 0;
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

	const APlayerController* LocalController = UGameplayStatics::GetPlayerController(this,0);

	if(IsOwnedBy(LocalController))
	{
		// Clear the HUD. Used for Server Transitions
		UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
		if ( World && World->IsGameWorld() )
		{
			if ( UGameViewportClient* ViewportClient = World->GetGameViewport() )
			{
				ViewportClient->RemoveAllViewportWidgets();
			}
		}
	}
	// Create the PlayerHUD onto the controller.
	if (AFPPlayerController* PlayerController = Cast<AFPPlayerController>(GetController()))
	{
		PlayerController->CreateHUD();
	}
	
}

void AFP_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorldTimerManager().SetTimerForNextTick(this, &AFP_Character::SpawnDefaultInventory);
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
	// Create the PlayerHUD onto the controller.
	AFPPlayerController* PlayerController = Cast<AFPPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->CreateHUD();
	}
}

void AFP_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// Bind AbilitySystem Inputs 
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

UPlayerHUD* AFP_Character::GetPlayerHUD() const
{
	AFPPlayerController* PlayerController = Cast<AFPPlayerController>(GetController());
	if (PlayerController)
	{
		return PlayerController->GetPlayerHUD();
	}

	return nullptr;
}

UAbilitySystemComponent* AFP_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFP_Character::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);

	//Grant abilities, but only on the server
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
		// Grant all Start Up Abilities
		for(TSubclassOf<UPlayerGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID),
				this));
		}

		// Grant all Passive GamePlay Effects
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

void AFP_Character::ServerSetWeaponOnTag_Implementation(FGameplayTag WeaponTag)
{
	SwapToWeaponOnTag(WeaponTag);
}

bool AFP_Character::ServerSetWeaponOnTag_Validate()
{
	return true;
}

void AFP_Character::ClientSetWeapon_Implementation(ABWeapon* WeaponToEquip)
{
	EquipWeapon(WeaponToEquip);
}

void AFP_Character::SwapToWeaponOnTag(FGameplayTag WeaponTag)
{
	if (!WeaponInventory.Contains(WeaponTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WEAPON TAG"));
		return;
	}

	if (CurrentWeapon == WeaponInventory.FindRef(WeaponTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPON IS CURRENT WEAPON"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("CALLED EQUIP WEAPON. ISSUE MUST BE HERE"));
	ClientSetWeapon(WeaponInventory.FindRef(WeaponTag));
	//SetCurrentWeapon(WeaponInventory.FindRef(WeaponTag), CurrentWeapon);
}

bool AFP_Character::DoesWeaponExistInInventory(const ABWeapon* InWeapon) const
{
	// Finds the specific tag in the map and returns true if it is there.
	return WeaponInventory.Contains(InWeapon->WeaponTag);
}

void AFP_Character::SetCurrentWeapon(ABWeapon* NewWeapon, ABWeapon* LastWeapon)
{
	if (NewWeapon == LastWeapon)
	{
		return;
	}
	
	// Cancel Active Weapon Abilities
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer AbilityTagsToCancel = FGameplayTagContainer(WeaponAbilityTag);
		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel);
	}

	// UnEquip last weapon
	UnEquipWeapon(LastWeapon);

	if (NewWeapon)
	{
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

		// Set the Equipped Weapon tag
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
		}

		// Set the Ammo Values onto the HUD
		AFPPlayerController* PlayerController = GetController<AFPPlayerController>();
		if (PlayerController && PlayerController->IsLocalController())
		{
			PlayerController->SetPrimaryClipAmmo(CurrentWeapon->GetPrimaryClipAmmo());
			PlayerController->SetPrimaryReserveAmmo(GetPrimaryReserveAmmo());
			PlayerController->GetPlayerHUD()->SetWeaponIcon(CurrentWeapon->GetUITexture());
		}

		// Setup Ammo Changed Delegates for UI changes
		NewWeapon->OnPrimaryClipAmmoChanged.AddDynamic(this, &AFP_Character::CurrentWeaponPrimaryClipAmmoChanged);
		if (AbilitySystemComponent)
		{
			PrimaryReserveAmmoChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAmmoAttributeSet::GetReserveAmmoAttributeFromTag(CurrentWeapon->PrimaryAmmoType)).AddUObject(this, &AFP_Character::CurrentWeaponPrimaryReserveAmmoChanged);
		}

		// Handle FirstPerson Equip Montage
		// TODO: Weapon Equip Animations
		UAnimMontage* Equip1PMontage = CurrentWeapon->GetEquip1PMontage();
		if (Equip1PMontage && GetFirstPersonMesh())
		{
			GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(Equip1PMontage);
		}	
		// Handle ThirdPerson Equip Montage
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
			// Remove the UI Update Delegate for Weapon Ammo
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAmmoAttributeSet::GetReserveAmmoAttributeFromTag(WeaponToUnEquip->PrimaryAmmoType)).Remove(PrimaryReserveAmmoChangedDelegateHandle);
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
	 *		TODO:  HANDLE AMMO SETUP AND ADDITION SETUP
	 *
	 */
	if (AFPPlayerController* PC = GetController<AFPPlayerController>(); PC && PC->IsLocalController())
	{
		PC->SetPrimaryClipAmmo(0);
		PC->SetPrimaryReserveAmmo(0);
	}
}

void AFP_Character::CurrentWeaponPrimaryClipAmmoChanged(int32 OldPrimaryClipAmmo, int32 NewPrimaryClipAmmo)
{
	AFPPlayerController* PlayerController = GetController<AFPPlayerController>();
	if (PlayerController && PlayerController->IsLocalController())
	{
		PlayerController->SetPrimaryClipAmmo(NewPrimaryClipAmmo);
	}
}

void AFP_Character::OnRep_CurrentWeapon(ABWeapon* LastWeapon)
{
	// Handles Prediction for swapping weapons
	bChangedWeaponLocally = false;
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void AFP_Character::SpawnDefaultInventory()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}

	// Cycle through all Weapons and add them all in
	// Only Equip the First Weapon
	for (auto WeaponToAdd: DefaultInventoryWeapons)
	{
		ABWeapon* NewWeapon = GetWorld()->SpawnActorDeferred<ABWeapon>(WeaponToAdd, FTransform::Identity, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		NewWeapon->bSpawnWithCollision = false;
		NewWeapon->FinishSpawning(FTransform::Identity);

		const bool bEquipFirstWeapon = WeaponToAdd == DefaultInventoryWeapons[0];
		AddWeaponToInventory(NewWeapon, bEquipFirstWeapon);
	}
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
	// Ensures the Weapons are synced correctly
	ABWeapon* LastWeapon = CurrentWeapon;
	CurrentWeapon = InWeapon;
	OnRep_CurrentWeapon(LastWeapon);
}

bool AFP_Character::ClientSyncCurrentWeapon_Validate(ABWeapon* InWeapon)
{
	return true;
}

void AFP_Character::CurrentWeaponPrimaryReserveAmmoChanged(const FOnAttributeChangeData& Data)
{
	AFPPlayerController* PlayerController = GetController<AFPPlayerController>();
	if (PlayerController && PlayerController->IsLocalController())
	{
		PlayerController->SetPrimaryReserveAmmo(GetPrimaryReserveAmmo());
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

