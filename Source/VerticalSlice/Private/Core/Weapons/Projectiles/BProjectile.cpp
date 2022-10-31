// 2022 ChronoOwl Studios
// Stefan Petrie

#include "Core/Weapons/Projectiles/BProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemGlobals.h"
#include "FP_Character.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

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
		CollisionComponent->OnComponentHit.AddDynamic(this, &ABProjectile::OnOverlapBegin);
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

void ABProjectile::SetDamage(float inDamage)
{
	Damage = inDamage;
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

void ABProjectile::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit)
{
	if (NiagaraComponent)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraComponent, GetActorLocation());
	}
	
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
				FVector CharLoc = Character->GetActorLocation();
				FVector ProjLoc = GetActorLocation();

				float Distance =  FVector::Dist(CharLoc, ProjLoc);

				float falloff = FMath::Clamp(1.5f * FMath::Cos(FMath::Pow(Distance / ExplosionRadius, 0.3f) * (PI / 2)), 0, 1);

				Damage = static_cast<int>(falloff * Damage);
				
				// gets caster and target ability system components
				UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this);
				UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Character);

				//Damage = Damage * Distance;
				
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpec, FGameplayTag::RequestGameplayTag(FName("Data.Damage")), static_cast<float>(Damage));
				
				UE_LOG(LogTemp, Warning, TEXT("%f"), Damage);
				
				
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



