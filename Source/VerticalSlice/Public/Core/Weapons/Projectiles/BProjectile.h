// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BProjectile.generated.h"

UCLASS()
class VERTICALSLICE_API ABProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABProjectile();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComponent;
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;
};
