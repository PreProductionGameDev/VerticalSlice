// 2022 ChronoOwl Studios


#include "Core/Player/SettingsSaveGame.h"

USettingsSaveGame::USettingsSaveGame() : USaveGame()
{
	MainVolume = 0.75f;
	SFXVolume = 0.75f;
	MusicVolume = 0.75f;

	MouseSensitivity = 1.0f;

	PlayerModel = 0;

	PrimaryHue.Add(0.0f);
	PrimaryHue.Add(0.0f);
	PrimaryHue.Add(0.0f);

	SecondaryHue.Add(0.0f);
	SecondaryHue.Add(0.0f);
	SecondaryHue.Add(0.0f);

	HeadItem.Add(TEXT("0"));
	HeadItem.Add(TEXT("0"));
	HeadItem.Add(TEXT("0"));

	BodyItem.Add(TEXT("0"));
	BodyItem.Add(TEXT("0"));
	BodyItem.Add(TEXT("0"));

	ExtraItem.Add(TEXT("0"));
	ExtraItem.Add(TEXT("0"));
	ExtraItem.Add(TEXT("0"));

	bHoldToZoom = true;

	Jump = FKey("Space Bar");
	PrimaryAction = FKey("Left Mouse Button");
	SecondaryAction = FKey("Right Mouse Button");
	Reload = FKey("R");
	SMG = FKey("1");
	Shotgun = FKey("3");
	Sniper = FKey("4");
	AssaultRifle = FKey("2");
	RocketLauncher = FKey("5");
	NextWeapon = FKey("Mouse Wheel Up");
	PrevWeapon = FKey("Mouse Wheel Down");
	SwapAbility = FKey("Q");
	UtilityAbility = FKey("E");
	MovementAbility = FKey("Left Shift");
	OpenLeaderboard = FKey("Tab");

}
