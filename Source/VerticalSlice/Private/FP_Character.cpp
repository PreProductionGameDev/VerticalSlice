// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"

#include "Core/Data.h"
#include "Core/TestAbilitySystemComponent.h"
#include "Core/TestAttributeSet.h"
#include "Core/TestGameplayAbility.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAbilitiesInitialized =false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTestAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes= CreateDefaultSubobject<UTestAttributeSet>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();
	
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

	// Bind Fire Events
	// Primary Fire
	InputComponent->BindAction("Primary Action", IE_Pressed, this, &AFP_Character::OnPrimaryActionPressed);
	InputComponent->BindAction("Primary Action", IE_Released, this, &AFP_Character::OnPrimaryActionReleased);
	// Secondary Fire
	InputComponent->BindAction("Secondary Action", IE_Pressed, this, &AFP_Character::OnSecondaryActionPressed);
	InputComponent->BindAction("Secondary Action", IE_Released, this, &AFP_Character::OnSecondaryActionReleased);

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
			"ETestAbilityInputID",
			static_cast<int32>(ETestAbilityInputID::Confirm),
			static_cast<int32>(ETestAbilityInputID::Cancel));

		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AFP_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Server GAS init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
			"ETestAbilityInputID",
			static_cast<int32>(ETestAbilityInputID::Confirm),
			static_cast<int32>(ETestAbilityInputID::Cancel));

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
		for(TSubclassOf<UTestGameplayAbility>& StartupAbility : GameplayAbilities)
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

void AFP_Character::OnPrimaryActionPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start Shoot"));
}

void AFP_Character::OnPrimaryActionReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("End Shoot"));
}

void AFP_Character::OnSecondaryActionPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Start Scope"));
}

void AFP_Character::OnSecondaryActionReleased()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("End Scope"));
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
	}
}
