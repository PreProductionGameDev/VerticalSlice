// 2022 ChronoOwl Studios

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
	UFUNCTION()
	void BurnCheck();

	UPROPERTY()
	UCapsuleComponent* Capsule;

	UPROPERTY()
	FTimerHandle BurnTimer;

	UPROPERTY()
	UNiagaraSystem* FireSystem;

};
