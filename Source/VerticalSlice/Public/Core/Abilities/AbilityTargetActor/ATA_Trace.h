// 2022 ChronoOwl Studios

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
	// Default Constructor
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

	// Ignore hits which block or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bIgnoreBlockingHits;
	// CAN REMOVE. GAME DOES NOT NEED 3RD PERSON LMAO. Might keep for further understanding
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")	
	bool bTraceFromPlayerViewPoint;
	// HitResults will persist until confirmation/Cancellation or if new HitResult takes its place
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "Trace")
	bool bUsePersistentHitResults;

	// Resets the weapon spread
	UFUNCTION(BlueprintCallable)
	virtual void ResetSpread();
	// Returns the Current Weapon Spread
	virtual float GetCurrentSpread() const;

	// Expose the StartLocation to Blueprint
	UFUNCTION(BlueprintCallable)
	void SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation);
	// Expose ShouldProduceTargetDataOnServer to Blueprint
	UFUNCTION(BlueprintCallable)
	virtual void SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer);
	// Expose DestroyOnConfirmation to Blueprint
	UFUNCTION(BlueprintCallable)
	void SetDestroyOnConfirmation(bool bInDestroyOnConfirmation = false);

	// The Targeting System
	// Targeting is used for passing data between the Client and Server in GameplayAbilities
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	// On Game Start
	virtual void BeginPlay() override;
	// On Game End
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// On Game Tick
	virtual void Tick(float DeltaSeconds) override;

	// Traces as normal but will manually filter all hit actors
	virtual void LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params);
	// Aim using the PlayerController
	virtual void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false);
	// Reduces the Ray to the Ability Range
	virtual bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

	// Stop the GAS Targeting
	virtual void StopTargeting();

protected:
	// The End Position of the Current LineTrace
	FVector CurrentTraceEnd;
	// WeakObjectPointers of Reticle Actors  
	TArray<TWeakObjectPtr<AGameplayAbilityWorldReticle>> ReticleActors;
	// Array of PersistentHitResults
	TArray<FHitResult> PersistentHitResults;

	// Generate the TargetData
	virtual FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;
	// Perform the Trace
	virtual TArray<FHitResult> PerformTrace(AActor* InSourceActor);
	// Create the Trace
	virtual void DoTrace(TArray<FHitResult>& HitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params) PURE_VIRTUAL(AATA_Trace, return;);
	// Displays the Debug view of the Trace
	virtual void ShowDebugTrace(TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = 2.0f) PURE_VIRTUAL(AATA_Trace, return;);

	// Reticles can be used when Aiming to display additional actors. Possibly not used but the option is there
	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	virtual void DestroyReticleActors();
	
};
