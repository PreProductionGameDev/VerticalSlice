#pragma once

UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAction,
	SecondaryAction
};

UENUM(BlueprintType)
enum class EElements : uint8
{
	Elem_Fire		UMETA(DisplayName = "Fire"),
	Elem_Air		UMETA(DisplayName = "Air"),
	Elem_Ice		UMETA(DisplayName = "Ice"),
	Elem_Earth		UMETA(DisplayName = "Earth")
};