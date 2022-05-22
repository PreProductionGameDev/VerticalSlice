// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ElementsEnum.generated.h"

UENUM(BlueprintType)
enum EElements
{
	Elem_Fire		UMETA(DisplayName = "Fire"),
	Elem_Air		UMETA(DisplayName = "Air"),
	Elem_Ice		UMETA(DisplayName = "Ice"),
	Elem_Earth		UMETA(DisplayName = "Earth")
};