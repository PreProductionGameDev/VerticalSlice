// 2022 ChronoOwl Studios


#include "Core/Weapons/Projectiles/BProjectile.h"

// Sets default values
ABProjectile::ABProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

