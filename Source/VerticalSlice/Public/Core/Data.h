// 2022 ChronoOwl Studios

#pragma once

#define COLLISION_PICKUP						ECollisionChannel::ECC_GameTraceChannel1

// The Input ID. Used heavily in the GAS Systems
UENUM(BlueprintType)
enum class EPlayerAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	PrimaryAction,
	SecondaryAction,
	Reload,
	PrevWeapon,
	NextWeapon,
	AbilityAction,
	Heal,
	Respawn,
	UtilityAbility,
	MovementAbility,
	SMG,
	Shotgun,
	Sniper,
	AssaultRifle,
	RocketLauncher,
};

