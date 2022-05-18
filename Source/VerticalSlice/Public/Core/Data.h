#pragma once

UENUM(BlueprintType)
enum class ETestAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAction,
	SecondaryAction
};