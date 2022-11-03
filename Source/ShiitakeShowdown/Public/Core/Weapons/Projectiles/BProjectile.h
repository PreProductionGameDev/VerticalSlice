// 2022 ChronoOwl Studios
// Stefan Petrie

#pragma once

#include "CoreMinimal.h"
#include "Core/Abilities/GameplayEffects/GameplayEffectContext.h"
#include "GameFramework/Actor.h"
#include "BProjectile.generated.h"

/**
 *  The Projectile Class
 *  Currently Solely used for the Rocket Weapon
 */
UCLASS()
class SHIITAKESHOWDOWN_API ABProjectile : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	ABProjectile();

	void SetEffectSpec(FGameplayEffectSpecHandle inEffectSpec);

	void SetDamage(float inDamage);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);
	
	// Projectile Movement Component
	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement;
	// Component for Projectile Collision
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComponent;
	// The Projectile Mesh
	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayEffectSpecHandle EffectSpec;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ExplosionRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UNiagaraSystem* NiagaraComponent;
	
	UPROPERTY()
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USoundBase* ExplosionSound;
};
