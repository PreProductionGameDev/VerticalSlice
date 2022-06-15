// Fill out your copyright notice in the Description page of Project Settings.


#include "SavedSoundCue.h"

#include "SoundCueGraph/SoundCueGraph.h"


USavedSoundCue::USavedSoundCue()
{}

void USavedSoundCue::SetVolume(float newVolume)
{
	VolumeMultiplier = newVolume;
}

void USavedSoundCue::SetAttenuation(USoundAttenuation* Attenuation)
{
	AttenuationSettings = Attenuation;
}

USoundAttenuation* USavedSoundCue::GetAttenuation()
{
	return AttenuationSettings;
}

	float USavedSoundCue::SoundOverride(FTransform Origin, FVector Location , float Volume)
{
	if(AttenuationSettings)
	{
		return Volume * AttenuationSettings->Attenuation.Evaluate(Origin, Location);
	}
	else
	{
		return Volume;
	}
	
}

void USavedSoundCue::SetSoundWave(USoundWave* SoundWave)
{
	SoundNode = ConstructSoundNode<USoundNodeWavePlayer>();
	SoundNode->SetSoundWave(SoundWave);
	auto SoundOutPin = SoundNode->GraphNode->FindPin(FName("Output"));
	
	ModulationNode= ConstructSoundNode<USoundNodeModulator>();
	auto ModulationOutPin = ModulationNode->GraphNode->FindPin(FName("Output"));
	auto ModulationInPin = ModulationNode->GraphNode->CreatePin(EEdGraphPinDirection::EGPD_Input,FName("Input"),FName("Input"));

	SoundOutPin->MakeLinkTo(ModulationInPin);
	
	ModulationOutPin->MakeLinkTo((*SoundCueGraph->Nodes.begin())->Pins.Top());
	
	CompileSoundNodesFromGraphNodes();

	//SoundNode->Serialize(PropertyManager);
	//ModulationNode->Serialize(PropertyManager);
}

void USavedSoundCue::SetLooping(bool Looping)
{
	bLooping = Looping;
	SoundNode->bLooping=Looping;
}

bool USavedSoundCue::GetLooping()
{
	return bLooping;
}

void USavedSoundCue::SetModulation(FVector2D VolumeModulation, FVector2D PitchModulation)
{
	PitchModulationVector2D=PitchModulation;
	VolumeModulationVector2D = VolumeModulation;
	if (VolumeModulation.X >=VolumeModulation.Y)
	{
		ModulationNode->VolumeMax =(VolumeModulation.X);
		ModulationNode->VolumeMin =(VolumeModulation.Y);
	}
	else
	{
		ModulationNode->VolumeMax =(VolumeModulation.Y);
		ModulationNode->VolumeMin =(VolumeModulation.X);
	}
	if (PitchModulation.X >=PitchModulation.Y)
	{
		ModulationNode->PitchMax =(PitchModulation.X);
		ModulationNode->PitchMin =(PitchModulation.Y);
	}
	else
	{
		ModulationNode->PitchMax =(PitchModulation.Y);
		ModulationNode->PitchMin =(PitchModulation.X);
	}
}

FVector2D USavedSoundCue::GetVolumeModulation()
{
	return VolumeModulationVector2D;
}

FVector2D USavedSoundCue::GetPitchModulation()
{
	return PitchModulationVector2D;
}

void USavedSoundCue::SetSoundClass(USoundClass* SoundClass)
{
	SoundClassObject = SoundClass;
}

USoundClass* USavedSoundCue::GetSoundClass() const
{
	return SoundClassObject;
}

