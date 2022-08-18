// 2022 ChronoOwl Studios

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_LerpChangeFOV.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLerpChangeFOVDelegate);

/**
 *	Ability Task for Changing the FOV for an inputted Camera over a linear lerp or float curve
 */
UCLASS()
class VERTICALSLICE_API UAT_LerpChangeFOV : public UAbilityTask
{
	GENERATED_BODY()
	// Default Constructor
	UAT_LerpChangeFOV(const FObjectInitializer& ObjectInitializer);
	// Delegate Set in Blueprints
	UPROPERTY(BlueprintAssignable)
	FLerpChangeFOVDelegate OnTargetFOVReached;
	// Change the FOV to a specified value over time. Float curve can be used (Between 0-1) or uses default linear interpolation
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_LerpChangeFOV* WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName, class UCameraComponent* CameraComponent, float TargetFOV, float Duration, UCurveFloat* OptionalInterpolationCurve);

	// Ability Task Overrides
	// Ability Activation
	virtual void Activate() override;
	// Ability Tick - Used for the main Task Logic
	virtual void TickTask(float DeltaTime) override;
	// Ability Destroy
	virtual void OnDestroy(bool bAbilityIsEnding) override;

protected:
	bool bIsFinished;
	// The Initial FOV started at
	float StartFOV;
	// The FOV to end up at
	float TargetFOV;
	// Duration for the Lerp
	float Duration;
	// The Time Started
	float TimeChangeStarted;
	// The Time ended
	float TimeChangeWillEnd;
	// The Camera we are applying the FOV change to
	class UCameraComponent* CameraComponent;
	// The Optional Curve for the Lerp
	UCurveFloat* LerpCurve;
};
