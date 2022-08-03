#pragma once

#define COLLISION_PICKUP						ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAction,
	SecondaryAction,
	AbilityAction,
	Heal,
	Reload,
	Respawn,
	FireAbility,
	IceAbility,
	LightningAbility,
	FireMovement,
	IceMovement,
	LightningMovement
};

UENUM(BlueprintType)
enum class EElements : uint8
{
	Elem_Fire		UMETA(DisplayName = "Fire"),
	Elem_Air		UMETA(DisplayName = "Air"),
	Elem_Ice		UMETA(DisplayName = "Ice"),
	Elem_Earth		UMETA(DisplayName = "Earth")
};