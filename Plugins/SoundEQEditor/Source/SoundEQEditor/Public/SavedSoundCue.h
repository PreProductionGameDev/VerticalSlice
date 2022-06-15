// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundNodeWavePlayer.h"
#include "Sound/SoundNodeModulator.h"
#include "SavedSoundCue.generated.h"


/**
 * 
*/
UCLASS()
class  USavedSoundCue : public USoundCue
{
	GENERATED_BODY()

public:	
	
	USavedSoundCue();

	UFUNCTION(BlueprintCallable)
	void SetVolume(float newVolume);

	UFUNCTION(BlueprintCallable)
	void SetAttenuation(USoundAttenuation* Attenuation);
	
	UFUNCTION(BlueprintCallable)
	USoundAttenuation* GetAttenuation();

	UFUNCTION(BlueprintCallable)
	float SoundOverride(FTransform Origin, FVector Location, float Volume);

	UFUNCTION(BlueprintCallable)
	void SetSoundWave(USoundWave* SoundWave);

	UFUNCTION(BlueprintCallable)
	void SetLooping(bool Looping);

	UFUNCTION(BlueprintCallable)
	bool GetLooping();

	UFUNCTION(BlueprintCallable)
	void SetModulation(FVector2D VolumeModulation, FVector2D PitchModulation);
	
	UFUNCTION(BlueprintCallable)
	FVector2D GetVolumeModulation();

	UFUNCTION(BlueprintCallable)
	FVector2D GetPitchModulation();
	
	UFUNCTION(BlueprintCallable)
	void SetSoundClass(USoundClass* SoundClass);

	UFUNCTION(BlueprintCallable)
	virtual USoundClass* GetSoundClass() const override;

	UPROPERTY(BlueprintReadOnly)
	bool bLooping;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D VolumeModulationVector2D;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D PitchModulationVector2D;
private:
	UPROPERTY()
	TObjectPtr<USoundNodeWavePlayer> SoundNode;
	
	UPROPERTY()
	TObjectPtr<USoundNodeModulator> ModulationNode;
	
};
