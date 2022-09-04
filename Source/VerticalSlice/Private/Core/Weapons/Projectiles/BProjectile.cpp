// 2022 ChronoOwl Studios


#include "Core/Weapons/Projectiles/BProjectile.h"

#include <complex>

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABProjectile::ABProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRootComponent"));
	}

	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(35.0f);
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMesh)
	{
		ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
		ProjectileMesh->SetMobility(EComponentMobility::Movable);
		ProjectileMesh->SetupAttachment(RootComponent);
	}

	if (!ProjectileMovement)
	{
		ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);
		ProjectileMovement->InitialSpeed = 3000.0f;
		ProjectileMovement->MaxSpeed = 3000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

