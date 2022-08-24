// 2022 ChronoOwl Studios


#include "Core/Elements/Fire/TrailHitbox.h"

#include "AbilitySystemGlobals.h"
#include "FP_Character.h"
#include "Core/PlayerAttributeSet.h"


// Sets default values
ATrailHitbox::ATrailHitbox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	RootComponent = Capsule;
	Capsule->SetCapsuleRadius(50);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>NiagaraSystem(TEXT("/Game/ShiitakeSorcerers/VFX/Pyromancy/Fireball/NS_Trail_2"));
	if (ensure(NiagaraSystem.Object != nullptr))
	{
		FireSystem = NiagaraSystem.Object;
	}
}

// Called when the game starts or when spawned
void ATrailHitbox::BeginPlay()
{
	//ensure the hitbox despawns after 2 and a half seconds
	Super::BeginPlay();
	SetLifeSpan(2.5f);

	UNiagaraFunctionLibrary::SpawnSystemAttached(FireSystem, Capsule, NAME_None, FVector(0,0,-160),FRotator(90,0,0), EAttachLocation::Type::KeepRelativeOffset, true);
	
	//starts checking to burn layers every .1 second
	FTimerDynamicDelegate DynamicDelegate;
	DynamicDelegate.BindUFunction(this, "BurnCheck");

	FTimerManager& TimerManager= GetWorld()->GetTimerManager();
	BurnTimer = TimerManager.K2_FindDynamicTimerHandle(DynamicDelegate);
	TimerManager.SetTimer(BurnTimer, DynamicDelegate, 0.1f, true,0);
	
}

void ATrailHitbox::BurnCheck()
{
	//gets all of the overlapping AFP_Character actors
	TArray<AActor*> OverlappingActors;
	Capsule->GetOverlappingActors(OverlappingActors, AFP_Character::StaticClass());

	for(AActor* OverlappingActor : OverlappingActors)
	{
		//sends the event back to the ability system to do damage
		AFP_Character* Character = Cast<AFP_Character>(OverlappingActor);
		if(IsValid(Character) && (Character != Cast<AFP_Character>(GetInstigator())))
		{
			//gets caster and target ability system components
			UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
			UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);

			//creates gameplay effect
			UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), "GE_TrailBurn");
			GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

			//set health modifier information
			FGameplayModifierInfo DamageModifierInfo;
			DamageModifierInfo.ModifierOp = EGameplayModOp::Additive;
			DamageModifierInfo.Attribute = UPlayerAttributeSet::GetHealthAttribute();
			DamageModifierInfo.ModifierMagnitude = FScalableFloat(-2.0f);
			GameplayEffect->Modifiers.Add(DamageModifierInfo);

			//tags to ignore
			GameplayEffect->ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
			GameplayEffect->ApplicationTagRequirements.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dying")));

			//apply the created effect
			AbilitySystemComponent->ApplyGameplayEffectToTarget(GameplayEffect,TargetAbilitySystemComponent);
		}
	}
}


