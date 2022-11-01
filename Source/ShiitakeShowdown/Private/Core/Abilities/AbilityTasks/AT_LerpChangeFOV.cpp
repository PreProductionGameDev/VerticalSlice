// 2022 ChronoOwl Studios


#include "Core/Abilities/AbilityTasks/AT_LerpChangeFOV.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h"


UAT_LerpChangeFOV::UAT_LerpChangeFOV(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsFinished = false;
}

UAT_LerpChangeFOV* UAT_LerpChangeFOV::WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName,
	UCameraComponent* CameraComponent, float TargetFOV, float Duration, UCurveFloat* OptionalInterpolationCurve)
{
	// Create the Ability Task
	UAT_LerpChangeFOV* MyObj = NewAbilityTask<UAT_LerpChangeFOV>(OwningAbility, TaskInstanceName);

	// Populate its Variables
	// Set the Camera, Getting the FOVs
	MyObj->CameraComponent = CameraComponent;
	if (CameraComponent != nullptr)
	{
		MyObj->StartFOV = MyObj->CameraComponent->FieldOfView;
	}
	MyObj->TargetFOV = TargetFOV;
	// Avoids Negative or 0 cases
	MyObj->Duration = FMath::Max(Duration, 0.001f);
	// Calculate the Time to Start and End
	MyObj->TimeChangeStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeChangeWillEnd = MyObj->TimeChangeStarted + MyObj->Duration;
	MyObj->LerpCurve = OptionalInterpolationCurve;

	return MyObj;
}

void UAT_LerpChangeFOV::Activate()
{
}

void UAT_LerpChangeFOV::TickTask(float DeltaTime)
{
	// Quit if the task is complete
	if (bIsFinished)
	{
		return;
	}
	Super::TickTask(DeltaTime);

	// Ensure Camera Component is Valid and handle FOV Lerp
	if (CameraComponent)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		// Once current time is greater than the end time,
		// Set the final FOV and end the ability task
		if (CurrentTime >= TimeChangeWillEnd)
		{
			bIsFinished = true;
			// Ensures the Lerp isn't off by slight decimal position
			CameraComponent->SetFieldOfView(TargetFOV);
			// Broadcast Ability Delegates set in Blueprints
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnTargetFOVReached.Broadcast();
			}
			EndTask();
		}
		else
		{
			// Calculate the Fraction to Move in
			float MoveFraction = (CurrentTime - TimeChangeStarted) / Duration;
			// If using a Lerp Curve, Get value from that
			if (LerpCurve)
			{
				MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
			}
			// Get the NewFOV
			const float NewFOV = FMath::Lerp<float, float>(StartFOV, TargetFOV, MoveFraction);
			// Set the Camera's FOV
			CameraComponent->SetFieldOfView(NewFOV);
		}
	}
	else
	{
		// Camera is not valid, End Ability
		bIsFinished = true;
		EndTask();
	}
}

void UAT_LerpChangeFOV::OnDestroy(bool bAbilityIsEnding)
{
	Super::OnDestroy(bAbilityIsEnding);
}
