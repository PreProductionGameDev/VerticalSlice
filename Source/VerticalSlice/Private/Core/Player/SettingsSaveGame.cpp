// 2022 ChronoOwl Studios


#include "Core/Player/SettingsSaveGame.h"

#include "GameFramework/GameUserSettings.h"


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
	
	Jump = FKey("SpaceBar");
	PrimaryAction = FKey("LeftMouseButton");
	SecondaryAction = FKey("RightMouseButton");
	Reload = FKey("R");
	SMG = FKey("One");
	AssaultRifle = FKey("Two");
	Shotgun = FKey("Three");
	Sniper = FKey("Four");
	RocketLauncher = FKey("Five");
	NextWeapon = FKey("MouseWheelUp");
	PrevWeapon = FKey("MouseWheelDown");
	SwapAbility = FKey("Q");
	UtilityAbility = FKey("E");
	MovementAbility = FKey("Left Shift");
	OpenLeaderboard = FKey("Tab");
}
