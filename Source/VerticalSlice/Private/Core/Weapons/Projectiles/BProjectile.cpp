// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Projectiles/BProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "FP_Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

/**
 * @name Stefan Petrie
 * @brief Set ups the Defaults for the Projectile
 */
ABProjectile::ABProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Root Component
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRootComponent"));
	}

	// Make the Sphere Collider
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(35.0f);
		RootComponent = CollisionComponent;
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABProjectile::OnOverlapBegin);
	}

	// Create the Default Projectile Mesh
	// Mesh has No Collision
	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetMobility(EComponentMobility::Movable);
		ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ProjectileMesh->SetupAttachment(RootComponent);
		
	}

	// Create the Default Projectile Movement Component
	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);
		ProjectileMovement->InitialSpeed = 3000.0f;
		ProjectileMovement->MaxSpeed = 3000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}

	// Enable Component Replication
	bReplicates = true;
}

void ABProjectile::SetEffectSpec(FGameplayEffectSpecHandle inEffectSpec)
{
	EffectSpec = inEffectSpec;
}

/**
 * @name Stefan Petrie
 * @brief Begin Play
 */
void ABProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->IgnoreActorWhenMoving(Owner, true);
}

void ABProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		Destroy();
		return;
	}

	FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);
	TArray<FHitResult> OutResults;

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 64, FColor::Green, false, 10);
	if (GetWorld()->SweepMultiByChannel(OutResults, GetActorLocation(), GetActorLocation() + 0.01, FQuat::Identity, ECC_Pawn, Sphere))
	{
		for (FHitResult& hit : OutResults)
		{
			if (AFP_Character* Character = Cast<AFP_Character>(hit.GetActor()))
			{
				// gets caster and target ability system components
				UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
				UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);

				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, FGameplayTag::RequestGameplayTag("Data.Damage"), 80.0f);			
			
				Character->GetPlayerAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
			}
		}
	}

	Destroy();
}

/**
 * @name Stefan Petrie
 * @brief On Tick Event. Can Remove
 */
void ABProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



