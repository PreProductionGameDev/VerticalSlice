// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WorldCollision.h"
#include "ATA_Trace.generated.h"

/**
 *	Reusable and Reconfigurable Trace Target Actor. Subclass with Trace Shapes
 *	Used with AT_WaitForTargetDataUsingActor instead of the Default WaitTargetData AbilityTask as the default one will destroy TargetActor
 */
UCLASS()
class VERTICALSLICE_API AATA_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AATA_Trace();

	// Base Targeting Spread (Degrees)
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float BaseSpread;

	// Aiming Spread Modifier
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float AimingSpreadMod;

	// Continuous Targeting: Spread Increment
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadIncrement;

	// Continuous Targeting: Max Increment
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	float TargetingSpreadMax;

	// Current Spread from Continuous Targeting
	float CurrentTargetingSpread;

	// Whether Aiming or not
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	bool bUseAimingSpreadMod;

	// GameplayTags for when Aiming
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingTag;
	UPROPERTY(BlueprintReadWrite, Category = "Accuracy")
	FGameplayTag AimingRemovalTag;

	// Max lenght of shot.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	float MaxRange;

	// The Collision Profile for the Trace
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, meta = (ExposeOnSpawn = true), Category = "Trace")
	FCollisionProfileName TraceProfile;

	// Does the trace affect the aiming pitch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bTraceAffectsAimPitch;

	// Maximum Hit results per trace. 0 just Returns the trace end point.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	int32 MaxHitResultsPerTrace;

	// Number of Traces to perform at one time. Single bullet weapons will do a singular trace
	// MultiPellet weapons like Shotguns can do multiple traces
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	int32 NumberOfTraces;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bIgnoreBlockingHits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")		// CAN REMOVE. GAME DOES NOT NEED 3RD PERSON LMAO
	bool bTraceFromPlayerViewPoint;

	// HitResults will persist until confirmation/Cancellation or if new HitResult takes its place
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bUsePersistentHitResults;

	UFUNCTION(BlueprintCallable)
	virtual void ResetSpread();

	virtual float GetCurrentSpread() const;

	// Expose to Blueprint
	UFUNCTION(BlueprintCallable)
	void SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation);

	// Expose to Blueprint
	UFUNCTION(BlueprintCallable)
	virtual void SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer);

	UFUNCTION(BlueprintCallable)
	void SetDestroyOnConfirmation(bool bInDestroyOnConfirmation = false);

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void CancelTargeting() override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	// Traces as normal but will manually filter all hit actors
	virtual void LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);
	
	virtual void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false);

	virtual bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	virtual void StopTargeting();

protected:
	FVector CurrentTraceEnd;

	TArray<TWeakObjectPtr<AGameplayAbilityWorldReticle>> ReticleActors;
	TArray<FHitResult> PersistentHitResults;

	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;
	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor);

	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) PURE_VIRTUAL(AATA_Trace, return;);
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.0f) PURE_VIRTUAL(AATA_Trace, return;);

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();
	
};
