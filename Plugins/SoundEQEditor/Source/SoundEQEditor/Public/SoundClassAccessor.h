// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundClass.h"
#include "SoundClassAccessor.generated.h"

/**
 * 
*/
UCLASS()
class  USoundClassAccessor final : public USoundClass
{
	GENERATED_BODY()

public:
	USoundClassAccessor();
	
	UFUNCTION(BlueprintCallable)
	static void SetClassVolume(USoundClass* SoundClass, float volume);

	UFUNCTION(BlueprintCallable)
	static float GetClassVolume(USoundClass* SoundClass);
};
