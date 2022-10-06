// 2022 ChronoOwl Studios
// Jacob

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "TrailHitbox.generated.h"

UCLASS()
class VERTICALSLICE_API ATrailHitbox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrailHitbox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Checks if it needs to burn overlapping players
	UFUNCTION()
	void BurnCheck();

	// Capsule collision component
	UPROPERTY()
	UCapsuleComponent* Capsule;

	// Handle to trigger burn check
	UPROPERTY()
	FTimerHandle BurnTimer;

	//Particle system to show fire
	UPROPERTY()
	UNiagaraSystem* FireSystem;

	UPROPERTY()
	USoundBase* Sound;

};
