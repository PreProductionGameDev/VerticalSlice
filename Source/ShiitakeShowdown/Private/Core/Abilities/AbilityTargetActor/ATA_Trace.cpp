// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Abilities/AbilityTargetActor/ATA_Trace.h"

#include "AbilitySystemComponent.h"

AATA_Trace::AATA_Trace()
{
	bDestroyOnConfirmation = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
	MaxHitResultsPerTrace = 1;
	NumberOfTraces = 1;
	bIgnoreBlockingHits = false;
	bTraceAffectsAimPitch = true;
	bTraceFromPlayerViewPoint = true;
	MaxRange = 999999.0f;
	bUseAimingSpreadMod = false;
	BaseSpread = 0.0f;
	AimingSpreadMod = 0.0f;
	TargetingSpreadIncrement = 0.0f;
	TargetingSpreadMax = 0.0f;
	CurrentTargetingSpread = 0.0f;
	bUsePersistentHitResults = true;
}

void AATA_Trace::ResetSpread()
{
	// Resets Spread to Default Values
	bUseAimingSpreadMod = false;
	BaseSpread = 0.0f;
	AimingSpreadMod = 0.0f;
	TargetingSpreadIncrement = 0.0f;
	TargetingSpreadMax = 0.0f;
	CurrentTargetingSpread = 0.0f;
}

float AATA_Trace::GetCurrentSpread() const
{
	// Calculate Spread
	float FinalSpread = BaseSpread + CurrentTargetingSpread;

	// Multiply Spread by the Aiming Multiplier if being Aimed
	if (bUseAimingSpreadMod && AimingTag.IsValid() && AimingRemovalTag.IsValid())
	{
		UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get();
		if (ASC && (ASC->GetTagCount(AimingTag) > ASC->GetTagCount(AimingRemovalTag)))
		{
			FinalSpread *= AimingSpreadMod;
		}
	}
	return FinalSpread;
}

void AATA_Trace::SetStartLocation(const FGameplayAbilityTargetingLocationInfo& InStartLocation)
{
	StartLocation = InStartLocation;
}

void AATA_Trace::SetShouldProduceTargetDataOnServer(bool bInShouldProduceTargetDataOnServer)
{
	ShouldProduceTargetDataOnServer = bInShouldProduceTargetDataOnServer;
}

void AATA_Trace::SetDestroyOnConfirmation(bool bInDestroyOnConfirmation)
{
	bDestroyOnConfirmation = bInDestroyOnConfirmation;
}

void AATA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	// Enable Tick when Starting to Target
	SetActorTickEnabled(true);
	// Cache Ability and Actor
	OwningAbility = Ability;
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	// This is a Lazy way of Emptying and Repopulating the Reticle Actors
	DestroyReticleActors();

	// If Reticle class is valid, Spawn Reticles
	if (ReticleClass)
	{
		for (int32 i = 0; i < MaxHitResultsPerTrace * NumberOfTraces; i++)
		{
			SpawnReticleActor(GetActorLocation(), GetActorRotation());
		}
	}

	// Empty Persistent Hit Results if being used
	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AATA_Trace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	// If SourceActor is Valid. Do Trace, Make Data and broadcast the data
	if (SourceActor)
	{
		TArray<FHitResult> HitResults = PerformTrace(SourceActor);
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);
// Debug Draw for Trace
#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForDuration, 2.0f);
		}
#endif
	}

	// Empty Persistent Hit Results if being used
	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AATA_Trace::CancelTargeting()
{
	// Store ActorInfo and AbilitySystemComponent
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* ASC = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (ASC)
	{
		ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GenericCancel, OwningAbility->GetCurrentAbilitySpecHandle(), OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey()).Remove(GenericCancelHandle);
	}
	else
	{
		ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"), *GetName());
	}
	// Broadcast the CanceledDelegate
	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	// Disable tick now that we are no longer targeting
	SetActorTickEnabled(false);
	// Empty Persistent Hit Results if being used
	if (bUsePersistentHitResults)
	{
		PersistentHitResults.Empty();
	}
}

void AATA_Trace::BeginPlay()
{
	Super::BeginPlay();

	// Start with Tick Disabled. We Enable it in StartTargeting() and Disabled in StopTargeting()
	SetActorTickEnabled(false);
}

void AATA_Trace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Destroy all Reticles that created
	DestroyReticleActors();
	
	Super::EndPlay(EndPlayReason);
}

void AATA_Trace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TArray<FHitResult> HitResults;
	if (bDebug || bUsePersistentHitResults)
	{
		// Only need to trace on tick if we're showing debug or if we use persistent hit results. Otherwise we just use the confirmation trace
		HitResults = PerformTrace(SourceActor);
	}
#if ENABLE_DRAW_DEBUG
	if (SourceActor && bDebug)
	{
		ShowDebugTrace(HitResults, EDrawDebugTrace::Type::ForOneFrame);
	}
#endif
}

void AATA_Trace::LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams Params)
{
	check(World);
	// Get Hit Results
	TArray<FHitResult> HitResults;
	World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);
	TArray<FHitResult> FilteredHitResults;
	// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();

	// Filter all hit Results
	for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
	{
		FHitResult& Hit = HitResults[HitIdx];

		if (!IsValid(Hit.GetActor()) || FilterHandle.FilterPassesForActor(Hit.GetActor()))
		{
			Hit.TraceStart = TraceStart;
			Hit.TraceEnd = End;

			FilteredHitResults.Add(Hit);
		}
	}
	// Set OutHitResults
	OutHitResults = FilteredHitResults;
	return;
}

void AATA_Trace::AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch)
{
	// Server and Launching Client Only
	if (!OwningAbility) 
	{
		return;
	}
	// Default Values in case of AI controller
	FVector ViewStart = TraceStart;
	FRotator ViewRot = StartLocation.GetTargetingTransform().GetRotation().Rotator();

	if (MasterPC)
	{
		MasterPC->GetPlayerViewPoint(ViewStart, ViewRot);
	}

	// Calculate View and End Directions to ClipCameraRay
	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);
	ClipCameraRayToAbilityRange(ViewStart, ViewDir, TraceStart, MaxRange, ViewEnd);
	// Use First hit result and filter results
	TArray<FHitResult> HitResults;
	LineTraceWithFilter(HitResults, InSourceActor->GetWorld(), Filter, ViewStart, ViewEnd, TraceProfile.Name, Params);
	// Calculate Spread
	CurrentTargetingSpread = FMath::Min(TargetingSpreadMax, CurrentTargetingSpread + TargetingSpreadIncrement);
	// If the Hit Results exist, UseTraceResults
	const bool bUseTraceResult = HitResults.Num() > 0 && (FVector::DistSquared(TraceStart, HitResults[0].Location) <= (MaxRange * MaxRange));
	// Calculate AdjustedEnd if HitResult exists
	const FVector AdjustedEnd = (bUseTraceResult) ? HitResults[0].Location : ViewEnd;
	// Calculate AdjustedAimDirection
	FVector AdjustedAimDir = (AdjustedEnd - TraceStart).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	// Calculate Adjusted Aim Direction if UsingTraceResult
	if (!bTraceAffectsAimPitch && bUseTraceResult)
	{
		FVector OriginalAimDir = (ViewEnd - TraceStart).GetSafeNormal();
		if (!OriginalAimDir.IsZero())
		{
			// Convert to Angles and Use Original Pitch
			const FRotator OriginalAimRot = OriginalAimDir.Rotation();
			FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
			AdjustedAimRot.Pitch = OriginalAimRot.Pitch;
			AdjustedAimDir = AdjustedAimRot.Vector();
		}
	}

	// Set CurrentSpread
	const float CurrentSpread = GetCurrentSpread();
	// Calculate Random Spread range
	const float ConeHalfAngle = FMath::DegreesToRadians(CurrentSpread * 0.5f);
	const int32 RandomSeed = FMath::Rand();
	FRandomStream  WeaponRandomStream(RandomSeed);
	const FVector ShootDir = WeaponRandomStream.VRandCone(AdjustedAimDir, ConeHalfAngle, ConeHalfAngle);

	// Set OutTraceEnd
	OutTraceEnd = TraceStart + (ShootDir * MaxRange);
}

bool AATA_Trace::ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition)
{
	FVector CameraToCenter = AbilityCenter - CameraLocation;
	float DotToCenter = FVector::DotProduct(CameraToCenter, CameraDirection);
	// If this fails, We're pointed away from center
	if (DotToCenter >= 0)	
	{
		float DistanceSquared = CameraToCenter.SizeSquared() - (DotToCenter * DotToCenter);
		float RadiusSquared = (AbilityRange * AbilityRange);

		if (DistanceSquared <= RadiusSquared)
		{
			float DistanceFromCamera = FMath::Sqrt(RadiusSquared - DistanceSquared);
			float DistanceAlongRay = DotToCenter + DistanceFromCamera;					//Subtracting instead of adding will get the other intersection point
			ClippedPosition = CameraLocation + (DistanceAlongRay * CameraDirection);	//Cam aim point clipped to range sphere
			return true;
		}
	}
	return false;
}

void AATA_Trace::StopTargeting()
{
	// Disable Tick to save Processing Speed
	SetActorTickEnabled(false);
	// Destroy All existing Reticle Actors
	DestroyReticleActors();
	// Clear added callbacks
	TargetDataReadyDelegate.Clear();
	CanceledDelegate.Clear();

	// Remove GenericDelegates
	if (GenericDelegateBoundASC)
	{
		GenericDelegateBoundASC->GenericLocalConfirmCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		GenericDelegateBoundASC->GenericLocalCancelCallbacks.RemoveDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
		GenericDelegateBoundASC = nullptr;
	}
}

FGameplayAbilityTargetDataHandle AATA_Trace::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	// Create a Empty DataHandle
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	// Populate DataHandle with HitResults
	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}

	// Return PopulatedDataHandle
	return ReturnDataHandle;
}

TArray<FHitResult> AATA_Trace::PerformTrace(AActor* InSourceActor)
{
	// Disable Complex Trace
	bool bTraceComplex = false;
	// Ignore Source Actor
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(InSourceActor);
	// Setup Collision Query Params
	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGSGATA_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bIgnoreBlocks = bIgnoreBlockingHits;
	// Set Start Location
	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;

	if (MasterPC)
	{
		FVector ViewStart;
		FRotator ViewRot;
		MasterPC->GetPlayerViewPoint(ViewStart, ViewRot);

		TraceStart = bTraceFromPlayerViewPoint ? ViewStart : TraceStart;
	}
	
	if (bUsePersistentHitResults)
	{
		// Clear any blocking hit results, invalid Actors, or actors out of range
		for (int32 i = PersistentHitResults.Num() - 1; i >= 0; i--)
		{
			FHitResult& HitResult = PersistentHitResults[i];

			if (HitResult.bBlockingHit || !IsValid(HitResult.GetActor()) || FVector::DistSquared(TraceStart, HitResult.GetActor()->GetActorLocation()) > (MaxRange * MaxRange))
			{
				PersistentHitResults.RemoveAt(i);
			}
		}
	}
	// Create the ReturnHitResults Array
	TArray<FHitResult> ReturnHitResults;
	for (int32 TraceIndex = 0; TraceIndex < NumberOfTraces; TraceIndex++)
	{
		// Effective on server and launching client only
		AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd);
		SetActorLocationAndRotation(TraceEnd, SourceActor->GetActorRotation());
		CurrentTraceEnd = TraceEnd;
		// Populate HitResults
		TArray<FHitResult> TraceHitResults;
		DoTrace(TraceHitResults, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
		// Cycle through All HitResults
		for (int32 j = TraceHitResults.Num() - 1; j >= 0; j--)
		{
			if (MaxHitResultsPerTrace >= 0 && j + 1 > MaxHitResultsPerTrace)
			{
				// Trim to MaxHitResultsPerTrace
				TraceHitResults.RemoveAt(j);
				continue;
			}

			FHitResult& HitResult = TraceHitResults[j];

			// Reminder: if bUsePersistentHitResults, Number of Traces = 1
			if (bUsePersistentHitResults)
			{
				// This is looping backwards so that further objects from player are added first to the queue.
				// This results in closer actors taking precedence as the further actors will get bumped out of the TArray.
				if (IsValid(HitResult.GetActor()) && (!HitResult.bBlockingHit || PersistentHitResults.Num() < 1))
				{
					bool bActorAlreadyInPersistentHits = false;
					// Make sure PersistentHitResults doesn't have this hit actor already
					for (int32 k = 0; k < PersistentHitResults.Num(); k++)
					{
						FHitResult& PersistentHitResult = PersistentHitResults[k];
						if (PersistentHitResult.GetActor() == HitResult.GetActor())
						{
							bActorAlreadyInPersistentHits = true;
							break;
						}
					}
					if (bActorAlreadyInPersistentHits)
					{
						continue;
					}
					if (PersistentHitResults.Num() >= MaxHitResultsPerTrace)
					{
						// Treat PersistentHitResults like a queue, remove first element
						PersistentHitResults.RemoveAt(0);
					}
					PersistentHitResults.Add(HitResult);
				}
			}
			else
			{
				// ReticleActors for PersistentHitResults are handled later
				int32 ReticleIndex = TraceIndex * MaxHitResultsPerTrace + j;
				if (ReticleIndex < ReticleActors.Num())
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[ReticleIndex].Get())
					{
						const bool bHitActor = HitResult.GetActor() != nullptr;

						if (bHitActor && !HitResult.bBlockingHit)
						{
							LocalReticleActor->SetActorHiddenInGame(false);

							const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? HitResult.GetActor()->GetActorLocation() : HitResult.Location;

							LocalReticleActor->SetActorLocation(ReticleLocation);
							LocalReticleActor->SetIsTargetAnActor(bHitActor);
						}
						else
						{
							LocalReticleActor->SetActorHiddenInGame(true);
						}
					}
				}
			}
		}

		if (!bUsePersistentHitResults)
		{
			if (TraceHitResults.Num() < ReticleActors.Num())
			{
				// We have less hit results than ReticleActors, hide the extra ones
				for (int32 j = TraceHitResults.Num(); j < ReticleActors.Num(); j++)
				{
					if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[j].Get())
					{
						LocalReticleActor->SetIsTargetAnActor(false);
						LocalReticleActor->SetActorHiddenInGame(true);
					}
				}
			}
		}

		if (TraceHitResults.Num() < 1)
		{
			// If there were no hits, add a default HitResult at the end of the trace
			FHitResult HitResult;
			// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			HitResult.TraceEnd = TraceEnd;
			HitResult.Location = TraceEnd;
			HitResult.ImpactPoint = TraceEnd;
			TraceHitResults.Add(HitResult);

			if (bUsePersistentHitResults && PersistentHitResults.Num() < 1)
			{
				PersistentHitResults.Add(HitResult);
			}
		}

		ReturnHitResults.Append(TraceHitResults);
	}

	// Reminder: if bUsePersistentHitResults, Number of Traces = 1
	if (bUsePersistentHitResults && MaxHitResultsPerTrace > 0)
	{
		// Handle ReticleActors
		for (int32 PersistentHitResultIndex = 0; PersistentHitResultIndex < PersistentHitResults.Num(); PersistentHitResultIndex++)
		{
			FHitResult& HitResult = PersistentHitResults[PersistentHitResultIndex];

			// Update TraceStart because old persistent HitResults will have their original TraceStart and the player could have moved since then
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();

			if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
			{
				const bool bHitActor = HitResult.GetActor() != nullptr;

				if (bHitActor && !HitResult.bBlockingHit)
				{
					LocalReticleActor->SetActorHiddenInGame(false);

					const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? HitResult.GetActor()->GetActorLocation() : HitResult.Location;

					LocalReticleActor->SetActorLocation(ReticleLocation);
					LocalReticleActor->SetIsTargetAnActor(bHitActor);
				}
				else
				{
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}

		if (PersistentHitResults.Num() < ReticleActors.Num())
		{
			// We have less hit results than ReticleActors, hide the extra ones
			for (int32 PersistentHitResultIndex = PersistentHitResults.Num(); PersistentHitResultIndex < ReticleActors.Num(); PersistentHitResultIndex++)
			{
				if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[PersistentHitResultIndex].Get())
				{
					LocalReticleActor->SetIsTargetAnActor(false);
					LocalReticleActor->SetActorHiddenInGame(true);
				}
			}
		}
		return PersistentHitResults;
	}
	return ReturnHitResults;
}

AGameplayAbilityWorldReticle* AATA_Trace::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	// If Reticle is Valid, Create the Spawned Reticle
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, Location, Rotation);
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, MasterPC, ReticleParams);
			SpawnedReticleActor->SetActorHiddenInGame(true);
			ReticleActors.Add(SpawnedReticleActor);

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistency between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isn't a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}

			return SpawnedReticleActor;
		}
	}
	return nullptr;
}

void AATA_Trace::DestroyReticleActors()
{
	// Destroy All Existing Reticle Actors
	for (int32 i = ReticleActors.Num() - 1; i >= 0; i--)
	{
		if (ReticleActors[i].IsValid())
		{
			ReticleActors[i].Get()->Destroy();
		}
	}

	ReticleActors.Empty();
}
