// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundClassAccessor.h"

USoundClassAccessor::USoundClassAccessor()
{
}

void USoundClassAccessor::SetClassVolume(USoundClass* SoundClass, float volume)
{
	SoundClass->Properties.Volume = volume;
}

float USoundClassAccessor::GetClassVolume(USoundClass* SoundClass)
{
		return SoundClass->Properties.Volume;
}
